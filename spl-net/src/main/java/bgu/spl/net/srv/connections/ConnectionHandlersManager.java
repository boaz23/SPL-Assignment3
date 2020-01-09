package bgu.spl.net.srv.connections;

public interface ConnectionHandlersManager<T> extends Connections<T> {
    int nextConnectionId();
    void addConnectionHandler(int connectionId, ConnectionHandler<T> connectionHandler);
    boolean subscribe(String topic, int connectionId, Object attachment);
    boolean unsubscribe(int connectionId, String topic);
    Iterable<ConnectionSubscriptionInfo<T>> getConnectionsSubscribedTo(String topic);
}
