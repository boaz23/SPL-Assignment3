#include "../../include/stomp/StompMessageEncoderDecoder.h"

StompMessageEncoderDecoder::StompMessageEncoderDecoder() : byteVector(1024) { }

Frame* StompMessageEncoderDecoder::decodeNextByte(byte nextByte) {
    if(nextByte != '\0') {
        byteVector.push_back(nextByte);
    }
    else {
        return buildFrame();
    }

    return nullptr;
}

Array<byte> StompMessageEncoderDecoder::encode(const Frame &message) {
    std::string data;
    encodeMessage(message, data);
    encodeHeaders(message, data);
    encodeBody(message, data);

    // TODO: move op
    byte *bytes = new byte[data.length()];
    for (unsigned long i = 0; i < data.length(); ++i) {
        bytes[i] = data[i];
    }

    return Array<byte>(bytes, data.length());
}

void StompMessageEncoderDecoder::encodeMessage(const Frame &message, std::string &data) const {
    data.append(message.messageType(), message.messageType().length());
    data.append("\n");
}

void StompMessageEncoderDecoder::encodeHeaders(const Frame &message, std::string &data) const {
    for(const auto &header : message.headers()){
        data.append(header.first);
        data.append(":");
        data.append(header.second);
        data.append("\n");
    }
    data.append("\n");
}

void StompMessageEncoderDecoder::encodeBody(const Frame &message, std::string &data) const {
    data.append(message.body(), message.body().length());
    data.append("\0");
}

Frame* StompMessageEncoderDecoder::buildFrame() {
    int index = 0;
    std::string line;
    getLine(line, index);

    Frame *frame = createFrameInstance(line);
    frame->setMessageType(line);
    while (index < byteVector.size() && byteVector[index] != '\n') {
        std::string key;
        std::string value;
        decodeKey(index, key);
        decodeValue(index, value);
        frame->setHeader(key, value);
    }
    if (index >= byteVector.size()) {
        throw std::runtime_error("Received invalid frame");
    }
    decodeBody(frame, index);

    return frame;
}

Frame *StompMessageEncoderDecoder::createFrameInstance(const std::string &line) {
    if (line == ConnectedFrame::MESSAGE_TYPE) {
        return new ConnectedFrame();
    }
    else if (line == ErrorFrame::MESSAGE_TYPE) {
        return new ErrorFrame();
    }
    else if (line == ReceiptFrame::MESSAGE_TYPE) {
        return new ReceiptFrame();
    }
    else if (line == MessageFrame::MESSAGE_TYPE) {
        return new MessageFrame();
    }
    else {
        return nullptr;
    }
}

void StompMessageEncoderDecoder::getLine(std::string &line, int &index) const {
    for(; index < byteVector.size(); ++index){
        byte b = byteVector[index];
        if(b == '\r') {
            if (index < byteVector.size() - 1 && byteVector[index + 1] == '\n'){
                //TODO: check the increment
                index = index + 2;
                break;
            }
        }
        else if(b == '\n') {
            //TODO: check the increment
            ++index;
            break;
        }
        line.append(&b, 1);
    }
}

void StompMessageEncoderDecoder::decodeKey(int &index, std::string &key) const {
    for (; index < byteVector.size(); ++index) {
        byte b = byteVector[index];
        if (b == ':') {
            //TODO: check the inc
            ++index;
            break;
        }
        key.append(&b, 1);
    }
}

void StompMessageEncoderDecoder::decodeValue(int &index, std::string &value) const {
    for (; index < byteVector.size(); ++index) {
        byte b = byteVector[index];
        if (b == '\n') {
            ++index;
            break;
        }
        value.append(&b, 1);
    }
}

void StompMessageEncoderDecoder::decodeBody(Frame *frame, int &index) const {
    std::string body;
    for (; index < byteVector.size(); index = index + 1) {
        byte b = byteVector[index];
        if (b == '\0') {
            ++index;
            break;
        }
        body.append(&b, 1);
    }
    frame->setBody(body);
}
