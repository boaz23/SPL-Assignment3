package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

public abstract class BaseServer<T> implements Server<T> {
    private final int port;
    private final Supplier<? extends MessagingProtocol<T>> protocolFactory;
    private final Supplier<? extends MessageEncoderDecoder<T>> encdecFactory;
    protected final Connections<T> connections;
    private ServerSocket sock;

    public BaseServer(
        int port,
        Supplier<? extends MessagingProtocol<T>> protocolFactory,
        Supplier<? extends MessageEncoderDecoder<T>> encdecFactory,
        Connections<T> connections) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
        this.connections = connections;
        this.sock = null;
    }

    @Override
    public void serve() {
        try (ServerSocket serverSock = new ServerSocket(port)) {
			System.out.println("Server started");

            this.sock = serverSock; //just to be able to close

            while (!Thread.currentThread().isInterrupted()) {

                Socket clientSock = serverSock.accept();

                // TODO: generate connection ID
                BlockingConnectionHandler<T> handler = createConnectionHandler(
                        clientSock,
                        encdecFactory.get(),
                        protocolFactory.get(),
                        // TODO: generate connection ID
                        0);

                execute(handler);
            }
        } catch (IOException ex) {
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
		if (sock != null)
			sock.close();
    }

    void addConnectionHanlder(int connectionId, ConnectionHandler<T> connectionHandler) {

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

    protected abstract void execute(BlockingConnectionHandler<T>  handler);

    protected class ConnectionsHandlerActions implements ServerConnectionHandlerActions<T> {
        @Override
        public void add(int connectionId, ConnectionHandler<T> connectionHandler) {
            BaseServer.this.addConnectionHanlder(connectionId, connectionHandler);
        }
    }
}