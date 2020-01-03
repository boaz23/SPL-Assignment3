package bgu.spl.net.srv;

import java.util.concurrent.atomic.AtomicInteger;

/**
 * Thread safe class to share a incremented id
 */
public class IdCount
{
    private AtomicInteger id;

    public IdCount(){
        id.set(0);
    }

    public int getNewId(){
        int tId = id.get();
        while(!id.compareAndSet(tId, tId+1)){
            tId = id.get();
        }

        return tId;
    }
}
