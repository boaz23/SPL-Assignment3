package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.NonBlockingConnectionHandler;
import bgu.spl.net.srv.Reactor;
import bgu.spl.net.srv.ServerConnectionHandlerActions;

import java.nio.channels.SocketChannel;

public class StompNonBlockingConnectionHandler extends NonBlockingConnectionHandler<Frame> {
    public StompNonBlockingConnectionHandler(StompMessageEncoderDecoder reader,
                                             StompMessagingProtocol protocol,
                                             SocketChannel chan,
                                             Reactor<Frame> reactor,
                                             int connectionId,
                                             ServerConnectionHandlerActions<Frame> connections) {
        super(reader, protocol, chan, reactor, connectionId, connections);
    }

    @Override
    protected void init() {
        connections.startProtocol(connectionId, protocol);
        super.init();
    }
}
