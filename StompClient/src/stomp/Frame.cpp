
#include "../../include/stomp/Frame.h"

const std::string Frame::HEADER_RECEIPT_ID = "receipt-id";

const std::string ConnectFrame::MESSAGE_TYPE = "CONNECT";
const std::string ConnectFrame::HEADER_ACCEPTVERSION = "accept-version";
const std::string ConnectFrame::HEADER_HOST = "host";
const std::string ConnectFrame::HEADER_LOGIN = "login";
const std::string ConnectFrame::HEADER_PASSCODE = "passcode";

const std::string ConnectedFrame::MESSAGE_TYPE = "CONNECTED";
const std::string ConnectedFrame::HEADER_VERSION = "version";

const std::string DisconnectFame::MESSAGE_TYPE = "DISCONNECT";

const std::string ErrorFrame::MESSAGE_TYPE = "ERROR";
const std::string ErrorFrame::HEADER_MESSAGE = "message";

const std::string ReceiptFrame::MESSAGE_TYPE = "RECEIPT";