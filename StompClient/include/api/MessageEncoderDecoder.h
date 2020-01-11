#ifndef STOMPCLIENT_MESSAGEENCODERDECODER_H
#define STOMPCLIENT_MESSAGEENCODERDECODER_H

#include "../Array.h"

using byte = char;

template <typename Tin, typename Tout> class MessageEncoderDecoder {
    /**
     * add the next byte to the decoding process
     *
     * @param nextByte the next byte to consider for the currently decoded
     * message
     * @return a message if this byte completes one or null if it doesnt.
     */
    virtual Tin* decodeNextByte(byte nextByte) = 0;

    /**
     * encodes the given message to bytes array
     *
     * @param message the message to encode
     * @return the encoded bytes
     */
    virtual Array<byte> encode(Tout &message) = 0;
};


#endif //STOMPCLIENT_MESSAGEENCODERDECODER_H
