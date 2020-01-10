
#include "../../include/stomp/StompMessageEncoderDecoder.h"

StompMessageEncoderDecoder::StompMessageEncoderDecoder() : byteVector(1 << 10){}

Frame* StompMessageEncoderDecoder::decodeNextByte(byte nextByte) {
    if(nextByte != '\0'){
        byteVector.push_back(nextByte);
    } else {
        return createFrame();
    }

    return nullptr;
}

Array<byte> StompMessageEncoderDecoder::encode( Frame &message) {
    std::string *data = new std::string();

    encodeMessage(message, data);
    encodeHeaders(message, data);
    encodeBody(message, data);

    // move op
    Array<byte> arr;
    arr.array = data->c_str();
    arr.length = data->length();

    return  arr;
}

void StompMessageEncoderDecoder::encodeBody(const Frame &message, std::string *data) const {
    data->append(message.body(), message.body().length());
    data->append("\0");
}

void StompMessageEncoderDecoder::encodeMessage(const Frame &message, std::string *data) const {
    data->append(message.messageType(), message.messageType().length());
    data->append("\n");
}

void StompMessageEncoderDecoder::encodeHeaders(Frame &message, std::string *data) const {
    for(const auto &header : message.headers()){
        data->append(header.first);
        data->append(":");
        data->append(header.second);
        data->append("\n");
    }
    data->append("\n");
}

Frame* StompMessageEncoderDecoder::createFrame() {

    Frame *frame = new Frame();
    int index = 0;
    decodeMessage(frame, index);

    if(index < byteVector.size()){
        byte b = byteVector[index];
        if(b == '\n') { // body
            decodeBody(frame, index);
        } else {
            while (index < byteVector.size() && byteVector[index] != '\n') {
                //TODO: check of more delimi
                std::string key = "";
                std::string value = "";

                decodeKey(index, key);
                decodeValue(index, value);

                //TODO : add the header
            }
        }
    }

    return frame;
}

void StompMessageEncoderDecoder::decodeBody(Frame *frame, int &index) const {
    std::string body = "";
    for (; index < byteVector.size(); index = index + 1) {
        byte b = byteVector[index];
        if (b == '\0') {
            break;
        }
        body.append(&b, 1);
    }
    frame->setBody(body);
}

void StompMessageEncoderDecoder::decodeKey(int &index, std::string &key) const {
    for (; index < byteVector.size(); index = index + 1) {
        byte b = byteVector[index];
        if (b == ':') {
            break;
        }
        key.append(&b, 1);
    }
}

void StompMessageEncoderDecoder::decodeValue(int &index, std::string &value) const {
    for (; index < byteVector.size(); index = index + 1) {
        byte b = byteVector[index];
        if (b == '\n') {
            break;
        }
        value.append(&b, 1);
    }
}

void StompMessageEncoderDecoder::decodeMessage(Frame *frame, int &index) const {
    std::string line = "";

    for(index=0; index < byteVector.size(); index = index + 1){
        byte b = byteVector[index];
        if(b == '\r'){
            if(index < byteVector.size() - 1 && byteVector[index + 1] == '\n'){
                index = index + 1;
                break;
            }
        }
        if(b == '\n'){
            break;
        }
        line.append(&b, 1);

    }
    frame->setMessageType(line);
}
