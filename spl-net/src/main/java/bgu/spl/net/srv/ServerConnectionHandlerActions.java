package bgu.spl.net.srv;

import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.connections.ConnectionHandler;

public interface ServerConnectionHandlerActions<T> {
    void add(int connectionId, ConnectionHandler<T> connectionHandler);
    void remove(int connectionId);
    void startProtocol(int connectionId, MessagingProtocol<T> protocol);
}
