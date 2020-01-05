package bgu.spl.net.api.frames;

public class UnsubscribeFrame extends Frame {

    public static final String ID_HEADER = "id";

    public UnsubscribeFrame(String destination, String body) {
        super("UNSUBSCRIBE", body);
    }

    public String getId() {
        return getHeader(ID_HEADER);
    }

    public void setId(String id) {
        setHeader(ID_HEADER, id);
    }
}
