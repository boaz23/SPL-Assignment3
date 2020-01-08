
#ifndef STOMPCLIENT_FRAME_H
#define STOMPCLIENT_FRAME_H

#include <string>
#include <unordered_map>

// TODO: add fields
// TODO: add more subclasses
class Frame {
private:
    std::string _messageType;
    std::unordered_map<std::string, std::string> _headers;
    std::string _body;

public:
    const static std::string RECEIPT_ID;

    Frame();
    Frame(std::string messageType, std::unordered_map<std::string, std::string> headers, std::string body);

    std::string messageType();
    void setMessageType(std::string messageType);

    std::string body();
    void setBody(std::string body);

    std::unordered_map<std::string, std::string> headers();

    std::string receiptId();
    void setReceipt(std::string receiptId);

    virtual bool isValid();
};

class ConnectFrame : public Frame {
public:
    const static std::string ACCEPTVERSION_HEADER;
    const static std::string HOST_HEADER;
    const static std::string LOGIN_HEADER;
    const static std::string PASSCODE_HEADER;

    ConnectFrame(std::string acceptVersion, std::string host, std::string login, std::string passcode);
};

class ConnectedFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;
    const static std::string VERSION_HEADER;

    ConnectedFrame(std::string version);
    ConnectedFrame(Frame &&frame);

    std::string version();
};

#endif //STOMPCLIENT_FRAME_H
