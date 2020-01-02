package bgu.spl.net.api.frames;

public class MessageFrame extends Frame {
    private static final String SUBSCRIPTION_HEADER = "subscription";
    private static final String MESSAGE_ID_HEADER = "Message-id";
    private static final String DESTINATION_HEADER = "destination";

    public MessageFrame(String body, String subscription, String messageId, String destination) {
        super("MESSAGE", body);
        setSubscription(subscription);
        setMessageId(messageId);
        setDestination(destination);
    }

    public String getSubscription() {
        return getHeader(SUBSCRIPTION_HEADER);
    }

    public void setSubscription(String subscription) {
        setHeader(SUBSCRIPTION_HEADER, subscription);
    }

    public String getMessageId() {
        return getHeader(MESSAGE_ID_HEADER);
    }

    public void setMessageId(String messageId) {
        setHeader(MESSAGE_ID_HEADER, messageId);
    }

    public String getDestination() {
        return getHeader(DESTINATION_HEADER);
    }

    public void setDestination(String destination) {
        setHeader(DESTINATION_HEADER, destination);
    }
}
