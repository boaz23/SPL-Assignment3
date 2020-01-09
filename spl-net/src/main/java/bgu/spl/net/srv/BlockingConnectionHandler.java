package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.connections.ConnectionHandler;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    protected final MessagingProtocol<T> protocol;
    protected final MessageEncoderDecoder<T> encdec;
    protected final Socket socket;

    protected final int connectionId;
    protected final ServerConnectionHandlerActions<T> connections;

    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;

    public BlockingConnectionHandler(
        Socket socket,
        MessageEncoderDecoder<T> reader,
        MessagingProtocol<T> protocol,
        int connectionId,
        ServerConnectionHandlerActions<T> connections) {

        this.socket = socket;
        this.encdec = reader;
        this.protocol = protocol;

        this.connectionId = connectionId;
        this.connections = connections;
    }

    @Override
    public void run() {
        try (Socket sock = this.socket) { //just for automatic closing
            int read;

            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());

            initialize();
            connections.add(connectionId, this);
            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                T nextMessage = encdec.decodeNextByte((byte)read);
                if (nextMessage != null) {
                    protocol.process(nextMessage);
                }
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        finally {
            connections.remove(connectionId);
        }
    }

    @Override
    public void close() throws IOException {
        connected = false;

        // order is important.
        // closing a socket might throw an exception so we want to remove
        // the connection handler before doing so.
        connections.remove(connectionId);
        socket.close();
    }

    @Override
    public void send(T msg) throws IOException {
        if (msg != null) {
            synchronized (this) {
                out.write(encdec.encode(msg));
                out.flush();
            }
        }
    }

    /**
     * Perform protocol handshake with client.
     * Allows overriding in deriving classes
     */
    protected void initialize() {
        connections.startProtocol(connectionId, protocol);
    }
}
