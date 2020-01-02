package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.ServerConnectionHandlerActions;

public interface StompServerConnectionHandlerActions extends ServerConnectionHandlerActions<Frame> {
    void startProtocol(int connectionId, StompMessagingProtocol protocol);
}
