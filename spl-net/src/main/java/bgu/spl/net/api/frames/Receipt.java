package bgu.spl.net.api.frames;

public class Receipt extends Frame {
    public static final String RECEIPT_ID_HEADER = "receipt-id";

    public Receipt(String receiptId) {
        super("RECEIPT", "");
        setReceiptId(receiptId);
    }

    public String getReceiptId() {
        return getHeader(RECEIPT_ID_HEADER);
    }

    public void setReceiptId(String receiptId) {
        setHeader(RECEIPT_ID_HEADER, receiptId);
    }
}
