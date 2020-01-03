package bgu.spl.net.srv;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.StompMessageProcessor;
import bgu.spl.net.api.frames.*;

import java.util.HashMap;
import java.util.Map;

public class StompMessagingProtocolImpl implements StompMessagingProtocol {
    private static IdCount messageId = new IdCount();

    private int connectionId;
    private Connections<Frame> connections;
    private Map<String, StompMessageProcessor<Frame>> comMap;

    public StompMessagingProtocolImpl(){
        InitMap();
    }

    @Override
    public void start(int connectionId, Connections<Frame> connections) {

    }

    @Override
    public void process(Frame message) {
        StompMessageProcessor<Frame> process = comMap.getOrDefault(message.getMessageType(), null);
        process.process(message);
    }

    @Override
    public boolean shouldTerminate() {
        return false;
    }

    protected class SubscribeMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            //TODO how is managing the topics subscribers
            // TODO register the user to the requested topic
            String recepitId = message.getHeader(Receipt.RECEIPT_ID_HEADER);
            if(recepitId == null){
                //TODO create an error message
            }
            connections.send(connectionId, new Receipt(recepitId));
        }
    }

    protected class UnSubscribeMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            //TODO same as subscribe
            String receiptId = message.getHeader(Receipt.RECEIPT_ID_HEADER);
            if(receiptId == null){
                //TODO create an error message
            }
            connections.send(connectionId, new Receipt(receiptId));
        }
    }

    protected class SendMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            //TODO maybe check for valid data in the body i.e
            // {user} has added the book {book name}
            String dest  = message.getHeader(SendFrame.DESTINATION_HEADER);
            String body = message.getBody();

            if(dest == null | body == null){
                //TODO create error message
            }

            Frame messageFrame =  new MessageFrame(body,
                    connections.getsubscriptionById(dest, connectionId),
                    messageId.getNewIdAsString(),
                    dest);
            connections.send(dest, messageFrame);

        }
    }

    protected class DisconnectMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            String receiptId  = message.getHeader(DisconnectFrame.RECEIPT_ID_HEADER);
            connections.disconnect(connectionId);

            connections.send(connectionId, new Receipt(receiptId));
        }
    }

    private void InitMap() {
        //TODO check if there is a better way to represent the Stomp title
        comMap = new HashMap<>();
        comMap.put("SUBSCRIBE", new SubscribeMessageProcessor());
        comMap.put("UNSUBSCRIBE", new UnSubscribeMessageProcessor());
        comMap.put("SEND", new SendMessageProcessor());
        comMap.put("DISCONNECT", new DisconnectMessageProcessor());
    }
}
