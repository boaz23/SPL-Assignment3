package bgu.spl.net.api.frames;

public class ErrorFrame extends Frame {

    public static final String MESSAGE_HEADER = "message";

    public ErrorFrame(String message, String body) {
        super("ERROR", body);
        setMessage(message);
    }

    public String getMessage() {
        return getHeader(MESSAGE_HEADER);
    }

    public void setMessage(String message) {
        setHeader(MESSAGE_HEADER, message);
    }
}
