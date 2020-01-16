package bgu.spl.net;

public class StringBufferLogger {
    private StringBuilder buffer;

    public static final StringBufferLogger ReactorLogger = new StringBufferLogger();

    public StringBufferLogger() {
        buffer = new StringBuilder();
    }

    public synchronized StringBufferLogger append(String s) {
        buffer.append(s);
        return this;
    }

    public synchronized StringBufferLogger appendLine(String s) {
        buffer.append(s).append("\n");
        return this;
    }

    public synchronized StringBufferLogger appendLine() {
        buffer.append("\n");
        return this;
    }

    public synchronized boolean isEmpty() {
        return buffer.length() == 0;
    }

    @Override
    public String toString() {
        String s = buffer.toString();
        buffer.setLength(0);
        return s;
    }
}
