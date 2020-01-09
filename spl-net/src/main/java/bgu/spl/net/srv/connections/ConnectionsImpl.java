package bgu.spl.net.srv.connections;

import sun.reflect.generics.reflectiveObjects.NotImplementedException;

import java.util.concurrent.ConcurrentMap;

public class ConnectionsImpl<T> implements Connections<T> {
    private final ConcurrentMap<Integer, ? extends Client<T>> clientsMap;

    public ConnectionsImpl(ConcurrentMap<Integer, ? extends Client<T>> clientsMap) {
        this.clientsMap = clientsMap;
    }

    @Override
    public boolean send(int connectionId, T msg) {
        clientsMap.get(connectionId).connection().send(msg);
        return true;
    }

    @Override
    public void send(String channel, T msg) {
        throw new NotImplementedException();
    }

    @Override
    public void disconnect(int connectionId) {
        removeConnection(connectionId);
    }

    private void removeConnection(int connectionId) {
        clientsMap.remove(connectionId);
    }
}
