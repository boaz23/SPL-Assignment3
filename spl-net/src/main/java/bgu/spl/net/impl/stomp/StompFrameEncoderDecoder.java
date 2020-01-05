package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.frames.Frame;
import sun.reflect.generics.reflectiveObjects.NotImplementedException;

public class StompFrameEncoderDecoder implements MessageEncoderDecoder<Frame> {
    public static String toString(Frame frame) {
        // TODO: implement
        throw new NotImplementedException();
    }

    @Override
    public Frame decodeNextByte(byte nextByte) {
        // TODO: implement
        throw new NotImplementedException();
    }

    @Override
    public byte[] encode(Frame message) {
        // TODO: implement
        return new byte[0];
    }
}
