package bgu.spl.net.srv.connections;

public class ConnectionSubscriptionInfo<T> {
    private final int connectionId;
    private Object attachment;

    public ConnectionSubscriptionInfo(int connectionId, Object attachment) {
        this.connectionId = connectionId;
        this.attachment = attachment;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public Object getAttachment() {
        return attachment;
    }

    public void setAttachment(Object attachment) {
        this.attachment = attachment;
    }
}
