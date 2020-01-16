package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.impl.stomp.StompFrameEncoderDecoder;
import bgu.spl.net.srv.connections.ConnectionHandler;
import bgu.spl.net.srv.connections.ConnectionHandlersManager;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.*;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.function.Supplier;

public class Reactor<T> implements Server<T> {

    private final int port;
    private final Supplier<? extends MessagingProtocol<T>> protocolFactory;
    private final Supplier<? extends MessageEncoderDecoder<T>> readerFactory;
    private final ActorThreadPool pool;
    private Selector selector;

    private Thread selectorThread;
    private final ConcurrentLinkedQueue<Runnable> selectorTasks = new ConcurrentLinkedQueue<>();

    protected final ConnectionHandlersManager<T> connectionHandlersManager;

    public Reactor(
            int numThreads,
            int port,
            Supplier<? extends MessagingProtocol<T>> protocolFactory,
            Supplier<? extends MessageEncoderDecoder<T>> readerFactory,
            ConnectionHandlersManager<T> connectionHandlersManager) {

        this.pool = new ActorThreadPool(numThreads);
        this.port = port;
        this.protocolFactory = protocolFactory;
        this.readerFactory = readerFactory;
        this.connectionHandlersManager = connectionHandlersManager;
    }

    @Override
    public void serve() {
	selectorThread = Thread.currentThread();
        try (Selector selector = Selector.open();
                ServerSocketChannel serverSock = ServerSocketChannel.open()) {

            this.selector = selector; //just to be able to close

            serverSock.bind(new InetSocketAddress(port));
            serverSock.configureBlocking(false);
            serverSock.register(selector, SelectionKey.OP_ACCEPT);
			System.out.println("Server started");

            while (!Thread.currentThread().isInterrupted()) {

                selector.select();
                runSelectionThreadTasks();

                for (SelectionKey key : selector.selectedKeys()) {

                    if (!key.isValid()) {
                        continue;
                    } else if (key.isAcceptable()) {
                        handleAccept(serverSock, selector);
                    } else {
                        handleReadWrite(key);
                    }
                }

                selector.selectedKeys().clear(); //clear the selected keys set so that we can know about new events

            }

        } catch (ClosedSelectorException ex) {
            //do nothing - server was requested to be closed
        } catch (IOException ex) {
            //this is an error
            ex.printStackTrace();
        }

        System.out.println("server closed!!!");
        pool.shutdown();
    }

    /*package*/ void updateInterestedOps(SocketChannel chan, int ops) {
        final SelectionKey key = chan.keyFor(selector);
        if (Thread.currentThread() == selectorThread) {
            key.interestOps(ops);
        } else {
            selectorTasks.add(() -> {
                key.interestOps(ops);
            });
            selector.wakeup();
        }
    }


    private void handleAccept(ServerSocketChannel serverChan, Selector selector) throws IOException {
        SocketChannel clientChan = serverChan.accept();
        clientChan.configureBlocking(false);

        int nextId = connectionHandlersManager.nextConnectionId();
        MessageEncoderDecoder<T> encdec = readerFactory.get();
        StompFrameEncoderDecoder tmp = (StompFrameEncoderDecoder) encdec;
        final NonBlockingConnectionHandler<T> handler = createConnectionHandler(clientChan, encdec, protocolFactory.get(), nextId);
        handler.init();
        tmp.setStuff(connectionHandlersManager, nextId);
        clientChan.register(selector, SelectionKey.OP_READ, handler);
    }

    private void handleReadWrite(SelectionKey key) {
        @SuppressWarnings("unchecked")
        NonBlockingConnectionHandler<T> handler = (NonBlockingConnectionHandler<T>) key.attachment();

        if (key.isReadable()) {
            Runnable task = handler.continueRead();
            if (task != null) {
                pool.submit(handler, task);
            }
        }

	    if (key.isValid() && key.isWritable()) {
            handler.continueWrite();
        }
    }

    private void runSelectionThreadTasks() {
        while (!selectorTasks.isEmpty()) {
            selectorTasks.remove().run();
        }
    }

    @Override
    public void close() throws IOException {
        selector.close();
    }

    /**
     * A factory method for creating the connection handler
     * @param clientChan The socket
     * @param encdec The encoder/decoder
     * @param protocol The protocol
     * @param nextId The connetion id
     * @return A new instance of a connection handler with the specified parameters
     */
    protected NonBlockingConnectionHandler<T> createConnectionHandler(SocketChannel clientChan,
                                                                      MessageEncoderDecoder<T> encdec,
                                                                      MessagingProtocol<T> protocol,
                                                                      int nextId) {
        return new NonBlockingConnectionHandler<>(
                encdec,
                protocol,
                clientChan,
                this,
                nextId,
                new ConnectionsHandlerActions());
    }

    private void addConnectionHanlder(int connectionId, ConnectionHandler<T> connectionHandler) {
        connectionHandlersManager.addConnectionHandler(connectionId, connectionHandler);
    }

    private void removeConnectionHandler(int connectionId) {
        connectionHandlersManager.disconnect(connectionId);
    }

    private void startProtocol(int connectionId, MessagingProtocol<T> protocol) {
        protocol.start(connectionId, connectionHandlersManager);
    }

    protected class ConnectionsHandlerActions implements ServerConnectionHandlerActions<T> {
        @Override
        public void add(int connectionId, ConnectionHandler<T> connectionHandler) {
            Reactor.this.addConnectionHanlder(connectionId, connectionHandler);
        }

        @Override
        public void remove(int connectionId) {
            Reactor.this.removeConnectionHandler(connectionId);
        }

        @Override
        public void startProtocol(int connectionId, MessagingProtocol<T> protocol) {
            Reactor.this.startProtocol(connectionId, protocol);
        }
    }
}
