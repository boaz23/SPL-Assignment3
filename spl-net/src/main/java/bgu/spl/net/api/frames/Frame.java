package bgu.spl.net.api.frames;

import java.util.HashMap;
import java.util.Map;

public class Frame {
    private String messageType;
    private Map<String, String> headers;
    private String body;

    public Frame(String messageType, String body) {
        this(messageType, new HashMap<>(), body);
    }
    public Frame(String messageType, Map<String, String> headers, String body) {
        this.messageType = messageType;
        this.headers = headers;
        this.body = body;
    }

    public String getMessageType() {
        return messageType;
    }

    public String getHeader(String header) {
        return headers.get(header);
    }

    public void setHeader(String header, String value) {
        headers.put(header, value);
    }

    public boolean hasHeader(String header) {
        return headers.containsKey(header);
    }

    public void setMessageType(String messageType) {
        this.messageType = messageType;
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    public Iterable<Map.Entry<String, String>> headers() {
        return headers.entrySet();
    }

    public String toString(){
        //TODO implement
        return null;
    }
}
