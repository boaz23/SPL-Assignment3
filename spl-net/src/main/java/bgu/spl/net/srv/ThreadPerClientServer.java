package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;

import java.util.function.Supplier;

public class ThreadPerClientServer<T> extends BaseServer<T> {
    public ThreadPerClientServer(int port,
                                 Supplier<? extends MessagingProtocol<T>> protocolFactory,
                                 Supplier<? extends MessageEncoderDecoder<T>> encdecFactory,
                                 Connections<T> connections) {
        super(port, protocolFactory, encdecFactory, connections);
    }

    @Override
    protected void execute(BlockingConnectionHandler<T> handler) {
        new Thread(handler).start();
    }
}
