package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.NonBlockingConnectionHandler;
import bgu.spl.net.srv.Reactor;

import java.nio.channels.SocketChannel;

public class StompNonBlockingConnectionHandler extends NonBlockingConnectionHandler<Frame> {
    public StompNonBlockingConnectionHandler(StompMessageEncoderDecoder reader,
                                             StompMessagingProtocol protocol,
                                             SocketChannel chan,
                                             Reactor reactor,
                                             int connectionId,
                                             StompServerConnectionHandlerActions connections) {
        super(reader, protocol, chan, reactor, connectionId, connections);
    }

    @Override
    protected void init() {
        ((StompServerConnectionHandlerActions)connections).startProtocol(connectionId, (StompMessagingProtocol)protocol);
        super.init();
    }
}
