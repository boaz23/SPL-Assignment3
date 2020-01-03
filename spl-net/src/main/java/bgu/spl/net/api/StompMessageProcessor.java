package bgu.spl.net.api;

public interface StompMessageProcessor<T> {
    void process(T message);
}
