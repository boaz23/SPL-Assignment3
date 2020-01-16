package bgu.spl.net.impl.stomp;

import bgu.spl.net.StringBufferLogger;
import bgu.spl.net.api.StompMessageProcessor;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.frames.*;
import bgu.spl.net.srv.IdCount;
import bgu.spl.net.srv.connections.Subscription;
import bgu.spl.net.srv.connections.Connections;

import java.util.HashMap;
import java.util.Map;

public class StompMessagingProtocolImpl implements StompMessagingProtocol {

    private static IdCount messageId = new IdCount();
    private static final String version = "1.2";

    private int connectionId;
    private StompConnections connections;
    private Map<String, StompMessageProcessor<Frame>> comMap;
    private boolean shouldTerminate;

    private final Map<String, StompMessageProcessor<Frame>> NotConnectedProcessorMap;
    private final Map<String, StompMessageProcessor<Frame>> ConnectedProcessorMap;

    /**
     * Constructor
     */
    public StompMessagingProtocolImpl(){
        shouldTerminate = false;

        NotConnectedProcessorMap = InitMapNotConnected();
        ConnectedProcessorMap = InitMapConnected();
        comMap = NotConnectedProcessorMap;
    }

    /**
     * Initialize the connection id and the Connections in instance
     * @param connectionId connection id
     * @param connections Connections<Frame> instance
     */
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
            sendReceipt(message);
        }
    }

    private void sendReceipt(Frame message) {
        String receiptId = message.getHeader(Frame.HEADER_RECEIPT);
        if (receiptId != null) {
            connections.send(connectionId, new Receipt(receiptId));
        }
    }

    /**
     * Signal if the run should stop
     * @return true if the thread should stop
     */
    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    /**
     * StompMessageProcessor implementation that handles the CONNECT Frame
     */
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

            boolean connectSuccessful = false;
            String errorMsg = null;
            User user = connections.getUser(userName);
            if (user == null) {
                connectSuccessful = true;
                connections.addUser(connectionId, userName, passcode);
            }
            else {
                synchronized (user) {
                    if (user.isConnected()) {
                        errorMsg = "User already logged in";
                    } else if (!passcode.equals(user.password())) {
                        errorMsg = "Wrong password";
                    } else {
                        connectSuccessful = true;
                        connections.connectUser(userName, connectionId);
                    }
                }
            }

            if (connectSuccessful) {
                connections.send(connectionId, new ConnectedFrame(version));
                switchToConnectedProcessorState();
            }
            else {
                errorMessage(message, errorMsg, "");
            }
        }
    }

    /**
     * StompMessageProcessor implementation that handles the SUBSCRIBE Frame
     */
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

            if (dest.equals("")) {
                errorMessage(message, "destination header is empty",
                        "The destination cant be empty");
                return;
            }
            boolean error = false;
            User user = connections.getClient(connectionId).user();
            synchronized (user) {
                SubscriptionAttachment attachment = connections.getSubscriptionAttachment(connectionId, dest);
                if (attachment == null) {
                    attachment = new SubscriptionAttachment(subscriptionId);
                    if (connections.isSubscriptionAttachmentUsed(connectionId, attachment)) {
                        error = true;
                    }
                    else {
                        connections.subscribe(dest, connectionId, attachment);
                    }
                }
            }
            if (error) {
                errorMessage(message, "Subscription id already used", "");
            }
        }
    }


    /**
     * StompMessageProcessor implementation that handles the UNSUBSCRIBE Frame
     */
    protected class UnsubscribeMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            String subscriptionId = message.getHeader(UnsubscribeFrame.ID_HEADER);
            if(subscriptionId == null){
                errorMessage(message, "malformed frame received",
                        "Doesnt contain id header");
                return;
            }

            SubscriptionAttachment attachment = new SubscriptionAttachment(subscriptionId);
            User user = connections.getClient(connectionId).user();
            synchronized (user) {
                if (connections.isSubscriptionAttachmentUsed(connectionId, attachment)) {
                    connections.unsubscribe(connectionId, new SubscriptionAttachment(subscriptionId));
                }
            }
        }
    }

    /**
     * StompMessageProcessor implementation that handles the SEND Frame
     */
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

            Iterable<Subscription<Frame>> connectionInfosOfTopic = connections.getConnectionsSubscribedTo(topic);
            for (Subscription<Frame> conn : connectionInfosOfTopic) {
                int connectionId = conn.getConnectionId();
                User user = connections.getUser(connectionId);
                if (user == null || !user.isConnected()) {
                    if (shouldTerminate()) {
                        StringBufferLogger.ReactorLogger.appendLine("sending message after termination to offline user" + ((user != null) ? user.username() : ""));
                    }
                    else {
                        StringBufferLogger.ReactorLogger.appendLine("sending message to offline user" + ((user != null) ? user.username() : ""));
                    }
                    continue;
                }
                if (shouldTerminate()) {
                    StringBufferLogger.ReactorLogger.appendLine("sending message after termination to online user" + user.username());
                }
                else {
                    StringBufferLogger.ReactorLogger.appendLine("sending message to online user" + user.username());
                }

                SubscriptionAttachment attachment = (SubscriptionAttachment)conn.getAttachment();
                Frame messageFrame =  new MessageFrame(body,
                        attachment.getSubscriptionId(),
                        messageId.getNewIdAsString(),
                        topic);
                connections.send(conn.getConnectionId(), messageFrame);
            }
        }
    }


    /**
     * StompMessageProcessor implementation that handles the DISCONNECT Frame
     */
    protected class DisconnectMessageProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            StringBufferLogger.ReactorLogger.appendLine("processing DISCONNECT...");
            StompClient client = connections.getClient(connectionId);
            if (client != null) {
                User user = client.user();
                if (user != null) {
                    StringBufferLogger.ReactorLogger.appendLine("logging out user");
                    synchronized (user) {
                        connections.logoutUser(connectionId);
                    }
                }
            }

            shouldTerminate = true;
            sendReceipt(message);
            connections.disconnect(connectionId);
            StringBufferLogger.ReactorLogger.appendLine("DISCONNECT processing finished");
        }
    }


    /**
     * StompMessageProcessor implementation that handles frame when the user is not connected
     */
    protected class UserNotConnectedProcessor implements StompMessageProcessor<Frame>{
        @Override
        public void process(Frame message) {
            errorMessage(message, "Not valid request, user not connected",
                    "User is not connected");
        }
    }

    /**
     * When the connection already sent a login that was successful
     */
    protected class AlreadyConnetedOnConnectionProcessor implements StompMessageProcessor<Frame> {
        @Override
        public void process(Frame message) {
            errorMessage(message, "Already connected with this connection", "");
        }
    }

    private void errorMessage(Frame message, String messageHeader, String addedBodyMessage){
        String body = "The message: \n " + StompFrameEncoderDecoder.toString(message) + "\n" +
                addedBodyMessage;

        Frame errorMessage = new ErrorFrame(messageHeader, body);
        String receiptId = message.getClientReceiptId();
        if(receiptId != null){
            errorMessage.setHeader(Receipt.RECEIPT_ID_HEADER, receiptId);
        }
        connections.send(connectionId, errorMessage);
        shouldTerminate = true;
    }

    private void switchToConnectedProcessorState() {
        comMap =  ConnectedProcessorMap;
    }

    private Map<String, StompMessageProcessor<Frame>> InitMapNotConnected() {
        Map<String, StompMessageProcessor<Frame>> comMap = new HashMap<>();
        comMap.put("CONNECT", new ConnectMessageProcessor());
        comMap.put("STOMP", comMap.get("CONNECT"));
        comMap.put("SUBSCRIBE", new UserNotConnectedProcessor());
        comMap.put("UNSUBSCRIBE", new UserNotConnectedProcessor());
        comMap.put("SEND", new UserNotConnectedProcessor());
        comMap.put("DISCONNECT", new UserNotConnectedProcessor());
        return comMap;
    }

    private Map<String, StompMessageProcessor<Frame>> InitMapConnected() {
        Map<String, StompMessageProcessor<Frame>> comMap = new HashMap<>();
        comMap.put("CONNECT", new AlreadyConnetedOnConnectionProcessor());
        comMap.put("STOMP", comMap.get("CONNECT"));
        comMap.put("SUBSCRIBE", new SubscribeMessageProcessor());
        comMap.put("UNSUBSCRIBE", new UnsubscribeMessageProcessor());
        comMap.put("SEND", new SendMessageProcessor());
        comMap.put("DISCONNECT", new DisconnectMessageProcessor());
        return comMap;
    }
}
