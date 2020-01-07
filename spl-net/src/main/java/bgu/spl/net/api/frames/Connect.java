package bgu.spl.net.api.frames;

import java.util.Map;

public class Connect extends Frame {

    public static final String ACCEPTVERSION_HEADER = "accept-version";
    public static final String HOST_HEADER = "host";
    public static final String LOGIN_HEADER = "login";
    public static final String PASSCODE_HEADER = "passcode";

    public Connect(String acceptVersion, String host, String userName, String passcode) {
        super("CONNECT", "");
        setAcceptVersion(acceptVersion);
        setHost(host);
        setLogin(userName);
        setPasscode(passcode);
    }

    public String getAcceptVersion() {
        return getHeader(ACCEPTVERSION_HEADER);
    }

    public void setAcceptVersion(String acceptVersion) {
        setHeader(ACCEPTVERSION_HEADER, acceptVersion);
    }
    public String getHost() {
        return getHeader(HOST_HEADER);
    }

    public void setHost(String host) {
        setHeader(HOST_HEADER, host);
    }
    public String getLogin() {
        return getHeader(LOGIN_HEADER);
    }

    public void setLogin(String login) {
        setHeader(LOGIN_HEADER, login);
    }
    public String getPasscode() {
        return getHeader(PASSCODE_HEADER);
    }

    public void setPasscode(String passcode) {
        setHeader(PASSCODE_HEADER, passcode);
    }
}
