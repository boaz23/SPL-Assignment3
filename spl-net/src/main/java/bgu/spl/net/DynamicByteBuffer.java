package bgu.spl.net;

import java.util.Arrays;

/**
 * Represents a byte array (buffer) that grows dynamically when needed.
 * It exposes the internal byte array for efficiency.
 */
public class DynamicByteBuffer {
    /**
     * The internal byte array that stores the bytes written to this buffer instance
     */
    public byte[] bytes;

    /**
     * The amount of bytes the buffer currently stores
     */
    public int size;

    /**
     * Initializes a new dynamic buffer instance with the given initial capacity
     * @param initialCapacity The initial capacity of the internal bytes array
     */
    public DynamicByteBuffer(int initialCapacity) {
        bytes = new byte[initialCapacity];
        size = 0;
    }

    /**
     * Adds a byte to the end of the buffer
     * @param b The byte to add
     */
    public void pushByte(byte b) {
        if (size == bytes.length) {
            bytes = Arrays.copyOf(bytes, size * 2);
        }

        bytes[size++] = b;
    }

    /**
     * Clears the buffer
     */
    public void clear() {
        size = 0;
    }
}
