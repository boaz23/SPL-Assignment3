package bgu.spl.net.srv;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.StompMessageProcessor;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.api.frames.Receipt;

import java.util.HashMap;
import java.util.Map;

public class StompMessagingProtocolImpl implements StompMessagingProtocol {
    private static IdCount messageId = new IdCount();

    private Map<String, StompMessageProcessor<Frame>> comMap;

    public StompMessagingProtocolImpl(){
        InitMap();
    }

    @Override
    public void start(int connectionId, Connections<Frame> connections) {

    }

    @Override
    public void process(Frame message) {
        String messageT = message.getMessageType();

    }

    @Override
    public boolean shouldTerminate() {
        return false;
    }

    protected class SubscribeMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public Frame process(Frame message) {
            //TODO how is in control of managing to topics subs, then register the
            // user to the requested topic
            return new Receipt(String.format("%d",messageId.getNewId()));
        }
    }

    private void InitMap() {
        comMap = new HashMap<>();
        comMap.put("SUBSCRIBE", new SubscribeMessageProcessor());
    }
}
