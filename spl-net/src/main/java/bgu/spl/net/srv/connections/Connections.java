package bgu.spl.net.srv.connections;

public interface Connections<T> {
    boolean send(int connectionId, T msg);

//    void send(String topic, T msg);

    void disconnect(int connectionId);

    boolean subscribe(String topic, int connectionId, Object attachment);

    boolean unsubscribe(int connectionId, String topic);

    Iterable<ConnectionSubscriptionInfo<T>> getConnectionsSubscribedTo(String topic);
}
