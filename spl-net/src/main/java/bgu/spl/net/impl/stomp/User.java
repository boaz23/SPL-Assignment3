package bgu.spl.net.impl.stomp;

/**
 * Class that handles the user data
 */
class User {
    private String username;
    private String password;
    private boolean isConnected;

    User(String username, String password) {
        this.username = username;
        this.password = password;
        isConnected = true;
    }

    /**
     * @return the username
     */
    public String username() {
        return username;
    }

    /**
     * @return the password of the user
     */
    public String password() {
        return password;
    }

    /**
     * @return the users connection state
     */
    public boolean isConnected() {
        return isConnected;
    }

    /**
     * Set the user connection state
     * @param connected
     */
    public void setConnected(boolean connected) {
        isConnected = connected;
    }
}
