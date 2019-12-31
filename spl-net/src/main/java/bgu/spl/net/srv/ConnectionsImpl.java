package bgu.spl.net.srv;

import sun.reflect.generics.reflectiveObjects.NotImplementedException;

public class ConnectionsImpl<T> implements Connections<T> {
    @Override
    public boolean send(int connectionId, T msg) {
        throw new NotImplementedException();
    }

    @Override
    public void send(String channel, T msg) {
        throw new NotImplementedException();
    }

    @Override
    public void disconnect(int connectionId) {
        throw new NotImplementedException();
    }
}
