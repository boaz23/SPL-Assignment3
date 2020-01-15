package bgu.spl.net.srv.connections;

/**
 * An object for specifying a policy for getting fresh connection ids for incoming accepted connections
 */
public interface ConnectionIdsManager {
    int nextConnectionId();
}
