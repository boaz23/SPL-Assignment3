package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.StompMessageProcessor;
import bgu.spl.net.api.frames.*;
import bgu.spl.net.srv.connections.ConnectionInfo;
import bgu.spl.net.srv.connections.Connections;
import bgu.spl.net.srv.IdCount;

import java.util.HashMap;
import java.util.Map;

public class StompMessagingProtocolImpl implements StompMessagingProtocol {
    // TODO: add mapping for subscription-id to topic name

    private static IdCount messageId = new IdCount();
    private static final String version = "1.2";

    private int connectionId;
    private StompConnections connections;
    private Map<String, StompMessageProcessor<Frame>> comMap;
    private Map<Integer, String> subscriptionMap;
    private boolean shouldTerminate;

    public StompMessagingProtocolImpl(){
        subscriptionMap = new HashMap<>();
        InitMapNotConnected();
        shouldTerminate = false;
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
        return shouldTerminate;
    }

    protected class ConnectMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            String acceptVersion = message.getHeader(Connect.ACCEPTVERSION_HEADER);
            String userName = message.getHeader(Connect.LOGIN_HEADER);
            String passcode = message.getHeader(Connect.PASSCODE_HEADER);
            String host = message.getHeader(Connect.HOST_HEADER);

            if(acceptVersion == null | userName == null | passcode == null | host == null){
                errorMessage(message,"malformed frame received", "");
                return;
            }
            if(!acceptVersion.equals(version)){
                errorMessage(message,"version of stomp is not supported", "");
                return;
            }

            String passwordByUserName = connections.getUserPassword(userName);
            //TODO add a method that check if the user exist, instead of trying to get the password.
            if(passwordByUserName == null){
                connections.addUser(userName, passcode);
                InitMap();
                return;
            }
            if(!passcode.equals(passwordByUserName)){
                errorMessage(message,"Check your password", "");
                return;
            }
        }
    }

    protected class SubscribeMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            //TODO in the assignment file they wrote that the id header is not
            // mandatory but when unsubscribe it is, so set the id header as required

            int id = -1;
            String dest = message.getHeader(SubscribeFrame.DESTINATION_HEADER);
            String subscriptionId = message.getHeader(SubscribeFrame.ID_HEADER);
            if(dest == null){
                errorMessage(message, "malformed frame received",
                        "Didnt contain dest header");
                return;
            }

            id = validateId(subscriptionId, message);
            if(id < 0){
                return;
            }

            String tmpTopic = subscriptionMap.getOrDefault(id, null);

            if(tmpTopic == null){
                subscriptionMap.put(id, dest);
            }
            else if(tmpTopic.equals(dest)){
                //TODO may send error
            } else{
                errorMessage(message, "Already used id", "");
                return;
            }

            SubscriptionAttachment subscriptionAttachment = new SubscriptionAttachment(subscriptionId);
            if(!connections.subscribe(dest ,connectionId, subscriptionAttachment)){
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

            int id = -1;
            String subscriptionId = message.getHeader(UnsubscribeFrame.ID_HEADER);
            id = validateId(subscriptionId, message);
            if(id < 0){
                return;
            }

            String topic = subscriptionMap.remove(id);
            if(topic == null){
                //TODO throw error if unsubscribe with id not in the map
            }



            connections.unsubscribe(connectionId, topic);

            String receiptId = message.getHeader(Receipt.RECEIPT_ID_HEADER);
            if(receiptId != null) {
                connections.send(connectionId, new Receipt(receiptId));
            }
        }
    }

    protected class SendMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            String topic  = message.getHeader(SendFrame.DESTINATION_HEADER);
            String body = message.getBody();

            if(topic == null){
                errorMessage(message, "malformed frame received",
                        "Didnt contain destination header");
                return;
            }

            Iterable<ConnectionInfo<Frame>> connectionInfosOfTopic = connections.getConnectionsSubscribedTo(topic);

            for (ConnectionInfo<Frame> conn : connectionInfosOfTopic) {
                String id = ((SubscriptionAttachment)conn.getAttachment()).getSubscriptionId();
                Frame messageFrame =  new MessageFrame(body,
                        id,
                        messageId.getNewIdAsString(),
                        topic);
                //TODO check if to get the connection id from the attachment
                // or use the send(topic, message) in conncetions.
                connections.send(topic, messageFrame);
            }
        }
    }

    protected class DisconnectMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            String receiptId  = message.getHeader(Receipt.RECEIPT_ID_HEADER);
            shouldTerminate = true;

            if(receiptId == null){
                //TODO send error message, or not sending anything at all
            } else {
                connections.send(connectionId, new Receipt(receiptId));
            }
        }
    }

    protected class UserNotConnectedProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            errorMessage(message, "Not valid request, user not connected",
                    "User is not connected");
        }
    }

    protected class ConnectButUserConnectedProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            errorMessage(message, "Not valid request, user already connected",
                    "User is connected");
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
        shouldTerminate = true;
    }

    private int validateId(String subscriptionId, Frame message){
        int id = -1;

        if(subscriptionId == null) {
            errorMessage(message, "malformed frame received",
                    "Didnt contain id header");
            return id;
        }

        try {
            id = Integer.parseInt(subscriptionId);
        } catch (NumberFormatException e){
            errorMessage(message,"malformed frame received",
                    "Id is not a number");
        }

        return  id;
    }

    private void InitMapNotConnected() {
        comMap = new HashMap<>();
        comMap.put("CONNECT", new ConnectMessageProcessor());
        comMap.put("SUBSCRIBE", new UserNotConnectedProcessor());
        comMap.put("UNSUBSCRIBE", new UserNotConnectedProcessor());
        comMap.put("SEND", new UserNotConnectedProcessor());
        comMap.put("DISCONNECT", new UserNotConnectedProcessor());
    }

    private void InitMap() {
        //TODO check if there is a better way to represent the Stomp title
        comMap = new HashMap<>();
        comMap.put("CONNECT", new ConnectButUserConnectedProcessor());
        comMap.put("SUBSCRIBE", new SubscribeMessageProcessor());
        comMap.put("UNSUBSCRIBE", new UnsubscribeMessageProcessor());
        comMap.put("SEND", new SendMessageProcessor());
        comMap.put("DISCONNECT", new DisconnectMessageProcessor());
    }
}
