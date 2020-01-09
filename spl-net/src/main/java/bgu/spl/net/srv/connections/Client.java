package bgu.spl.net.srv.connections;

public class Client<T> {
    private ConnectionHandler<T> connection;

    public Client(ConnectionHandler<T> connection) {
        this.connection = connection;
    }

    public ConnectionHandler<T> connection() {
        return connection;
    }
}
