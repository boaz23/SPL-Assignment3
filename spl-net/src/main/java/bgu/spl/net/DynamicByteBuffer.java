package bgu.spl.net;

import java.util.Arrays;

public class DynamicByteBuffer {
    public byte[] bytes;
    public int size;

    public DynamicByteBuffer(int initialCapacity) {
        bytes = new byte[initialCapacity];
        size = 0;
    }

    public void pushByte(byte b) {
        if (size == bytes.length) {
            bytes = Arrays.copyOf(bytes, size * 2);
        }

        bytes[size++] = b;
    }

    public void clear() {
        size = 0;
    }
}
