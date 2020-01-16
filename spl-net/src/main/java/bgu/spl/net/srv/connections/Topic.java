package bgu.spl.net.srv.connections;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

/**
 * Represents a topic/channel that connections can subscribe to
 * @param <T> The type parameter for the connection handler
 */
public class Topic<T> {
    private ConcurrentMap<Integer, Subscription<T>> subscribers;

    /**
     * Initializes a new topic
     */
    public Topic() {
        subscribers = new ConcurrentHashMap<>();
    }

    /**
     * Adds a subscriber to this topic
     * @param connectionId The connection id of the subscriber
     * @param attachment An additional attachment object that is attached
     *                   to this subscription used by the user of this class
     */
    public void addSubscriber(int connectionId, Object attachment) {
        subscribers.put(connectionId, new Subscription<>(connectionId, attachment));
    }

    /**
     * Unsubscribe a subscriber from this topic
     * @param connectionId The subscriber connection id to remove
     */
    public void removeSubscriber(int connectionId) {
        subscribers.remove(connectionId);
    }

    /**
     * @param connectionId The connection id of the subscriber
     * @return The supplied attachment object when the specified subscriber was added with
     */
    public Object getSubscriptionAttachment(int connectionId) {
        return subscribers.get(connectionId).getAttachment();
    }

    /**
     * @return An iterable of all subscribers currently subscribed to this topic
     */
    public Iterable<Subscription<T>> subscribers() {
        return subscribers.values();
    }
}
