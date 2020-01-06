package bgu.spl.net.impl.stomp;

import bgu.spl.net.DynamicByteBuffer;
import bgu.spl.net.DynamicByteBufferReader;
import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.frames.Frame;

import java.nio.charset.Charset;
import java.util.Map;

public class StompFrameEncoderDecoder implements StompMessageEncoderDecoder {
    private static final char EOL_ENCODE = '\n';
    private static final char EOL_OPTIONAL = '\r';
    private static final int BUFFER_START_CAPACITY = 1 << 10;

    private final Charset encoding;
    private final DynamicByteBuffer buffer;
    private final DynamicByteBufferReader bufferReader;
    private Frame.Builder frameBuilder;
    private DecodeState decodeState;

    public StompFrameEncoderDecoder(Charset encoding) {
        this.encoding = encoding;
        buffer = new DynamicByteBuffer(BUFFER_START_CAPACITY);
        bufferReader = new DynamicByteBufferReader(buffer, encoding);
        reset();
    }

    public static String toString(Frame frame) {
        StringBuilder frameSb = new StringBuilder();
        appendMessageType(frame, frameSb);
        appendHeaders(frame, frameSb);
        appendBody(frame, frameSb);
        return frameSb.toString();
    }

    private static void appendHeader(StringBuilder frameSb, Map.Entry<String, String> header) {
        frameSb.append(header.getKey())
            .append(':')
            .append(header.getValue())
            .append(EOL_ENCODE);
    }

    @Override
    public Frame decodeNextByte(byte nextByte) {
        return decodeState.decodeNextByte(nextByte);
    }

    @Override
    public byte[] encode(Frame message) {
        return toString(message).getBytes(encoding);
    }

    private static void appendBody(Frame frame, StringBuilder frameSb) {
        frameSb.append(EOL_ENCODE)
            .append(frame.getBody())
            .append('\0');
    }

    private static void appendMessageType(Frame frame, StringBuilder frameSb) {
        frameSb.append(frame.getMessageType())
            .append(EOL_ENCODE);
    }

    private static void appendHeaders(Frame frame, StringBuilder frameSb) {
        for (Map.Entry<String, String> header : frame.headers()) {
            appendHeader(frameSb, header);
        }
    }

    private void reset() {
        frameBuilder = new Frame.Builder();
        decodeState = new MessageTypeDecodeState();
    }

    private interface DecodeState {
        Frame decodeNextByte(byte nextByte);
    }

    private class MessageTypeDecodeState implements DecodeState {
        private boolean carriageReturn;

        MessageTypeDecodeState() {
            carriageReturn = false;
        }

        @Override
        public Frame decodeNextByte(byte nextByte) {
            if (nextByte == EOL_ENCODE) {
                String msgType = bufferReader.popString();
                frameBuilder.setMessageType(msgType);
                decodeState = new HeadersDecodeState();
                carriageReturn = false;
            }
            else {
                if (carriageReturn) {
                    // We skipped it, therefore we're adding it
                    buffer.pushByte((byte)EOL_OPTIONAL);
                    carriageReturn = false;
                }

                if (nextByte == EOL_OPTIONAL) {
                    // it's optional, we need to skip
                    carriageReturn = true;
                }
                else {
                    buffer.pushByte(nextByte);
                }
            }

            return null;
        }
    }

    private class HeadersDecodeState implements DecodeState {
        private static final char HEADER_VALUE_SEPARATOR = ':';

        private boolean newLineFeed;
        private boolean carriageReturn;
        private String headerName;

        HeadersDecodeState() {
            newLineFeed = false;
            carriageReturn = false;
            headerName = null;
        }

        @Override
        public Frame decodeNextByte(byte nextByte) {
            if (nextByte == EOL_ENCODE) {
                if (newLineFeed) {
                    decodeState = new BodyDecodeState();
                }
                else {
                    if (headerName == null) {
                        // TODO: error
                    }

                    String value = bufferReader.popString();
                    if (value.length() == 0) {
                        // TODO: error
                    }

                    frameBuilder.putHeader(headerName, value);
                    reset();
                }

                newLineFeed = true;
                carriageReturn = false;
            }
            else {
                if (carriageReturn) {
                    // We skipped it, therefore we're adding it
                    buffer.pushByte((byte)EOL_OPTIONAL);
                    carriageReturn = false;
                }

                if (nextByte == EOL_OPTIONAL) {
                    // it's optional, we need to skip
                    carriageReturn = true;
                }
                if (nextByte == HEADER_VALUE_SEPARATOR && headerName == null) {
                    headerName = bufferReader.popString();
                    if (headerName.length() == 0) {
                        // TODO: error
                    }
                }
                else {
                    buffer.pushByte(nextByte);
                }

                newLineFeed = false;
            }

            return null;
        }

        private void reset() {
            headerName = null;
        }
    }

    private class BodyDecodeState implements DecodeState {
        private static final char NULL_TERMINATION = '\0';

        @Override
        public Frame decodeNextByte(byte nextByte) {
            if (nextByte == NULL_TERMINATION) {
                String body = bufferReader.popString();
                frameBuilder.setBody(body);
                Frame frame = frameBuilder.build();
                StompFrameEncoderDecoder.this.reset();
                return frame;
            }

            buffer.pushByte(nextByte);
            return null;
        }
    }
}
