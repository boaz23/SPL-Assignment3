package bgu.spl.net.srv;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    boolean subscribeToChannel(String channel, String connectionId,
                               String subscriptionId);

    boolean unsubscribe(String connectionId, String subscriptionId);

    String getsubscriptionById(String topic, int clientId);
}
