import java.io.*;
import java.net.*;

public class Server_Threads {

    private ServerSocket serverSocket;
    private int port;
    private int connectionCount = 0;

    public Server_Threads(int port) {
        this.port = port;
    }

    public void startServer() {
        try {
            serverSocket = new ServerSocket(port);
            System.out.println("Server started on port " + port);
            System.out.println("Waiting for clients...");
        } catch (IOException e) {
            System.err.println("Error starting server: " + e.getMessage());
            return;
        }

        while (true) {
            try {
                Socket clientSocket = serverSocket.accept();
                connectionCount++;

                System.out.println("Client #" + connectionCount + " connected: " + clientSocket);

                // Create new thread for each client
                new Thread(new ClientHandler(clientSocket, connectionCount, this)).start();

            } catch (IOException e) {
                System.err.println("Error accepting connection: " + e.getMessage());
            }
        }
    }

    public void stopServer() {
        System.out.println("Shutting down server...");
        try {
            serverSocket.close();
        } catch (IOException e) {
            System.err.println("Error closing server: " + e.getMessage());
        }
        System.exit(0);
    }

    public static void main(String[] args) {
        int port = 6789;
        new Server_Threads(port).startServer();
    }
}

// Thread class
class ClientHandler implements Runnable {

    private Socket socket;
    private int clientId;
    private Server_Threads server;

    private BufferedReader input;
    private PrintWriter output;

    public ClientHandler(Socket socket, int clientId, Server_Threads server) {
        this.socket = socket;
        this.clientId = clientId;
        this.server = server;

        try {
            input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            output = new PrintWriter(socket.getOutputStream(), true);
        } catch (IOException e) {
            System.err.println("Error setting up streams: " + e.getMessage());
        }
    }

    @Override
    public void run() {
        try {
            String line;
            boolean stopServer = false;

            while ((line = input.readLine()) != null) {

                System.out.println("Client #" + clientId + " sent: " + line);

                int num = Integer.parseInt(line);

                if (num == -1) {
                    stopServer = true;
                    break;
                }

                if (num == 0) {
                    break;
                }

                output.println(num * num);
            }

            System.out.println("Client #" + clientId + " disconnected");

            socket.close();

            if (stopServer) {
                server.stopServer();
            }

        } catch (IOException | NumberFormatException e) {
            System.err.println("Error in client #" + clientId + ": " + e.getMessage());
        }
    }
}