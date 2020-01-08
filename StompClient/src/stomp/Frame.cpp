
#include "../../include/stomp/Frame.h"

const std::string Frame::RECEIPT_ID = "receipt-id";

const std::string ConnectFrame::ACCEPTVERSION_HEADER = "accept-version";
const std::string ConnectFrame::HOST_HEADER = "host";
const std::string ConnectFrame::LOGIN_HEADER = "login";
const std::string ConnectFrame::PASSCODE_HEADER = "passcode";

const std::string ConnectedFrame::MESSAGE_TYPE = "CONNECTED";
const std::string ConnectedFrame::VERSION_HEADER = "version";
