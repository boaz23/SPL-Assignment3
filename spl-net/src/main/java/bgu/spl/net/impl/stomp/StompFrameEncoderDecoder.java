package bgu.spl.net.impl.stomp;

import bgu.spl.net.DynamicByteBuffer;
import bgu.spl.net.DynamicByteBufferReader;
import bgu.spl.net.Logger;
import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.connections.ConnectionHandlersManager;

import java.nio.charset.Charset;
import java.util.Map;

public class StompFrameEncoderDecoder implements StompMessageEncoderDecoder {
    private static final char EOL_ENCODE = '\n';
    private static final char EOL_OPTIONAL = '\r';
    private static final int BUFFER_START_CAPACITY = 1 << 10;
    private static final char NULL_TERMINATION = '\0';

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

    @Override
    public Frame decodeNextByte(byte nextByte) {
        try {
            return decodeState.decodeNextByte(nextByte);
        }
        catch (RuntimeException e) {
            buffer.clear();
            reset();
            throw e;
        }
    }

    @Override
    public byte[] encode(Frame message) {
        return toString(message).getBytes(encoding);
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

    private static void appendHeader(StringBuilder frameSb, Map.Entry<String, String> header) {
        frameSb.append(header.getKey())
            .append(':')
            .append(header.getValue())
            .append(EOL_ENCODE);
    }

    private static void appendBody(Frame frame, StringBuilder frameSb) {
        frameSb.append(EOL_ENCODE)
            .append(frame.getBody())
            .append('\0');
    }

    private void reset() {
        frameBuilder = new Frame.Builder();
        decodeState = new MessageTypeDecodeState();
    }

    private StompConnections connections;
    private int id;
    public <T> void setStuff(ConnectionHandlersManager<T> connectionHandlersManager, int nextId) {
        connections = (StompConnections)connectionHandlersManager;
        id = nextId;
    }

    private interface DecodeState {
        Frame decodeNextByte(byte nextByte);
    }

    private class MessageTypeDecodeState implements DecodeState {
        private boolean carriageReturn;
        private boolean first = true;

        MessageTypeDecodeState() {
            carriageReturn = false;
        }

        @Override
        public Frame decodeNextByte(byte nextByte) {
            if (first) {
                User user =connections.getUser(id);
                if (user != null) {
                    Logger.incoming.appendLine("username: " + user.username());
                }
                first = false;
            }
            if (nextByte == NULL_TERMINATION) {
                throw new RuntimeException("invalid input. should not get here because we assume input is valid.");
            }

            if (nextByte == EOL_ENCODE) {
                decodeEndOfLine();
                carriageReturn = false;
            }
            else {
                handleCarriedCarriageReturn();
                decodeByte(nextByte);
            }

            return null;
        }

        private void decodeEndOfLine() {
            setMessageType();
            nextDecodingState();
        }

        private void setMessageType() {
            String msgType = bufferReader.popString();
            Logger.incoming.appendLine(msgType);
            frameBuilder.setMessageType(msgType);
        }

        private void nextDecodingState() {
            decodeState = new HeadersDecodeState();
        }

        private void decodeByte(byte nextByte) {
            if (nextByte == EOL_OPTIONAL) {
                // it's optional, we need to skip
                carriageReturn = true;
            }
            else {
                buffer.pushByte(nextByte);
            }
        }

        private void handleCarriedCarriageReturn() {
            if (carriageReturn) {
                // We skipped it, therefore we're adding it
                buffer.pushByte((byte)EOL_OPTIONAL);
                carriageReturn = false;
            }
        }
    }

    private class HeadersDecodeState implements DecodeState {
        private static final char HEADER_VALUE_SEPARATOR = ':';

        private boolean newLineFeed;
        private boolean carriageReturn;
        private String headerName;

        HeadersDecodeState() {
            newLineFeed = true;
            carriageReturn = false;
            headerName = null;
        }

        @Override
        public Frame decodeNextByte(byte nextByte) {
            if (nextByte == NULL_TERMINATION) {
                throw new RuntimeException("invalid input. should not get here because we assume input is valid.");
            }

            if (nextByte == EOL_ENCODE) {
                decodeEndOfLine();
                newLineFeed = true;
                carriageReturn = false;
            }
            else {
                handleCarriedCarriageReturn();
                decodeByte(nextByte);
                newLineFeed = false;
            }

            return null;
        }

        private void handleCarriedCarriageReturn() {
            if (carriageReturn) {
                // We skipped it, therefore we're adding it
                buffer.pushByte((byte)EOL_OPTIONAL);
                carriageReturn = false;
            }
        }

        private void decodeByte(byte nextByte) {
            if (nextByte == EOL_OPTIONAL) {
                // it's optional, we need to skip
                carriageReturn = true;
            }
            else if (nextByte == HEADER_VALUE_SEPARATOR && headerName == null) {
                popHeaderName();
            }
            else {
                buffer.pushByte(nextByte);
            }
        }

        private void popHeaderName() {
            headerName = bufferReader.popString();
            if (headerName.length() == 0) {
                throw new RuntimeException("invalid input. should not get here because we assume input is valid.");
            }
        }

        private void decodeEndOfLine() {
            if (newLineFeed) {
                nextDecodingState();
                Logger.incoming.appendLine("");
            }
            else {
                appendHeader();
            }
        }

        private void nextDecodingState() {
            decodeState = new BodyDecodeState();
        }

        private void appendHeader() {
            if (headerName == null) {
                throw new RuntimeException("invalid input. should not get here because we assume input is valid.");
            }

            String value = bufferReader.popString();
            if (value.length() == 0) {
                throw new RuntimeException("invalid input. should not get here because we assume input is valid.");
            }

            Logger.incoming.appendLine(headerName + ":" + value);
            frameBuilder.putHeader(headerName, value);
            reset();
        }

        private void reset() {
            headerName = null;
        }
    }

    private class BodyDecodeState implements DecodeState {
        @Override
        public Frame decodeNextByte(byte nextByte) {
            if (nextByte == NULL_TERMINATION) {
                return buildFrame();
            }

            buffer.pushByte(nextByte);
            return null;
        }

        private Frame buildFrame() {
            String body = bufferReader.popString();
            frameBuilder.setBody(body);
            Frame frame = frameBuilder.build();
            StompFrameEncoderDecoder.this.reset();
            Logger.incoming.appendLine(body);
            return frame;
        }
    }
}
