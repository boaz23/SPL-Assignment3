package bgu.spl.net.api;

/**
 * Interface to Stomp message processor, each process should handle a message
 * T
 * @param <T>
 */
public interface StompMessageProcessor<T> {
    void process(T message);
}
