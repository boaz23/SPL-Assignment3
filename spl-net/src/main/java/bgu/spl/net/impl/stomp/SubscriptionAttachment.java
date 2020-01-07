package bgu.spl.net.impl.stomp;

public class SubscriptionAttachment {
    private String subscriptionId;

    public SubscriptionAttachment(String subscriptionId) {
        this.subscriptionId = subscriptionId;
    }

    public String getSubscriptionId() {
        return subscriptionId;
    }
}
