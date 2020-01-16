package bgu.spl.net.srv.connections;

/**
 * A connections id generator policy that generates IDs from a counter that start
 * with 1
 */
public class IncrementConnectionsIdManager implements ConnectionIdsManager {
    private int nextConnectionId;

    /**
     * Initializes a new instance
     */
    public IncrementConnectionsIdManager() {
        nextConnectionId = 1;
    }

    @Override
    public int nextConnectionId() {
        return nextConnectionId++;
    }
}
