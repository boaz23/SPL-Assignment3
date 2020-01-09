package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.connections.ConnectionHandler;
import bgu.spl.net.srv.connections.ConnectionHandlersManager;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

public abstract class BaseServer<T> implements Server<T> {
    private final int port;
    private final Supplier<? extends MessagingProtocol<T>> protocolFactory;
    private final Supplier<? extends MessageEncoderDecoder<T>> encdecFactory;
    private ServerSocket serverSocket;

    protected final ConnectionHandlersManager<T> connectionHandlersManager;

    public BaseServer(
        int port,
        Supplier<? extends MessagingProtocol<T>> protocolFactory,
        Supplier<? extends MessageEncoderDecoder<T>> encdecFactory,
        ConnectionHandlersManager<T> connectionHandlersManager) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
        this.serverSocket = null;

        this.connectionHandlersManager = connectionHandlersManager;
    }

    @Override
    public void serve() {
        try (ServerSocket serverSocket = new ServerSocket(port)) {
			System.out.println("Server started");
            this.serverSocket = serverSocket; //just to be able to close
            while (!Thread.currentThread().isInterrupted()) {
                Socket clientSock = serverSocket.accept();
                BlockingConnectionHandler<T> handler = createConnectionHandler(
                    clientSock,
                    encdecFactory.get(),
                    protocolFactory.get(),
                    connectionHandlersManager.nextConnectionId()
                );
                execute(handler);
            }
        } catch (IOException ex) {
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
		if (serverSocket != null)
			serverSocket.close();
    }

    private void addConnectionHanlder(int connectionId, ConnectionHandler<T> connectionHandler) {
        connectionHandlersManager.addConnectionHandler(connectionId, connectionHandler);
    }

    private void removeConnectionHandler(int connectionId) {
        connectionHandlersManager.disconnect(connectionId);
    }

    protected BlockingConnectionHandler<T> createConnectionHandler(
        Socket socket,
        MessageEncoderDecoder<T> encdec,
        MessagingProtocol<T> protocol,
        int connectionId
    ) {
        return new BlockingConnectionHandler<>(
            socket,
            encdecFactory.get(),
            protocolFactory.get(),
            connectionId,
            new ConnectionsHandlerActions()
        );
    }

    private void startProtocol(int connectionId, MessagingProtocol<T> protocol) {
        protocol.start(connectionId, connectionHandlersManager);
    }

    protected abstract void execute(BlockingConnectionHandler<T>  handler);

    protected class ConnectionsHandlerActions implements ServerConnectionHandlerActions<T> {
        @Override
        public void add(int connectionId, ConnectionHandler<T> connectionHandler) {
            BaseServer.this.addConnectionHanlder(connectionId, connectionHandler);
        }

        @Override
        public void remove(int connectionId) {
            BaseServer.this.removeConnectionHandler(connectionId);
        }

        @Override
        public void startProtocol(int connectionId, MessagingProtocol<T> protocol) {
            BaseServer.this.startProtocol(connectionId, protocol);
        }
    }
}