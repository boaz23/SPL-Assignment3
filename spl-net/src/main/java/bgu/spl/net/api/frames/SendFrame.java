package bgu.spl.net.api.frames;

import java.util.Map;

public class SendFrame extends Frame {

    public static final String DESTINATION_HEADER = "destination";

    public SendFrame(String destination, String body) {
        super("SEND", body);
    }

    public String getDestination() {
        return getHeader(DESTINATION_HEADER);
    }

    public void setDestination(String destination) {
        setHeader(DESTINATION_HEADER, destination);
    }
}
