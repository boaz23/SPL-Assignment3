package bgu.spl.net.srv.connections;

public class IncrementConnectionsIdManager implements ConnectionIdsManager {
    private int nextConnectionId;

    public IncrementConnectionsIdManager() {
        nextConnectionId = 1;
    }

    @Override
    public int nextConnectionId() {
        return nextConnectionId++;
    }
}
