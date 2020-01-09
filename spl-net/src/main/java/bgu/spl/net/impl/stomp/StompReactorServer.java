package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.NonBlockingConnectionHandler;
import bgu.spl.net.srv.Reactor;
import bgu.spl.net.srv.connections.ConnectionIdsManager;

import java.nio.channels.SocketChannel;
import java.util.function.Supplier;

public class StompReactorServer extends Reactor<Frame> {
    public StompReactorServer(int numThreads,
                              int port,
                              Supplier<? extends MessagingProtocol<Frame>> protocolFactory,
                              Supplier<? extends MessageEncoderDecoder<Frame>> readerFactory,
                              ConnectionIdsManager connectionIdsManager) {
        super(numThreads, port, protocolFactory, readerFactory, new StompConnections(connectionIdsManager));
    }

    @Override
    protected NonBlockingConnectionHandler<Frame> createConnectionHandler(SocketChannel clientChan,
                                                                          MessageEncoderDecoder<Frame> encdec,
                                                                          MessagingProtocol<Frame> protocol,
                                                                          int nextId) {
        return new StompNonBlockingConnectionHandler(
            (StompMessageEncoderDecoder)encdec,
            (StompMessagingProtocol)protocol,
            clientChan,
        this,
                nextId,
                new StompConnectionsHandlerActions()
        );
    }

    protected class StompConnectionsHandlerActions extends ConnectionsHandlerActions {
    }
}
