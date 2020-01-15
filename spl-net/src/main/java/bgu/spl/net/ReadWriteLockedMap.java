package bgu.spl.net;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.function.Function;

/**
 * A map that the access to it is synchronized using a read/write lock
 * @param <K> The type of the key
 * @param <V> The type of the value
 */
public class ReadWriteLockedMap<K, V> {
    private Map<K, V> map;
    private ReadWriteLock lock;

    /**
     * Initializes a new instance with a {@link HashMap} as the internal map and a {@link ReentrantReadWriteLock} as the internal lock
     */
    public ReadWriteLockedMap() {
        map = new HashMap<>();
        lock = new ReentrantReadWriteLock();
    }

    /**
     * Inserts/updates the specified value as the value for the specified key
     * @param key The key
     * @param value The value
     */
    public void put(K key, V value) {
        lock.writeLock().lock();
        try {
            map.put(key, value);
        }
        finally {
            lock.writeLock().unlock();
        }
    }

    /**
     * @param key The key
     * @return The value associated with the key or {@code null} if no value is.
     */
    public V get(K key) {
        lock.readLock().lock();
        try {
            return map.get(key);
        }
        finally {
            lock.readLock().unlock();
        }
    }

    /**
     * If a value is associated for the specified key, then it returns it.
     * Otherwise, it inserts a the value returned by the mapping function
     * as the value for the specified key and returns it.
     * @param key The key
     * @param mappingFunction The function that creates a value if the specified
     *                        key is not associated with a value
     * @return The inserted/existing associated value of the specified key
     */
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

    /**
     * Gets the associated value for the specified key without acquiring the read lock
     * @param key The key
     * @return The value associated with the specified key
     */
    public V getWithoutLock(K key) {
        return map.get(key);
    }

    /**
     * @return The read/write lock used internally by this instance
     */
    public ReadWriteLock internalLock() {
        return lock;
    }
}
