package bgu.spl.net.api.frames;

public class SubscribeFrame extends Frame {

    public static final String DESTINATION_HEADER = "destination";
    public static final String ID_HEADER = "id";

    public SubscribeFrame(String destination, String body) {
        super("SUBSCRIBE", body);
    }

    public String getDestination() {
        return getHeader(DESTINATION_HEADER);
    }

    public void setDestination(String destination) {
        setHeader(DESTINATION_HEADER, destination);
    }

    public String getId() {
        return getHeader(ID_HEADER);
    }

    public void setId(String id) {
        setHeader(ID_HEADER, id);
    }
}
