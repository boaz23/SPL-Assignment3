package bgu.spl.net.api;

import bgu.spl.net.srv.connections.Connections;

public interface MessagingProtocol<T> {
    /**
     * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
     **/
    void start(int connectionId, Connections<T> connections);

    /**
     * process the given message 
     * @param msg the received message
     */
    void process(T msg);
 
    /**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();
 
}