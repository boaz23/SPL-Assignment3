package bgu.spl.net;

import java.nio.charset.Charset;

/**
 * A decorator for {@link DynamicByteBuffer} that adds methods for reading
 * values other than raw bytes
 */
public class DynamicByteBufferReader {
    private final DynamicByteBuffer buffer;
    private final Charset encoding;

    /**
     * Initializes a new instance with the given buffer to read from and with
     * the specified encoding
     * @param buffer The buffer to read from
     * @param encoding The encoding used to read strings from the buffer
     */
    public DynamicByteBufferReader(DynamicByteBuffer buffer, Charset encoding) {
        this.buffer = buffer;
        this.encoding = encoding;
    }

    /**
     * Reads a string from the beginning of buffer to it's end and then clears it
     * @return The read string
     */
    public String popString() {
        String result = new String(buffer.bytes, 0, buffer.size, encoding);
        buffer.clear();
        return result;
    }
}
