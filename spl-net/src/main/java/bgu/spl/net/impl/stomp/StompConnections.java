package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.connections.ConnectionHandlersManagerImpl;
import bgu.spl.net.srv.connections.ConnectionIdsManager;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

public class StompConnections extends ConnectionHandlersManagerImpl<Frame> {
    private final ConcurrentMap<String, String> usersMap;

    public StompConnections(ConnectionIdsManager connectionIdsManager) {
        super(connectionIdsManager);
        usersMap = new ConcurrentHashMap<>();
    }

    public void addUser(String username, String password) {
        usersMap.put(username, password);
    }

    public String getUserPassword(String username) {
        return usersMap.get(username);
    }
}
