package bgu.spl.net.srv.connections;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    boolean subscribeToChannel(String channel, String connectionId,
                               String subscriptionId);

    boolean unsubscribe(String connectionId, String subscriptionId);

    String getsubscriptionById(String topic, int clientId);

    // TODO: add method for subscribing and unsubscribing
    // TODO: attachment on subscribe that holds the subscription id
    // TODO: add a method for a getting the list of all subscribed clients to a specific topic with their attachment
}
