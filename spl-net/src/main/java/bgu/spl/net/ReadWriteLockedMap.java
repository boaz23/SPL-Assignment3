package bgu.spl.net;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.function.Function;

public class ReadWriteLockedMap<K, V> {
    private Map<K, V> map;
    private ReadWriteLock lock;

    public ReadWriteLockedMap() {
        map = new HashMap<>();
        lock = new ReentrantReadWriteLock();
    }

    public void put(K key, V value) {
        lock.writeLock().lock();
        try {
            map.put(key, value);
        }
        finally {
            lock.writeLock().unlock();
        }
    }

    public V get(K key) {
        lock.readLock().lock();
        try {
            return map.get(key);
        }
        finally {
            lock.readLock().unlock();
        }
    }

    public V computeIfAbsent(K key, Function<? super K, ? extends V> mappingFunction) {
        V value = get(key);
        if (value == null) {
            lock.writeLock().lock();
            try {
                if (map.get(key) == null) {
                    value = mappingFunction.apply(key);
                    map.put(key, value);
                }
                return value;
            }
            finally {
                lock.writeLock().unlock();
            }
        }

        return value;
    }

    public V getWithoutLock(K key) {
        return map.get(key);
    }

    public ReadWriteLock internalLock() {
        return lock;
    }
}
