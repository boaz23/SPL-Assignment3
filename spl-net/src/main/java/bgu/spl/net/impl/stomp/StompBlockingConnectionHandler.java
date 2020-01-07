package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.BlockingConnectionHandler;

import java.net.Socket;

public class StompBlockingConnectionHandler extends BlockingConnectionHandler<Frame> {
    public StompBlockingConnectionHandler(
        Socket socket,
        StompMessageEncoderDecoder reader,
        StompMessagingProtocol protocol,
        int connectionId,
        StompServerConnectionHandlerActions connections) {

        super(socket, reader, protocol, connectionId, connections);
    }

    @Override
    protected void initialize() {
        ((StompServerConnectionHandlerActions)connections).startProtocol(connectionId, (StompMessagingProtocol)protocol);
    }
}
