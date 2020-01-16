package bgu.spl.net.srv.connections;

/**
 * A connections that with methods that allow for managing connection handlers
 * such as getting a fresh connection id for an incoming accepted connection
 * and adding a new connection handler to the connections
 * @param <T> The type paramter for the connection handler
 */
public interface ConnectionHandlersManager<T> extends Connections<T> {
    int nextConnectionId();
    void addConnectionHandler(int connectionId, ConnectionHandler<T> connectionHandler);
}
