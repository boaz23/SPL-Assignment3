package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.connections.Client;
import bgu.spl.net.srv.connections.ConnectionHandler;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

public class StompClient extends Client<Frame> {
    private ConcurrentMap<String, SubscriptionAttachment> subscriptions;
    private ConcurrentMap<SubscriptionAttachment, String> subscritionIdsToTopic;
    private User user;

    StompClient(ConnectionHandler<Frame> connection) {
        super(connection);
        subscriptions = new ConcurrentHashMap<>();
        subscritionIdsToTopic = new ConcurrentHashMap<>();
    }

    void addSubscription(String topic, SubscriptionAttachment subscriptionAttachment) {
        subscriptions.put(topic, subscriptionAttachment);
        subscritionIdsToTopic.put(subscriptionAttachment, topic);
    }

    void removeSubscription(String topic) {
        SubscriptionAttachment attachment = subscriptions.get(topic);
        subscriptions.remove(topic);
        subscritionIdsToTopic.remove(attachment);
    }

    SubscriptionAttachment getSubscriptionAttachment(String topic) {
        return subscriptions.get(topic);
    }

    boolean isSubscriptionAttachmentUsed(SubscriptionAttachment attachment) {
        return subscritionIdsToTopic.containsKey(attachment);
    }

    String getTopicOfSubscriptionAttachment(SubscriptionAttachment attachment) {
        return subscritionIdsToTopic.get(attachment);
    }

    public User user() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    Iterable<String> subscriptions() {
        return subscriptions.keySet();
    }

    void clearSubscriptions() {
        subscriptions = new ConcurrentHashMap<>();
        subscritionIdsToTopic = new ConcurrentHashMap<>();
    }
}
