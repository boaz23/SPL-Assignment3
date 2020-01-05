package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.connections.ConnectionHandlersManagerImpl;
import bgu.spl.net.srv.connections.ConnectionIdsManager;
import sun.reflect.generics.reflectiveObjects.NotImplementedException;

import java.util.HashMap;
import java.util.Map;

public class StompConnections extends ConnectionHandlersManagerImpl<Frame> {
    private final Map<String, String> usersMap;

    public StompConnections(ConnectionIdsManager connectionIdsManager) {
        super(connectionIdsManager);
        usersMap = new HashMap<>();
    }

    public void addUser(String username, String password) {
        throw new NotImplementedException();
    }

    public String getUserPassword(String username) {
        throw new NotImplementedException();
    }
}
