package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.api.frames.Frame;
import bgu.spl.net.srv.Server;
import bgu.spl.net.srv.connections.ConnectionHandlersManager;
import bgu.spl.net.srv.connections.ConnectionIdsManager;
import bgu.spl.net.srv.connections.IncrementConnectionsIdManager;

import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.function.Supplier;

public class StompServer {
    public static void main(String[] args) {
        if (args.length < 2) {
            argumentsError("Not enough arguments.", 1);
        }

        int port = parsePort(args[0]);
        String policy = args[1];

        Server<Frame> server = initServer(port, policy, StandardCharsets.UTF_8);
        server.serve();
    }

    private static int parsePort(String sPort) {
        int port;
        try {
            port = Integer.parseInt(sPort);
        }
        catch (NumberFormatException e) {
            argumentsError("Expect a port number in the first argument.", 2);
            throw new RuntimeException("cannot get here because process has exited.");
        }
        return port;
    }

    @SuppressWarnings("SameParameterValue")
    private static Server<Frame> initServer(int port, String policy, Charset encoding) {
        Server<Frame> server;
        Supplier<StompMessagingProtocol> protocolFactory = StompMessagingProtocolImpl::new;
        Supplier<StompMessageEncoderDecoder> encoderDecoderFactory = () -> new StompFrameEncoderDecoder(encoding);
        ConnectionHandlersManager<Frame> stompConnections = new StompConnections(new IncrementConnectionsIdManager());
        if (policy.equals("tpc")) {
            server = initTpcServer(port, protocolFactory, encoderDecoderFactory, stompConnections);
        }
        else if (policy.equals("reactor")) {
            server = initReactorServer(port, protocolFactory, encoderDecoderFactory, stompConnections);
        }
        else {
            argumentsError("'" + policy + "' is not a valid policy", 3);
            throw new RuntimeException("cannot get here because process has exited.");
        }

        return server;
    }

    private static Server<Frame> initTpcServer(int port, Supplier<StompMessagingProtocol> protocolFactory, Supplier<StompMessageEncoderDecoder> encoderDecoderFactory, ConnectionHandlersManager<Frame> stompConnections) {
        return Server.threadPerClient(port, protocolFactory, encoderDecoderFactory, stompConnections);
    }

    private static Server<Frame> initReactorServer(int port, Supplier<StompMessagingProtocol> protocolFactory, Supplier<StompMessageEncoderDecoder> encoderDecoderFactory, ConnectionHandlersManager<Frame> stompConnections) {
        int numThreads = Runtime.getRuntime().availableProcessors();
        return Server.reactor(
            numThreads,
            port,
            protocolFactory,
            encoderDecoderFactory,
            stompConnections
        );
    }

    private static void argumentsError(String message, int exitCode) {
        System.out.println(message);
        System.out.println("Usage: <port> <policy = tpc | reactor>");
        System.exit(exitCode);
    }
}
