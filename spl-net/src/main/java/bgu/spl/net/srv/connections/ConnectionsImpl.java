package bgu.spl.net.srv.connections;

import bgu.spl.net.ReadWriteLockedMap;

import java.io.IOException;
import java.util.Collections;
import java.util.concurrent.ConcurrentMap;

/**
 * Implementation of the {@link Connections<T> interface}
 * @param <T> The type parameter for the connection handler
 */
public class ConnectionsImpl<T> implements Connections<T> {
    private final ConcurrentMap<Integer, ? extends Client<T>> clientsMap;
    private final ReadWriteLockedMap<String, Topic<T>> topicMap;

    /**
     * Initializes a new basic connections implementations with the given
     * clients map and topic map
     * @param clientsMap The clients map
     * @param topicMap The topic map
     */
    public ConnectionsImpl(ConcurrentMap<Integer, ? extends Client<T>> clientsMap,
                           ReadWriteLockedMap<String, Topic<T>> topicMap) {
        this.clientsMap = clientsMap;
        this.topicMap = topicMap;
    }

    @Override
    public boolean send(int connectionId, T msg) {
        try {
            Client<T> client = clientsMap.get(connectionId);
            if (client != null) {
                client.connection().send(msg);
                return true;
            }
            throw new RuntimeException("how did we get here");
        } catch (IOException ignored) {
        }

        return false;
    }

    @Override
    public void send(String channel, T msg) {
        Iterable<Subscription<T>> subscribers = getConnectionsSubscribedTo(channel);
        for (Subscription<T> conn : subscribers) {
            send(conn.getConnectionId(), msg);
        }
    }

    /**
     * @param topic the topic
     * @return An iterable of all the subscriptions of this topic
     */
    public Iterable<Subscription<T>> getConnectionsSubscribedTo(String topic) {
        Topic<T> t = topicMap.get(topic);
        if (t == null) {
            return Collections.emptyList();
        }
        return t.subscribers();
    }

    @Override
    public void disconnect(int connectionId) {
        removeConnection(connectionId);
    }

    private void removeConnection(int connectionId) {
        clientsMap.remove(connectionId);
    }
}
