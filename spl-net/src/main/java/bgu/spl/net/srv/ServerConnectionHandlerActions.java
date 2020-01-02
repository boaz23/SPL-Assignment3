package bgu.spl.net.srv;

public interface ServerConnectionHandlerActions<T> {
    void add(int connectionId, ConnectionHandler<T> connectionHandler);
}
