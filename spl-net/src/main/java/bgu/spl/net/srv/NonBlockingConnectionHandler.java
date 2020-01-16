package bgu.spl.net.srv;

import bgu.spl.net.FileLogger;
import bgu.spl.net.StringBufferLogger;
import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.connections.ConnectionHandler;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

public class NonBlockingConnectionHandler<T> implements ConnectionHandler<T> {

    private static final int BUFFER_ALLOCATION_SIZE = 1 << 13; //8k
    private static final ConcurrentLinkedQueue<ByteBuffer> BUFFER_POOL = new ConcurrentLinkedQueue<>();

    protected final MessagingProtocol<T> protocol;
    private final MessageEncoderDecoder<T> encdec;
    private final Queue<ByteBuffer> writeQueue = new ConcurrentLinkedQueue<>();
    private final SocketChannel chan;
    private final Reactor<T> reactor;

    protected final int connectionId;
    protected final ServerConnectionHandlerActions<T> connections;

    public NonBlockingConnectionHandler(
            MessageEncoderDecoder<T> reader,
            MessagingProtocol<T> protocol,
            SocketChannel chan,
            Reactor<T> reactor,

            int connectionId,
            ServerConnectionHandlerActions<T> connections) {
        this.chan = chan;
        this.encdec = reader;
        this.protocol = protocol;
        this.reactor = reactor;

        this.connectionId = connectionId;
        this.connections = connections;
    }

    public Runnable continueRead() {
        ByteBuffer buf = leaseBuffer();

        boolean success = false;
        try {
            success = chan.read(buf) != -1;
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        if (success) {
            buf.flip();
            return () -> {
                try {
                    while (buf.hasRemaining()) {
                        T nextMessage = null;
                        try {
                            nextMessage = encdec.decodeNextByte(buf.get());
                        }
                        catch (RuntimeException e) {
                            e.printStackTrace();
                            FileLogger.incoming.appendLine("invalid frame.\n--------------------");
                        }
                        if (nextMessage != null) {
                            StringBufferLogger.ReactorLogger.appendLine("handling frame: " + ((Frame)nextMessage).getMessageType());
                            FileLogger.incoming.appendLine("--------------------");
                            protocol.process(nextMessage);
                        }
                    }
                } finally {
                    releaseBuffer(buf);
                }
            };
        } else {
            releaseBuffer(buf);
            close();
            return null;
        }
    }

    protected void init() {
        connections.startProtocol(connectionId, protocol);
        connections.add(connectionId, this);
    }

    boolean closed = false;
    public void close() {
        closed = true;
        StringBufferLogger.ReactorLogger.appendLine("closing...");
        try {
            chan.close();
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        connections.remove(connectionId);
        StringBufferLogger.ReactorLogger.appendLine("closed");
    }

    public boolean isClosed() {
        return !chan.isOpen();
    }

    public void continueWrite() {
        while (!writeQueue.isEmpty()) {
            try {
                ByteBuffer top = writeQueue.peek();
                chan.write(top);
                if (top.hasRemaining()) {
                    return;
                } else {
                    writeQueue.remove();
                }
            } catch (IOException ex) {
                ex.printStackTrace();
                close();
            }
        }

        if (writeQueue.isEmpty()) {
            if (protocol.shouldTerminate()) close();
            else reactor.updateInterestedOps(chan, SelectionKey.OP_READ, 0);
        }
    }

    private static ByteBuffer leaseBuffer() {
        ByteBuffer buff = BUFFER_POOL.poll();
        if (buff == null) {
            return ByteBuffer.allocateDirect(BUFFER_ALLOCATION_SIZE);
        }

        buff.clear();
        return buff;
    }

    private static void releaseBuffer(ByteBuffer buff) {
        BUFFER_POOL.add(buff);
    }

    int time = 0;
    @Override
    public void send(T msg) {
        if (msg != null) {
            if (!closed && !isClosed()) {
                StringBufferLogger.ReactorLogger.appendLine("sending frame: " + ((Frame)msg).getMessageType());
                writeQueue.add(ByteBuffer.wrap(encdec.encode(msg)));
                reactor.updateInterestedOps(chan, SelectionKey.OP_READ | SelectionKey.OP_WRITE, time++);
            }
            else {
                StringBufferLogger.ReactorLogger.appendLine("sending frame: " + ((Frame)msg).getMessageType() + " | but the channel is closed.");
            }
        }
    }
}
