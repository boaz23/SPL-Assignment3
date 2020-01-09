package bgu.spl.net.srv.connections;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

public class ConnectionHandlersManagerImpl<T> implements ConnectionHandlersManager<T> {
    private final ConnectionIdsManager connectionIdsManager;

    private final ConcurrentMap<Integer, Client<T>> clientsMap;
    private final ConcurrentMap<String, Topic<T>> subscriptionsMap;

    public ConnectionHandlersManagerImpl(ConnectionIdsManager connectionIdsManager) {
        this.connectionIdsManager = connectionIdsManager;

        clientsMap = new ConcurrentHashMap<>();
        subscriptionsMap = new ConcurrentHashMap<>();
    }

    @Override
    public int nextConnectionId() {
        return connectionIdsManager.nextConnectionId();
    }

    @Override
    public void addConnectionHandler(int connectionId, ConnectionHandler<T> connectionHandler) {
        clientsMap.put(connectionId, new Client<>(connectionHandler));
    }

    @Override
    public boolean send(int connectionId, T msg) {
        clientsMap.get(connectionId).connection.send(msg);
        return true;
    }

    @Override
    public void disconnect(int connectionId) {
        removeConnection(connectionId);
        removeAllSubscriptions(connectionId);
    }

    @Override
    public boolean subscribe(String topic, int connectionId, Object attachment) {
        Topic<T> t = getTopic(topic);
        if (t == null) {
            t = addTopic(topic);
        }

        clientsMap.get(connectionId).addSubscription(topic);
        t.addSubscriber(connectionId, attachment);
        return true;
    }

    @Override
    public boolean unsubscribe(int connectionId, String topic) {
        getTopic(topic).removeSubscriber(connectionId);
        clientsMap.get(connectionId).removeSubscription(topic);
        return true;
    }

    @Override
    public Iterable<ConnectionSubscriptionInfo<T>> getConnectionsSubscribedTo(String topic) {
        return getTopic(topic).subscribers();
    }

    private Topic<T> addTopic(String topic) {
        return subscriptionsMap.put(topic, new Topic<>());
    }

    private Topic<T> getTopic(String topic) {
        return subscriptionsMap.get(topic);
    }

    private void removeConnection(int connectionId) {
        clientsMap.remove(connectionId);
    }

    private void removeAllSubscriptions(int connectionId) {
        removeClientFromAllTopics(connectionId);
    }

    private void removeClientFromAllTopics(int connectionId) {
        for (String topic : clientsMap.get(connectionId).subscriptions()) {
            removeClientFromTopic(connectionId, topic);
        }
    }

    private void removeClientFromTopic(int connectionId, String topic) {
        subscriptionsMap.get(topic).removeSubscriber(connectionId);
    }

    private static class Topic<T> {
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

    private static class Client<T> {
        ConnectionHandler<T> connection;

        private static final Object subscriptionValue = new Object();
        ConcurrentMap<String, Object> subscriptions;

        private Client(ConnectionHandler<T> connection) {
            this.connection = connection;
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
}
