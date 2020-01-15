package bgu.spl.net.srv.connections;

import bgu.spl.net.Logger;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.impl.stomp.StompClient;
import bgu.spl.net.impl.stomp.StompFrameEncoderDecoder;
import bgu.spl.net.impl.stomp.User;
import sun.reflect.generics.reflectiveObjects.NotImplementedException;

import java.io.IOException;
import java.util.concurrent.ConcurrentMap;

public class ConnectionsImpl<T> implements Connections<T> {
    private final ConcurrentMap<Integer, ? extends Client<T>> clientsMap;

    public ConnectionsImpl(ConcurrentMap<Integer, ? extends Client<T>> clientsMap) {
        this.clientsMap = clientsMap;
    }

    @Override
    public boolean send(int connectionId, T msg) {
        try {
            Client<T> client = clientsMap.get(connectionId);
            if (client != null) {
                User user = ((StompClient)client).user();
                if (user != null) {
                    Logger.outgoing.appendLine("to user: " + user.username() + "\n" + StompFrameEncoderDecoder.toString((Frame) msg) + "--------------------");
                }
                else {
                    Logger.outgoing.appendLine("to unkown user:\n" + StompFrameEncoderDecoder.toString((Frame) msg) + "--------------------");
                }
                client.connection().send(msg);
                return true;
            }
            throw new RuntimeException("how did we get here");
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        return false;
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
