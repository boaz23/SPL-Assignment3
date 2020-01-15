package bgu.spl.net.srv.connections;

/**
 * Stores relevant information for a single subscription operation,
 * e.g. the connection ID that subscribed and the attachment of the subscription
 * @param <T> The type parameter for the connection handler
 */
public class Subscription<T> {
    private final int connectionId;
    private Object attachment;

    /**
     * Initializes a new subscription info information with the specified connection id and attachment
     * @param connectionId The connection ID that subscribed
     * @param attachment The attachment that is attached to this subscription
     */
    public Subscription(int connectionId, Object attachment) {
        this.connectionId = connectionId;
        this.attachment = attachment;
    }

    /**
     * @return The connection id that subscribed
     */
    public int getConnectionId() {
        return connectionId;
    }

    /**
     * @return The attachment that was supplied during subscription
     */
    public Object getAttachment() {
        return attachment;
    }

    /**
     * Sets an attachment for this particular subscription represented by this instance
     * @param attachment The attachment to set to
     */
    public void setAttachment(Object attachment) {
        this.attachment = attachment;
    }
}
