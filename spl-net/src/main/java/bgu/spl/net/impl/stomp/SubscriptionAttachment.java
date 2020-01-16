package bgu.spl.net.impl.stomp;

import java.util.Objects;

public class SubscriptionAttachment {
    private String subscriptionId;

    /**
     * Constructor of the class
     * @param subscriptionId the subscription id
     */
    public SubscriptionAttachment(String subscriptionId) {
        this.subscriptionId = subscriptionId;
    }

    /**
     * @return the subscription id
     */
    public String getSubscriptionId() {
        return subscriptionId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof SubscriptionAttachment)) return false;
        SubscriptionAttachment that = (SubscriptionAttachment) o;
        return subscriptionId.equals(that.subscriptionId);
    }

    @Override
    public int hashCode() {
        return Objects.hash(subscriptionId);
    }
}
