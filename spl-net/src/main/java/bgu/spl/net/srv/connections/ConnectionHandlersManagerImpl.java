package bgu.spl.net.srv.connections;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class ConnectionHandlersManagerImpl<T> implements ConnectionHandlersManager<T> {
    private final ConnectionIdsManager connectionIdsManager;

    private final Map<Integer, ConnectionHandler<T>> connectionHandlersMap;
    private final ReadWriteLock connectionHandlersMapRwLock;

    private final Map<String, Topic<T>> subscriptionsMap;
    private final ReadWriteLock subscriptionsMapRwLock;

    public ConnectionHandlersManagerImpl(ConnectionIdsManager connectionIdsManager) {
        this.connectionIdsManager = connectionIdsManager;

        connectionHandlersMap = new ConcurrentHashMap<>();
        connectionHandlersMapRwLock = new ReentrantReadWriteLock();

        subscriptionsMap = new ConcurrentHashMap<>();
        subscriptionsMapRwLock = new ReentrantReadWriteLock();
    }

    @Override
    public int nextConnectionId() {
        return connectionIdsManager.nextConnectionId();
    }

    @Override
    public void addConnectionHandler(int connectionId, ConnectionHandler<T> connectionHandler) {
        connectionHandlersMapRwLock.writeLock().lock();
        try {
            connectionHandlersMap.put(connectionId, connectionHandler);
        }
        finally {
            connectionHandlersMapRwLock.writeLock().unlock();
        }
    }

    @Override
    public boolean send(int connectionId, T msg) {
        connectionHandlersMapRwLock.readLock().lock();
        ConnectionHandler<T> connectionHandler;
        try {
            connectionHandler = connectionHandlersMap.get(connectionId);
        }
        finally {
            connectionHandlersMapRwLock.readLock().unlock();
        }

        connectionHandler.send(msg);
        return true;
    }

//    @Override
//    public void send(String channel, T msg) {
//        // TODO: implement
//        throw new NotImplementedException();
//    }

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

        t.addSubscriber(connectionId, attachment);
        return true;
    }

    @Override
    public boolean unsubscribe(int connectionId, String topic) {
        getTopic(topic).removeSubscriber(connectionId);
        return true;
    }

    @Override
    public Iterable<ConnectionSubscriptionInfo<T>> getConnectionsSubscribedTo(String topic) {
        List<ConnectionSubscriptionInfo<T>> subscribers;
        Topic<T> t = getTopic(topic);
        t.lock.readLock().lock();
        try {
            subscribers = new LinkedList<>(t.subscribers.values());
        }
        finally {
            t.lock.readLock().unlock();
        }

        return subscribers;
    }

    private Topic<T> addTopic(String topic) {
        Topic<T> t;
        subscriptionsMapRwLock.writeLock().lock();
        try {
            t = subscriptionsMap.put(topic, new Topic<>());
        }
        finally {
            subscriptionsMapRwLock.writeLock().unlock();
        }
        return t;
    }

    private Topic<T> getTopic(String topic) {
        Topic<T> t;
        subscriptionsMapRwLock.readLock().lock();
        try {
            t = subscriptionsMap.get(topic);
        } finally {
            subscriptionsMapRwLock.readLock().unlock();
        }
        return t;
    }

    private void removeConnection(int connectionId) {
        connectionHandlersMapRwLock.writeLock().lock();
        try {
            connectionHandlersMap.remove(connectionId);
        }
        finally {
            connectionHandlersMapRwLock.writeLock().unlock();
        }
    }

    private void removeAllSubscriptions(int connectionId) {
        subscriptionsMapRwLock.readLock().lock();
        try {
            removeClientFromAllTopics(connectionId);
        }
        finally {
            subscriptionsMapRwLock.readLock().unlock();
        }
    }

    private void removeClientFromAllTopics(int connectionId) {
        for (String topic : subscriptionsMap.keySet()) {
            removeClientFromTopic(connectionId, topic);
        }
    }

    private void removeClientFromTopic(int connectionId, String topic) {
        subscriptionsMap.get(topic).removeSubscriber(connectionId);
    }

    private static class Topic<T> {
        Map<Integer, ConnectionSubscriptionInfo<T>> subscribers;
        ReadWriteLock lock;

        Topic() {
            subscribers = new HashMap<>();
            lock = new ReentrantReadWriteLock();
        }

        void addSubscriber(int connectionId, Object attachment) {
            lock.writeLock().lock();
            try {
                subscribers.put(connectionId, new ConnectionSubscriptionInfo<>(connectionId, attachment));
            }
            finally {
                lock.writeLock().unlock();
            }
        }

        void removeSubscriber(int connectionId) {
            lock.writeLock().lock();
            try {
                subscribers.remove(connectionId);
            }
            finally {
                lock.writeLock().unlock();
            }
        }
    }
}
