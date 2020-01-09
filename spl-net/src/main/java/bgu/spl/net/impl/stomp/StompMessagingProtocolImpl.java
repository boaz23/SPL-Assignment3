package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessageProcessor;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.frames.*;
import bgu.spl.net.srv.IdCount;
import bgu.spl.net.srv.connections.ConnectionSubscriptionInfo;
import bgu.spl.net.srv.connections.Connections;

import java.util.HashMap;
import java.util.Map;

public class StompMessagingProtocolImpl implements StompMessagingProtocol {

    private static IdCount messageId = new IdCount();
    private static final String version = "1.2";

    private int connectionId;
    private StompConnections connections;
    private Map<String, StompMessageProcessor<Frame>> comMap;
    private Map<String, String> subscriptionMap;
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

        if(process == null){
            errorMessage(message, "Unknown message","Unknown message");
            return;
        }
        process.process(message);

        if (!shouldTerminate){
            String receiptId = message.getHeader(Receipt.RECEIPT_ID_HEADER);
            if(receiptId != null) {
                connections.send(connectionId, new Receipt(receiptId));
            }
        }
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

            String dest = message.getHeader(SubscribeFrame.DESTINATION_HEADER);
            String subscriptionId = message.getHeader(SubscribeFrame.ID_HEADER);
            if(dest == null | subscriptionId == null){
                errorMessage(message, "malformed frame received",
                        "Didnt contain dest or id header");
                return;
            }

            if(dest.equals("")){
                errorMessage(message, "destination header is empty",
                        "The destination cant be empty");
                return;
            }

            String tmpTopic = subscriptionMap.getOrDefault(subscriptionId, null);

            if(tmpTopic == null){
                subscriptionMap.put(subscriptionId, dest);
            }
            else{
                errorMessage(message, "Already used id", "");
                return;
            }

            SubscriptionAttachment subscriptionAttachment = new SubscriptionAttachment(subscriptionId);
            connections.subscribe(dest, connectionId, subscriptionAttachment);
        }
    }

    protected class UnsubscribeMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {

            String subscriptionId = message.getHeader(UnsubscribeFrame.ID_HEADER);
            if(subscriptionId == null){
                errorMessage(message, "malformed frame received",
                        "Doesnt contain id header");
                return;
            }

            String topic = subscriptionMap.remove(subscriptionId);
            connections.unsubscribe(connectionId, topic);
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

            Iterable<ConnectionSubscriptionInfo<Frame>> connectionInfosOfTopic = connections.getConnectionsSubscribedTo(topic);

            for (ConnectionSubscriptionInfo<Frame> conn : connectionInfosOfTopic) {
                String id = ((SubscriptionAttachment)conn.getAttachment()).getSubscriptionId();
                Frame messageFrame =  new MessageFrame(body,
                        id,
                        messageId.getNewIdAsString(),
                        topic);

                connections.send(conn.getConnectionId(), messageFrame);
            }
        }
    }

    protected class DisconnectMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            shouldTerminate = true;

            String receiptId  = message.getHeader(Receipt.RECEIPT_ID_HEADER);
            if(receiptId != null) {
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
        String body = "The message: \n " + StompFrameEncoderDecoder.toString(message) + "\n" +
                addedBodyMessage;

        Frame errorMessage = new ErrorFrame(body , messageHeader);
        String receiptId = message.getHeader(Receipt.RECEIPT_ID_HEADER);
        if(receiptId != null){
            errorMessage.setHeader(Receipt.RECEIPT_ID_HEADER, receiptId);
        }
        connections.send(connectionId, errorMessage);
        shouldTerminate = true;
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
        comMap = new HashMap<>();
        comMap.put("CONNECT", new ConnectButUserConnectedProcessor());
        comMap.put("SUBSCRIBE", new SubscribeMessageProcessor());
        comMap.put("UNSUBSCRIBE", new UnsubscribeMessageProcessor());
        comMap.put("SEND", new SendMessageProcessor());
        comMap.put("DISCONNECT", new DisconnectMessageProcessor());
    }
}
