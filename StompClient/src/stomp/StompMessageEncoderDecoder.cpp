#include "../../include/stomp/StompMessageEncoderDecoder.h"

StompMessageEncoderDecoder::StompMessageEncoderDecoder() : byteVector() { }

std::unique_ptr<Frame> StompMessageEncoderDecoder::decodeNextByte(byte nextByte) {
    if(nextByte != '\0') {
        byteVector.push_back(nextByte);
    }
    else {
        return buildFrame();
    }

    return nullptr;
}

std::unique_ptr<std::string> StompMessageEncoderDecoder::encode(const Frame &message) {
    auto *data = new std::string();
    encodeMessage(message, *data);
    encodeHeaders(message, *data);
    encodeBody(message, *data);
    return std::unique_ptr<std::string>(data);
}

void StompMessageEncoderDecoder::encodeMessage(const Frame &message, std::string &data) const {
    data.append(message.messageType());
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
    data.append(message.body());
    data.resize(data.length() + 1, '\0');
}

std::unique_ptr<Frame> StompMessageEncoderDecoder::buildFrame() {
    size_t index = 0;
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
    ++index;
    if (index > byteVector.size()) {
        throw std::runtime_error("Received invalid frame");
    }
    decodeBody(frame, index);

    byteVector.clear();
    return std::unique_ptr<Frame>(frame);
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

void StompMessageEncoderDecoder::getLine(std::string &line, size_t &index) const {
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

void StompMessageEncoderDecoder::decodeKey(size_t &index, std::string &key) const {
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

void StompMessageEncoderDecoder::decodeValue(size_t &index, std::string &value) const {
    for (; index < byteVector.size(); ++index) {
        byte b = byteVector[index];
        if (b == '\n') {
            ++index;
            break;
        }
        value.append(&b, 1);
    }
}

void StompMessageEncoderDecoder::decodeBody(Frame *frame, size_t &index) const {
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
