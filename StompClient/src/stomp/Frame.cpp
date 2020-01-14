
#include "../../include/stomp/Frame.h"

const std::string Frame::HEADER_RECEIPT = "receipt";

const std::string ConnectFrame::MESSAGE_TYPE = "CONNECT";
const std::string ConnectFrame::HEADER_ACCEPTVERSION = "accept-version";
const std::string ConnectFrame::HEADER_HOST = "host";
const std::string ConnectFrame::HEADER_LOGIN = "login";
const std::string ConnectFrame::HEADER_PASSCODE = "passcode";

const std::string ConnectedFrame::MESSAGE_TYPE = "CONNECTED";
const std::string ConnectedFrame::HEADER_VERSION = "version";

const std::string DisconnectFame::MESSAGE_TYPE = "DISCONNECT";

const std::string ErrorFrame::MESSAGE_TYPE = "ERROR";
const std::string ErrorFrame::HEADER_lMESSAGE = "message";

const std::string ReceiptFrame::MESSAGE_TYPE = "RECEIPT";
const std::string ReceiptFrame::HEADER_RECEIPT_ID = "receipt-id";

const std::string SubscribeFrame::MESSAGE_TYPE = "SUBSCRIBE";
const std::string SubscribeFrame::HEADER_ID = "id";
const std::string SubscribeFrame::HEADER_DESTINATION = "destination";

const std::string UnsubscribeFrame::MESSAGE_TYPE = "UNSUBSCRIBE";
const std::string UnsubscribeFrame::HEADER_ID = "id";

const std::string SendFrame::MESSAGE_TYPE = "SEND";
const std::string SendFrame::HEADER_DESTINATION = "destination";

const std::string MessageFrame::MESSAGE_TYPE = "MESSAGE";
const std::string MessageFrame::HEADER_DESTINATION = "destination";
const std::string MessageFrame::HEADER_SUBSCRIPTION = "subscription";
const std::string MessageFrame::HEADER_MESSAGE_ID = "message-id";
