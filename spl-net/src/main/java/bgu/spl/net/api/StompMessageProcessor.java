package bgu.spl.net.api;

public interface StompMessageProcessor<T> {
    T process(T message);
}
