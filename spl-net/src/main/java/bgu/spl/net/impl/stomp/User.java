package bgu.spl.net.impl.stomp;

class User {
    private String username;
    private String password;
    private boolean isConnected;

    User(String username, String password) {
        this.username = username;
        this.password = password;
        isConnected = true;
    }

    public String username() {
        return username;
    }

    public String password() {
        return password;
    }

    public boolean isConnected() {
        return isConnected;
    }

    public void setConnected(boolean connected) {
        isConnected = connected;
    }
}
