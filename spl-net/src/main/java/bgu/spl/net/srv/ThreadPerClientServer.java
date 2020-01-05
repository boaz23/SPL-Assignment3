package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.connections.ConnectionHandlersManager;

import java.util.function.Supplier;

public class ThreadPerClientServer<T> extends BaseServer<T> {
    public ThreadPerClientServer(int port,
                                 Supplier<? extends MessagingProtocol<T>> protocolFactory,
                                 Supplier<? extends MessageEncoderDecoder<T>> encdecFactory,
                                 ConnectionHandlersManager<T> connectionHandlersManager) {
        super(port, protocolFactory, encdecFactory, connectionHandlersManager);
    }

    @Override
    protected void execute(BlockingConnectionHandler<T> handler) {
        new Thread(handler).start();
    }
}
