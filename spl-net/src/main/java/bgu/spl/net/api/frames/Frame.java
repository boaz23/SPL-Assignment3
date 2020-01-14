package bgu.spl.net.api.frames;

import java.util.HashMap;
import java.util.Map;

public class Frame {
    public static final String HEADER_RECEIPT = "receipt";

    private String messageType;
    private Map<String, String> headers;
    private String body;

    private Frame() {
        this(null, null);
    }
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

    public String getClientReceiptId() {
        return getHeader(HEADER_RECEIPT);
    }

    public static class Builder {
        private Frame frame;

        public Builder() {
            frame = new Frame();
        }

        public Builder setMessageType(String messageType) {
            frame.messageType = messageType;
            return this;
        }

        public String getMessageType() {
            return frame.getMessageType();
        }

        public Builder putHeader(String header, String value) {
            frame.headers.put(header, value);
            return this;
        }

        public Builder removeHeader(String header) {
            frame.headers.remove(header);
            return this;
        }

        public String getHeader(String header) {
            return frame.getHeader(header);
        }

        public Builder setBody(String body) {
            frame.body = body;
            return this;
        }

        public String getBody() {
            return frame.getBody();
        }

        public Frame build() {
            return frame;
        }
    }
}
