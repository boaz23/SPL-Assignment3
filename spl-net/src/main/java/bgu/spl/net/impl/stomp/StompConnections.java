package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.connections.*;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

public class StompConnections extends ConnectionsImpl<Frame> implements ConnectionHandlersManager<Frame> {
    private final ConcurrentMap<String, User> usersMap;
    private final ConnectionIdsManager connectionIdsManager;
    private final ConcurrentMap<Integer, StompClient> clientsMap;
    private final ConcurrentMap<String, Topic<Frame>> subscriptionsMap;

    public StompConnections(ConnectionIdsManager connectionIdsManager) {
        this(connectionIdsManager, new ConcurrentHashMap<>());
    }
    private StompConnections(ConnectionIdsManager connectionIdsManager, ConcurrentMap<Integer, StompClient> clientsMap) {
        super(clientsMap);
        this.connectionIdsManager = connectionIdsManager;
        this.clientsMap = clientsMap;
        subscriptionsMap = new ConcurrentHashMap<>();
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

    @Override
    public void disconnect(int connectionId) {
        StompClient client = clientsMap.get(connectionId);
        if (client != null) {
            if (client.user() != null) {
                client.user().setConnected(false);
            }
            super.disconnect(connectionId);
            removeClientFromAllTopics(connectionId, client);
        }
    }

    public boolean subscribe(String topic, int connectionId, SubscriptionAttachment attachment) {
        Topic<Frame> t = getTopic(topic);
        if (t == null) {
            t = addTopic(topic);
        }

        clientsMap.get(connectionId).addSubscription(topic, attachment);
        t.addSubscriber(connectionId, attachment);
        return true;
    }

    public boolean unsubscribe(int connectionId, SubscriptionAttachment attachment) {
        StompClient client = clientsMap.get(connectionId);
        String topic = client.getTopicOfSubscriptionAttachment(attachment);

        getTopic(topic).removeSubscriber(connectionId);
        client.removeSubscription(topic);
        return true;
    }

    public Iterable<ConnectionSubscriptionInfo<Frame>> getConnectionsSubscribedTo(String topic) {
        return getTopic(topic).subscribers();
    }

    private Topic<Frame> addTopic(String topic) {
        return subscriptionsMap.put(topic, new Topic<>());
    }

    private Topic<Frame> getTopic(String topic) {
        return subscriptionsMap.get(topic);
    }

    private void removeClientFromAllTopics(int connectionId, StompClient client) {
        for (String topic : client.subscriptions()) {
            removeClientFromTopic(connectionId, topic);
        }
        client.clearSubscriptions();
    }

    private void removeClientFromTopic(int connectionId, String topic) {
        subscriptionsMap.get(topic).removeSubscriber(connectionId);
    }
}
