package bgu.spl.net.srv.connections;

public interface ConnectionHandlersManager<T> extends Connections<T> {
    int nextConnectionId();
    void addConnectionHandler(int connectionId, ConnectionHandler<T> connectionHandler);
}
