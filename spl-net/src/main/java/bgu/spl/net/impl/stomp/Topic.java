package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.connections.ConnectionSubscriptionInfo;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

class Topic<T> {
    ConcurrentMap<Integer, ConnectionSubscriptionInfo<T>> subscribers;

    Topic() {
        subscribers = new ConcurrentHashMap<>();
    }

    void addSubscriber(int connectionId, Object attachment) {
        subscribers.put(connectionId, new ConnectionSubscriptionInfo<>(connectionId, attachment));
    }

    void removeSubscriber(int connectionId) {
        subscribers.remove(connectionId);
    }

    Iterable<ConnectionSubscriptionInfo<T>> subscribers() {
        return subscribers.values();
    }
}
