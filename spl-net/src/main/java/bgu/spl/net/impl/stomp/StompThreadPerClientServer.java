package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.BlockingConnectionHandler;
import bgu.spl.net.srv.connections.ConnectionHandlersManager;
import bgu.spl.net.srv.ThreadPerClientServer;
import bgu.spl.net.srv.connections.ConnectionIdsManager;

import java.net.Socket;
import java.util.function.Supplier;

public class StompThreadPerClientServer extends ThreadPerClientServer<Frame> {
    public StompThreadPerClientServer(
            int port,
            Supplier<StompMessagingProtocol> protocolFactory,
            Supplier<StompMessageEncoderDecoder> encdecFactory,
            ConnectionIdsManager connectionIdsManager) {
        super(port, protocolFactory, encdecFactory, new StompConnections(connectionIdsManager));
    }

    @Override
    protected BlockingConnectionHandler<Frame> createConnectionHandler(
        Socket socket,
        MessageEncoderDecoder<Frame> encdec,
        MessagingProtocol<Frame> protocol,
        int connectionId) {

        return new StompBlockingConnectionHandler(
            socket,
            (StompMessageEncoderDecoder)encdec,
            (StompMessagingProtocol)protocol,
            connectionId,
            new StompConnectionsHandlerActions()
        );
    }

    private void startProtocol(int connectionId, StompMessagingProtocol protocol) {
        protocol.start(connectionId, connectionHandlersManager);
    }

    protected class StompConnectionsHandlerActions extends ConnectionsHandlerActions
    implements StompServerConnectionHandlerActions {
        @Override
        public void startProtocol(int connectionId, StompMessagingProtocol protocol) {
            StompThreadPerClientServer.this.startProtocol(connectionId, protocol);
        }
    }
}
