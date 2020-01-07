package bgu.spl.net.api.frames;

import java.util.HashMap;

public class ConnectedFrame extends Frame {
    private static final String VERSION_HEADER = "version";

    public ConnectedFrame(String version) {
        super("CONNECTED", "");
        setVersion(version);
    }

    public String getVersion() {
        return getHeader(VERSION_HEADER);
    }

    public void setVersion(String version) {
        setHeader(VERSION_HEADER, version);
    }
}
