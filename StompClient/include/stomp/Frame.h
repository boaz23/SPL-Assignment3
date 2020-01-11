
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
    Frame(const std::string &messageType, const std::string &body);

    const std::string& messageType() const;
    void setMessageType(const std::string &messageType);

    const std::string& body() const;
    void setBody(const std::string &body);

    void addHeader(const std::string &name, const std::string &value);
    const std::unordered_map<std::string, std::string>& headers() const;

    const std::string& receiptId() const;
    void setReceiptId(const std::string &receiptId);

    virtual ~Frame() = default;
};

class ConnectFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_ACCEPTVERSION;
    const static std::string HEADER_HOST;
    const static std::string HEADER_LOGIN;
    const static std::string HEADER_PASSCODE;

    ConnectFrame(const std::string &acceptVersion, const std::string &host, const std::string &login, const std::string &passcode);

    const std::string& acceptVersion() const;
    void setAcceptVersion(const std::string &acceptVersion);

    const std::string& host() const;
    void setHost(const std::string &host);

    const std::string& loginName() const;
    void setLoginName(const std::string &loginName);

    const std::string& passcode() const;
    void setPasscode(const std::string &passcode);
};

class ConnectedFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_VERSION;

    ConnectedFrame();
    ConnectedFrame(const std::string &version);

    const std::string& version() const;
};

class DisconnectFame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    DisconnectFame();
};

class ErrorFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_MESSAGE;

    ErrorFrame();
    ErrorFrame(const std::string &msg);

    const std::string& errorMessage() const;
};

class ReceiptFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    ReceiptFrame();
    ReceiptFrame(const std::string &receiptId);
};

class SubscribeFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_ID;
    const static std::string HEADER_DESTINATION;

    SubscribeFrame(const std::string &destination, const std::string &subscriptionId);

    const std::string& destination() const;
    void setDestination(const std::string &destination);

    const std::string& subscriptionId() const;
    void setSubscriptionId(const std::string &subscriptionId);
};

class UnsubscribeFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_ID;

    UnsubscribeFrame(const std::string &subscriptionId);

    const std::string& subscriptionId() const;
    void setSubscriptionId(const std::string &subscriptionId);
};

class SendFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_DESTINATION;

    SendFrame(const std::string &destination, const std::string &body);

    const std::string& destination() const;
    void setDestination(const std::string &destination);
};

class MessageFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_SUBSCRIPTION;
    const static std::string HEADER_DESTINATION;
    const static std::string HEADER_MESSAGE_ID;

    MessageFrame();
    MessageFrame(const std::string &subscriptionId, const std::string &destination, const std::string &messageId, const std::string &body);

    const std::string& subscriptionId() const;
    void setSubscriptionId(const std::string &subscriptionId);

    const std::string& destination() const;
    void setDestination(const std::string &destination);

    const std::string& messageId() const;
    void setMessageId(const std::string &messageId);
};

#endif //STOMPCLIENT_FRAME_H
