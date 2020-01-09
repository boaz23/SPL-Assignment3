package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.connections.*;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

public class StompConnections extends ConnectionsImpl<Frame> implements ConnectionHandlersManager<Frame> {
    private final ConcurrentMap<String, String> usersMap;
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

    public void addUser(String username, String password) {
        usersMap.put(username, password);
    }

    public String getUserPassword(String username) {
        return usersMap.get(username);
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
        super.disconnect(connectionId);
        removeAllSubscriptions(connectionId);
    }

    @Override
    public boolean subscribe(String topic, int connectionId, Object attachment) {
        Topic<Frame> t = getTopic(topic);
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
    public Iterable<ConnectionSubscriptionInfo<Frame>> getConnectionsSubscribedTo(String topic) {
        return getTopic(topic).subscribers();
    }

    private Topic<Frame> addTopic(String topic) {
        return subscriptionsMap.put(topic, new Topic<>());
    }

    private Topic<Frame> getTopic(String topic) {
        return subscriptionsMap.get(topic);
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
}
