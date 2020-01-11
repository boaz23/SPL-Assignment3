
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
    Frame(const Frame &frame);
    Frame(std::string messageType, std::unordered_map<std::string, std::string> headers, std::string body);

    std::string messageType() const;
    void setMessageType(std::string messageType);

    std::string body() const;
    void setBody(std::string body);

    void addHeader(std::string name, std::string value);
    const std::unordered_map<std::string, std::string>& headers() const;

    std::string receiptId() const;
    void setReceiptId(std::string receiptId);

    virtual ~Frame();
};

class ConnectFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_ACCEPTVERSION;
    const static std::string HEADER_HOST;
    const static std::string HEADER_LOGIN;
    const static std::string HEADER_PASSCODE;

    ConnectFrame(std::string acceptVersion, std::string host, std::string login, std::string passcode);

    std::string acceptVersion();
    void setAcceptVersion(std::string acceptVersion);

    std::string host();
    void setHost(std::string host);

    std::string loginName();
    void setLoginName(std::string loginName);

    std::string passcode();
    void setPasscode(std::string passcode);
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
public:
    const static std::string MESSAGE_TYPE;

    ReceiptFrame(std::string receiptId);
    ReceiptFrame(Frame &&frame);
};

class SubscribeFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_ID;
    const static std::string HEADER_DESTINATION;

    SubscribeFrame(std::string destination, std::string subscriptionId);
    SubscribeFrame(Frame &&frame);

    std::string destination();
    void setDestination(std::string destination);

    std::string subscriptionId();
    void setSubscriptionId(std::string subscriptionId);
};

class UnsubscribeFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_ID;

    UnsubscribeFrame(std::string subscriptionId);
    UnsubscribeFrame(Frame &&frame);

    std::string subscriptionId();
    void setSubscriptionId(std::string subscriptionId);
};

class SendFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_DESTINATION;

    SendFrame(std::string destination, std::string body);
    SendFrame(Frame &&frame);

    std::string destination();
    void setDestination(std::string destination);
};

class MessageFrame : public Frame {
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_SUBSCRIPTION;
    const static std::string HEADER_DESTINATION;
    const static std::string HEADER_MESSAGE_ID;

    MessageFrame(std::string subscriptionId, std::string destination, std::string messageId, std::string body);
    MessageFrame(Frame &&frame);

    std::string subscriptionId();
    void setSubscriptionId(std::string subscriptionId);

    std::string destination();
    void setDestination(std::string destination);

    std::string messageId();
    void setMessageId(std::string messageId);
};

#endif //STOMPCLIENT_FRAME_H
