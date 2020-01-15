package bgu.spl.net.srv.connections;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

public class Topic<T> {
    private ConcurrentMap<Integer, ConnectionSubscriptionInfo<T>> subscribers;

    public Topic() {
        subscribers = new ConcurrentHashMap<>();
    }

    public void addSubscriber(int connectionId, Object attachment) {
        subscribers.put(connectionId, new ConnectionSubscriptionInfo<>(connectionId, attachment));
    }

    public void removeSubscriber(int connectionId) {
        subscribers.remove(connectionId);
    }

    public Object getSubscriptionAttachment(int connectionId) {
        return subscribers.get(connectionId).getAttachment();
    }

    public Iterable<ConnectionSubscriptionInfo<T>> subscribers() {
        return subscribers.values();
    }
}
