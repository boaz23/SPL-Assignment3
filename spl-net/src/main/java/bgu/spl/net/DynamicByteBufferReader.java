package bgu.spl.net;

import java.nio.charset.Charset;

public class DynamicByteBufferReader {
    private final DynamicByteBuffer buffer;
    private final Charset encoding;

    public DynamicByteBufferReader(DynamicByteBuffer buffer, Charset encoding) {
        this.buffer = buffer;
        this.encoding = encoding;
    }

    public String popString() {
        String result = new String(buffer.bytes, 0, buffer.size, encoding);
        buffer.clear();
        return result;
    }
}
