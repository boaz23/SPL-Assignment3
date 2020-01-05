package bgu.spl.net.srv.connections;

import sun.reflect.generics.reflectiveObjects.NotImplementedException;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class ConnectionHandlersManagerImpl<T> implements ConnectionHandlersManager<T> {
    private final ConnectionIdsManager connectionIdsManager;
    private final Map<Integer, ConnectionHandler<T>> connectionHandlersMap;
    private ReadWriteLock connectionHandlersMapRwLock;
    // TODO: manage and store a map for each topic of subscribed clients

    public ConnectionHandlersManagerImpl(ConnectionIdsManager connectionIdsManager) {
        this.connectionIdsManager = connectionIdsManager;
        connectionHandlersMap = new HashMap<>();
        connectionHandlersMapRwLock = new ReentrantReadWriteLock();
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
        // TODO: implement
        throw new NotImplementedException();
    }

//    @Override
//    public void send(String channel, T msg) {
//        // TODO: implement
//        throw new NotImplementedException();
//    }

    @Override
    public void disconnect(int connectionId) {
        connectionHandlersMapRwLock.writeLock().lock();
        try {
            connectionHandlersMap.remove(connectionId);
        }
        finally {
            connectionHandlersMapRwLock.writeLock().unlock();
        }
    }

    @Override
    public boolean subscribe(String topic, int connectionId, Object attachment) {
        // TODO: implement
        throw new NotImplementedException();
    }

    @Override
    public boolean unsubscribe(int connectionId, String topic) {
        // TODO: implement
        throw new NotImplementedException();
    }

    @Override
    public Iterable<ConnectionInfo<T>> getConnectionsSubscribedTo(String topic) {
        // TODO: implement
        throw new NotImplementedException();
    }
}
