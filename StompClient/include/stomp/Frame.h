
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
    const static std::string HEADER_RECEIPT_ID;

    Frame();
    Frame(std::string messageType, std::unordered_map<std::string, std::string> headers, std::string body);

    std::string messageType() const;
    void setMessageType(std::string messageType);

    std::string body() const;
    void setBody(std::string body);

    std::unordered_map<std::string, std::string> headers();

    std::string receiptId() const;
    void setReceipt(std::string receiptId);

    virtual bool isValid();
};

class ConnectFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_ACCEPTVERSION;
    const static std::string HEADER_HOST;
    const static std::string HEADER_LOGIN;
    const static std::string HEADER_PASSCODE;

    ConnectFrame(std::string acceptVersion, std::string host, std::string login, std::string passcode);
};

class ConnectedFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_VERSION;

    ConnectedFrame(std::string version);
    ConnectedFrame(Frame &&frame);

    std::string version();
};

class DisconnectFame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    DisconnectFame();
    DisconnectFame(Frame &&frame);
};

class ErrorFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_MESSAGE;

    ErrorFrame(std::string msg);
    ErrorFrame(Frame &&frame);

    std::string errorMessage();
};

class ReceiptFrame : public Frame {
    const static std::string MESSAGE_TYPE;

    ReceiptFrame(std::string receiptId);
    ReceiptFrame(Frame &&frame);
};

class SubscribeFrame : public Frame {
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_DESTINATION;
    const static std::string HEADER_ID;

    SubscribeFrame(std::string destination, std::string subscriptionId);
    SubscribeFrame(Frame &&frame);
};

#endif //STOMPCLIENT_FRAME_H
