package bgu.spl.net.srv;

import java.util.concurrent.atomic.AtomicInteger;

/**
 * Thread safe class to share a incremented id
 */
public class IdCount
{
    private AtomicInteger id;

    public IdCount(){
        id = new AtomicInteger(0);
    }

    public int getNewId(){
        return id.incrementAndGet();
    }

    public String getNewIdAsString(){
        int tId = getNewId();
        return String.format("%d",tId);
    }
}
