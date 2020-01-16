package bgu.spl.net.srv.connections;

/**
 * Represents a client that is connected to the server (with a connection handler)
 * @param <T>
 */
public class Client<T> {
    private ConnectionHandler<T> connection;

    /**
     * Initializes a new client instance with the given connection handler
     * @param connection The connection handler
     */
    public Client(ConnectionHandler<T> connection) {
        this.connection = connection;
    }

    /**
     * @return The connection handler of this client
     */
    public ConnectionHandler<T> connection() {
        return connection;
    }
}
