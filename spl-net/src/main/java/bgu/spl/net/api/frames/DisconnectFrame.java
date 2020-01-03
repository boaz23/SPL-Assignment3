package bgu.spl.net.api.frames;

public class DisconnectFrame extends Frame {

    public static final String RECEIPT_ID_HEADER = "receipt-id";

    public DisconnectFrame(String recepitId) {
        super("SEND", "");
        setReceiptId(recepitId);
    }

    public String getReceiptId() {
        return getHeader(RECEIPT_ID_HEADER);
    }

    public void setReceiptId(String receiptId) {
        setHeader(RECEIPT_ID_HEADER, receiptId);
    }
}

