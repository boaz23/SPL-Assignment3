package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.connections.Client;
import bgu.spl.net.srv.connections.ConnectionHandler;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

class StompClient extends Client<Frame> {
    private static final Object subscriptionValue = new Object();
    ConcurrentMap<String, Object> subscriptions;

    StompClient(ConnectionHandler<Frame> connection) {
        super(connection);
        this.subscriptions = new ConcurrentHashMap<>();
    }

    void addSubscription(String topic) {
        subscriptions.put(topic, subscriptionValue);
    }

    void removeSubscription(String topic) {
        subscriptions.remove(topic);
    }

    Iterable<String> subscriptions() {
        return subscriptions.keySet();
    }
}
