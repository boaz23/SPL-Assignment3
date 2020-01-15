package bgu.spl.net.impl.stomp;

import bgu.spl.net.ReadWriteLockedMap;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.connections.*;

import java.util.Collections;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.locks.ReadWriteLock;

// TODO: check synchronized access
public class StompConnections extends ConnectionsImpl<Frame> implements ConnectionHandlersManager<Frame> {
    private final ConnectionIdsManager connectionIdsManager;

    private final ConcurrentMap<String, User> usersMap;
    private final ConcurrentMap<Integer, StompClient> clientsMap;
    private final ReadWriteLockedMap<String, Topic<Frame>> topicMap;

    public StompConnections(ConnectionIdsManager connectionIdsManager) {
        this(connectionIdsManager, new ConcurrentHashMap<>());
    }
    private StompConnections(ConnectionIdsManager connectionIdsManager, ConcurrentMap<Integer, StompClient> clientsMap) {
        super(clientsMap);
        this.connectionIdsManager = connectionIdsManager;

        this.clientsMap = clientsMap;
        topicMap = new ReadWriteLockedMap<>();
        usersMap = new ConcurrentHashMap<>();
    }

    public void addUser(int connectionId, String username, String password) {
        User user = new User(username, password);
        clientsMap.get(connectionId).setUser(user);
        usersMap.put(username, user);
    }

    public void connectUser(String username, int connectionId) {
        User user = usersMap.get(username);
        clientsMap.get(connectionId).setUser(user);
        user.setConnected(true);
    }

    public User getUser(String username) {
        return usersMap.get(username);
    }

    public StompClient getClient(int connectionId) {
        return clientsMap.get(connectionId);
    }

    public User getUser(int connectionId) {
        return clientsMap.get(connectionId).user();
    }

    SubscriptionAttachment getSubscriptionAttachment(int connectionId, String topic) {
        return clientsMap.get(connectionId).getSubscriptionAttachment(topic);
    }

    boolean isSubscriptionAttachmentUsed(int connectionId, SubscriptionAttachment attachment) {
        return clientsMap.get(connectionId).isSubscriptionAttachmentUsed(attachment);
    }

    @Override
    public int nextConnectionId() {
        return connectionIdsManager.nextConnectionId();
    }

    @Override
    public void addConnectionHandler(int connectionId, ConnectionHandler<Frame> connectionHandler) {
        clientsMap.put(connectionId, new StompClient(connectionHandler));
    }

    public void logoutUser(int connectionId) {
        StompClient client = clientsMap.get(connectionId);
        if (client != null) {
            User user = client.user();
            if (user != null) {
                user.setConnected(false);
            }
            removeClientFromAllTopics(connectionId, client);
        }
    }

    public boolean subscribe(String topic, int connectionId, SubscriptionAttachment attachment) {
        Topic<Frame> t = topicMap.computeIfAbsent(topic, x -> new Topic<>());
        clientsMap.get(connectionId).addSubscription(topic, attachment);
        t.addSubscriber(connectionId, attachment);
        return true;
    }

    public boolean unsubscribe(int connectionId, SubscriptionAttachment attachment) {
        StompClient client = clientsMap.get(connectionId);
        String topic = client.getTopicOfSubscriptionAttachment(attachment);
        if (topic == null) {
            return false;
        }

        Topic<Frame> t = topicMap.get(topic);
        if (t == null) {
            return false;
        }
        t.removeSubscriber(connectionId);
        client.removeSubscription(topic);
        return true;
    }

    public Iterable<ConnectionSubscriptionInfo<Frame>> getConnectionsSubscribedTo(String topic) {
        Topic<Frame> t = topicMap.get(topic);
        if (t == null) {
            return Collections.emptyList();
        }
        return t.subscribers();
    }

    private void removeClientFromAllTopics(int connectionId, StompClient client) {
        ReadWriteLock topicLock = topicMap.internalLock();
        topicLock.readLock().lock();
        try {
            for (String topic : client.subscriptions()) {
                removeClientFromTopic(connectionId, topic);
            }
        }
        finally {
            topicLock.readLock().unlock();
        }
        client.clearSubscriptions();
    }

    private void removeClientFromTopic(int connectionId, String topic) {
        Topic<Frame> t = topicMap.getWithoutLock(topic);
        if (t == null) {
            return;
        }
        t.removeSubscriber(connectionId);
    }
}
