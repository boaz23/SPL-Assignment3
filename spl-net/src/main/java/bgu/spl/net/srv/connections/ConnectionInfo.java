package bgu.spl.net.srv.connections;

public class ConnectionInfo<T> {
    private final int connectionId;
    private final ConnectionHandler<T> connectionHandler;
    private Object attachment;

    public ConnectionInfo(int connectionId, ConnectionHandler<T> connectionHandler, Object attachment) {
        this.connectionId = connectionId;
        this.connectionHandler = connectionHandler;
        this.attachment = attachment;
    }

    public int getConnectionId() {
        return connectionId;
    }

    ConnectionHandler<T> getConnectionHandler() {
        return connectionHandler;
    }

    public Object getAttachment() {
        return attachment;
    }

    public void setAttachment(Object attachment) {
        this.attachment = attachment;
    }
}
