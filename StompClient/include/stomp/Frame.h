
#ifndef STOMPCLIENT_FRAME_H
#define STOMPCLIENT_FRAME_H

#include <string>
#include <unordered_map>

class Frame {
private:
    std::string _messageType;
    std::unordered_map<std::string, std::string> _headers;
    std::string _body;

public:
    const static std::string HEADER_RECEIPT;

    Frame() : _messageType(), _headers(), _body() { }
    Frame(std::string messageType) : _messageType(std::move(messageType)), _headers(), _body() { }
    Frame(std::string messageType, std::string body) : _messageType(std::move(messageType)), _headers(), _body(std::move(body)) { }

    const std::string& messageType() const {
        return _messageType;
    }
    Frame& setMessageType(const std::string &messageType) {
        _messageType = messageType;
        return *this;
    }

    const std::string& body() const {
        return _body;
    }
    Frame& setBody(const std::string &body) {
        _body = body;
        return *this;
    }

    const std::string& getHeader(const std::string &name) const {
        return _headers.at(name);
    }
    Frame& setHeader(const std::string &name, const std::string &value) {
        _headers[name] = value;
        return *this;
    }
    const std::unordered_map<std::string, std::string>& headers() const {
        return _headers;
    }

    const std::string& receipt() const {
        return getHeader(HEADER_RECEIPT);
    }
    Frame& setReceipt(const std::string &receiptId) {
        return setHeader(HEADER_RECEIPT, receiptId);
    }

    virtual ~Frame() = default;
};

class ConnectFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_ACCEPTVERSION;
    const static std::string HEADER_HOST;
    const static std::string HEADER_LOGIN;
    const static std::string HEADER_PASSCODE;

    ConnectFrame(const std::string &acceptVersion, const std::string &host, const std::string &login, const std::string &passcode) :
        Frame(MESSAGE_TYPE) {
        setHeader(HEADER_ACCEPTVERSION, acceptVersion);
        setHeader(HEADER_HOST, host);
        setHeader(HEADER_LOGIN, login);
        setHeader(HEADER_PASSCODE, passcode);
    }

    const std::string& acceptVersion() const {
        return getHeader(HEADER_ACCEPTVERSION);
    }
    Frame& setAcceptVersion(const std::string &acceptVersion) {
        return setHeader(HEADER_ACCEPTVERSION, acceptVersion);
    }

    const std::string& host() const {
        return getHeader(HEADER_HOST);
    }
    Frame& setHost(const std::string &host) {
        return setHeader(HEADER_HOST, host);
    }

    const std::string& loginName() const {
        return getHeader(HEADER_LOGIN);
    }
    Frame& setLoginName(const std::string &loginName) {
        return setHeader(HEADER_LOGIN, loginName);
    }

    const std::string& passcode() const {
        return getHeader(HEADER_PASSCODE);
    }
    Frame& setPasscode(const std::string &passcode) {
        return setHeader(HEADER_PASSCODE, passcode);
    }
};

class ConnectedFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_VERSION;

    ConnectedFrame() : Frame(MESSAGE_TYPE) { }
    ConnectedFrame(const std::string &version) : Frame(MESSAGE_TYPE) {
        setHeader(HEADER_VERSION, version);
    }

    const std::string& version() const {
        return getHeader(HEADER_VERSION);
    }
};

class DisconnectFame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    DisconnectFame() : Frame(MESSAGE_TYPE) { }
};

class ErrorFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_MESSAGE;

    ErrorFrame() : Frame(MESSAGE_TYPE) { }
    ErrorFrame(const std::string &msg) : Frame(MESSAGE_TYPE) {
        setHeader(HEADER_MESSAGE, msg);
    }

    const std::string& errorMessage() const {
        return getHeader(HEADER_MESSAGE);
    }
};

class ReceiptFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_RECEIPT_ID;

    ReceiptFrame() : Frame(MESSAGE_TYPE) { }
    ReceiptFrame(const std::string &receiptId) : Frame(MESSAGE_TYPE) {
        setHeader(HEADER_RECEIPT_ID, receiptId);
    }
};

class SubscribeFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_DESTINATION;
    const static std::string HEADER_ID;

    SubscribeFrame(const std::string &destination, const std::string &subscriptionId) : Frame(MESSAGE_TYPE) {
        setHeader(HEADER_DESTINATION, destination);
        setHeader(HEADER_ID, subscriptionId);
    }

    const std::string& destination() const {
        return getHeader(HEADER_DESTINATION);
    }
    Frame& setDestination(const std::string &destination) {
        return setHeader(HEADER_DESTINATION, destination);
    }

    const std::string& subscriptionId() const {
        return getHeader(HEADER_ID);
    }
    Frame& setSubscriptionId(const std::string &subscriptionId) {
        return setHeader(HEADER_ID, subscriptionId);
    }
};

class UnsubscribeFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_ID;

    UnsubscribeFrame(const std::string &subscriptionId) : Frame(MESSAGE_TYPE) {
        setHeader(HEADER_ID, subscriptionId);
    }

    const std::string& subscriptionId() const {
        return getHeader(HEADER_ID);
    }
    Frame& setSubscriptionId(const std::string &subscriptionId) {
        return setHeader(HEADER_ID, subscriptionId);
    }
};

class SendFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_DESTINATION;

    SendFrame(const std::string &destination, const std::string &body) : Frame(MESSAGE_TYPE, body) {
        setHeader(HEADER_DESTINATION, destination);
    }

    const std::string& destination() const {
        return getHeader(HEADER_DESTINATION);
    }
    Frame& setDestination(const std::string &destination) {
        return setHeader(HEADER_DESTINATION, destination);
    }
};

class MessageFrame : public Frame {
public:
    const static std::string MESSAGE_TYPE;

    const static std::string HEADER_SUBSCRIPTION;
    const static std::string HEADER_DESTINATION;
    const static std::string HEADER_MESSAGE_ID;

    MessageFrame() : Frame(MESSAGE_TYPE) { }
    MessageFrame(const std::string &subscriptionId, const std::string &destination, const std::string &messageId, const std::string &body) :
        Frame(MESSAGE_TYPE, body) {
        setHeader(HEADER_SUBSCRIPTION, subscriptionId);
        setHeader(HEADER_DESTINATION, destination);
        setHeader(HEADER_MESSAGE_ID, messageId);
    }

    const std::string& subscriptionId() const {
        return getHeader(HEADER_SUBSCRIPTION);
    }
    Frame& setSubscriptionId(const std::string &subscriptionId) {
        return setHeader(HEADER_SUBSCRIPTION, subscriptionId);
    }

    const std::string& destination() const {
        return getHeader(HEADER_DESTINATION);
    }
    Frame& setDestination(const std::string &destination) {
        return setHeader(HEADER_DESTINATION, destination);
    }

    const std::string& messageId() const {
        return getHeader(HEADER_MESSAGE_ID);
    }
    Frame& setMessageId(const std::string &messageId) {
        return setHeader(HEADER_MESSAGE_ID, messageId);
    }
};

#endif //STOMPCLIENT_FRAME_H
