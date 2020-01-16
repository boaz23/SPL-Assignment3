package bgu.spl.net;

import java.io.Closeable;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

public class FileLogger implements Closeable {
    private FileWriter fileWriter;

    public static FileLogger incoming;
    public static FileLogger outgoing;
    static {
        Date date = Calendar.getInstance().getTime();
        DateFormat dateFormat = new SimpleDateFormat("yyyy-mm-dd hh:mm:ss");
        String sDate = dateFormat.format(new Date());
        incoming = new FileLogger("/users/studs/bsc/2020/pratn/Desktop/log-incoming-" + sDate + ".log");
        outgoing = new FileLogger("/users/studs/bsc/2020/pratn/Desktop/log-outgoing-" + sDate + ".log");
    }

    public FileLogger(String path) {
//        try {
//            fileWriter = new FileWriter(path);
//        } catch (IOException e) {
//            e.printStackTrace();
//        }
    }

    public FileLogger appendLine(String line) {
//        synchronized (this) {
//            try {
//                fileWriter.write(line + "\n");
//                fileWriter.flush();
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//        }
        return this;
    }

    @Override
    public void close() throws IOException {
        if (fileWriter != null) {
            fileWriter.close();
        }
    }
}
