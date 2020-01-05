package bgu.spl.net.srv.connections;

public class ConnectionInfo<T> {
    private ConnectionHandler<T> connectionHandler;
    private Object attachment;

    public ConnectionInfo(ConnectionHandler<T> connectionHandler, Object attachment) {
        this.connectionHandler = connectionHandler;
        this.attachment = attachment;
    }

    public ConnectionHandler<T> getConnectionHandler() {
        return connectionHandler;
    }

    public Object getAttachment() {
        return attachment;
    }

    public void setAttachment(Object attachment) {
        this.attachment = attachment;
    }
}
