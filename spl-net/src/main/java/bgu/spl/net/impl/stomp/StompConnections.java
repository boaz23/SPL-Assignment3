package bgu.spl.net.impl.stomp;

import bgu.spl.net.ReadWriteLockedMap;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.connections.*;

import java.util.Collections;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.locks.ReadWriteLock;

public class StompConnections extends ConnectionsImpl<Frame> implements ConnectionHandlersManager<Frame> {
    private final ConnectionIdsManager connectionIdsManager;

    private final ConcurrentMap<String, User> usersMap;
    private final ConcurrentMap<Integer, StompClient> clientsMap;
    private final ReadWriteLockedMap<String, Topic<Frame>> topicMap;

    /**
     * Constructor
     * @param connectionIdsManager
     */
    public StompConnections(ConnectionIdsManager connectionIdsManager) {
        this(connectionIdsManager, new ConcurrentHashMap<>(), new ReadWriteLockedMap<>());
    }
    private StompConnections(ConnectionIdsManager connectionIdsManager,
                             ConcurrentMap<Integer, StompClient> clientsMap,
                             ReadWriteLockedMap<String, Topic<Frame>> topicMap) {
        super(clientsMap, topicMap);
        this.connectionIdsManager = connectionIdsManager;

        this.clientsMap = clientsMap;
        this.topicMap = topicMap;
        usersMap = new ConcurrentHashMap<>();
    }

    /**
     * Adding a user to the class data
     * @param connectionId connection id
     * @param username username identifier
     * @param password password of the user
     */
    public void addUser(int connectionId, String username, String password) {
        User user = new User(username, password);
        clientsMap.get(connectionId).setUser(user);
        usersMap.put(username, user);
    }

    /**
     * Set a user With the username to be connected
     * @param username username identifier
     * @param connectionId id associated with the username
     */
    public void connectUser(String username, int connectionId) {
        User user = usersMap.get(username);
        clientsMap.get(connectionId).setUser(user);
        user.setConnected(true);
    }

    /**
     * Return the User instance class of the user with username
     * @param username username identifier
     * @return User instance class of the user with username
     */
    public User getUser(String username) {
        return usersMap.get(username);
    }

    /**
     *
     * @param connectionId connection id
     * @return
     */
    public StompClient getClient(int connectionId) {
        return clientsMap.get(connectionId);
    }

    /**
     * Return the User instance class of the user identified by connectionId
     * @param connectionId connection id
     * @return User instance class of the user identified by connectionId
     */
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

    /**
     * Handle the logout of the user
     * @param connectionId connection id
     */
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


    @Override
    public void disconnect(int connectionId) {
        logoutUser(connectionId);
        super.disconnect(connectionId);
    }

    /**
     * Add the connection id to the topic, Create one if needed
     * Add the SubscriptionAttachment to the connectionId in the topic
     * @param topic the topic
     * @param connectionId connection id
     * @param attachment SubscriptionAttachment <connectionId, topic>
     * @return true if the connection id was added to the topic, false otherwise
     */
    public boolean subscribe(String topic, int connectionId, SubscriptionAttachment attachment) {
        Topic<Frame> t = topicMap.computeIfAbsent(topic, x -> new Topic<>());
        clientsMap.get(connectionId).addSubscription(topic, attachment);
        t.addSubscriber(connectionId, attachment);
        return true;
    }

    /**
     * Remove a connection id from a topic, using attachment to identify the topic
     * @param connectionId connection id
     * @param attachment attachment
     * @return true if the user was unsubscribe, false otherwise
     */
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
