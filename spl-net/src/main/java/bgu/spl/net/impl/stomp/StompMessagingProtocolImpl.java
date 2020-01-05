package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.StompMessageProcessor;
import bgu.spl.net.api.frames.*;
import bgu.spl.net.srv.connections.Connections;
import bgu.spl.net.srv.IdCount;

import java.util.HashMap;
import java.util.Map;

public class StompMessagingProtocolImpl implements StompMessagingProtocol {
    // TODO: add mapping for subscription-id to topic name

    private static IdCount messageId = new IdCount();

    private int connectionId;
    private StompConnections connections;
    private Map<String, StompMessageProcessor<Frame>> comMap;

    public StompMessagingProtocolImpl(){
        InitMap();
    }

    @Override
    public void start(int connectionId, Connections<Frame> connections) {
        this.connectionId = connectionId;
        this.connections = (StompConnections)connections;
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
            //TODO in the assignment file they wrote that the id header is not
            // mandatory but when unsubscribe it is, so set the id header as required

            String dest = message.getHeader(SubscribeFrame.DESTINATION_HEADER);
            String subscriptionId = message.getHeader(SubscribeFrame.ID_HEADER);
            if(dest == null | subscriptionId == null){
                errorMessage(message, "malformed frame received",
                        "Didnt contain dest header or id header");
                return;
            }
            if(!connections.subscribe(dest ,String.format("%d",connectionId),
                    subscriptionId)){
                //TODO check if a subscription to channel return false
                errorMessage(message, "Already used id", "");
                return;
            }

            //TODO throw an error if receipt is not exsit or just not sending any message
            if(message.getHeader(Receipt.RECEIPT_ID_HEADER) != null) {
                connections.send(connectionId, new Receipt(message.getHeader(Receipt.RECEIPT_ID_HEADER) ));
            }
        }
    }

    protected class UnsubscribeMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {

            String id = message.getHeader(UnsubscribeFrame.ID_HEADER);
            if(id == null){
                errorMessage(message, "malformed frame received",
                        "Didnt contain id header");
                return;
            }

            connections.unsubscribe(String.format("%d",connectionId), id);

            String receiptId = message.getHeader(Receipt.RECEIPT_ID_HEADER);
            if(receiptId != null) {
                connections.send(connectionId, new Receipt(receiptId));
            }
        }
    }

    protected class SendMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            //TODO maybe check for valid data in the body i.e
            // {user} has added the book {book name}
            String dest  = message.getHeader(SendFrame.DESTINATION_HEADER);
            String body = message.getBody();

            if(dest == null){
                errorMessage(message, "malformed frame received",
                        "Didnt contain destination header");
                return;
            }

            //TODO how to send to each client its subscription id
            Frame messageFrame =  new MessageFrame(body,
                    connections.getSubscriptionById(connectionId, dest),
                    messageId.getNewIdAsString(),
                    dest);
            connections.send(dest, messageFrame);

        }
    }

    protected class DisconnectMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            String receiptId  = message.getHeader(Receipt.RECEIPT_ID_HEADER);
            connections.disconnect(connectionId);

            if(receiptId == null){
                //TODO send error message, or not sending anything at all
            } else {
                connections.send(connectionId, new Receipt(receiptId));
            }
        }
    }

    private void errorMessage(Frame message, String messageHeader, String addedBodyMessage){
        String body = "The message: \n " + message.toString() + "\n" +
                addedBodyMessage;

        Frame errorMessage = new ErrorFrame(body , messageHeader);
        String receiptId = message.getHeader(Receipt.RECEIPT_ID_HEADER);
        if(receiptId != null){
            errorMessage.setHeader(Receipt.RECEIPT_ID_HEADER, receiptId);
        }
        connections.send(connectionId, errorMessage);
        connections.disconnect(connectionId);
    }

    private void InitMap() {
        //TODO check if there is a better way to represent the Stomp title
        comMap = new HashMap<>();
        comMap.put("SUBSCRIBE", new SubscribeMessageProcessor());
        comMap.put("UNSUBSCRIBE", new UnsubscribeMessageProcessor());
        comMap.put("SEND", new SendMessageProcessor());
        comMap.put("DISCONNECT", new DisconnectMessageProcessor());
    }
}
