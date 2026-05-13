import java.io.*;
import java.net.*;

public class Client_Threads {

    public static void main(String[] args) {

        String host = "localhost";
        int port = 6789;

        try (
                Socket socket = new Socket(host, port);
                BufferedReader serverInput = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                PrintWriter serverOutput = new PrintWriter(socket.getOutputStream(), true);
                BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in))) {

            System.out.println("Connected to server.");

            while (true) {
                System.out.print("Enter number (0 = exit, -1 = stop server): ");
                String input = userInput.readLine();

                serverOutput.println(input);

                int num = Integer.parseInt(input);

                if (num == 0 || num == -1) {
                    break;
                }

                String response = serverInput.readLine();
                System.out.println("Square from server: " + response);
            }

            System.out.println("Connection closed.");

        } catch (UnknownHostException e) {
            System.err.println("Unknown host.");
        } catch (IOException e) {
            System.err.println("I/O Error: " + e.getMessage());
        } catch (NumberFormatException e) {
            System.err.println("Please enter a valid integer.");
        }
    }
}

/*
 * Steps to run:
 * 1) javac Server_Threads.java Client_Threads.java
 * 2) java Server_Threads (keep this running), then run: java Client_Threads
 * 
 * Input examples:
 * 1) Enter number 4 -> output "Square from server: 16"
 * 2) Enter number 9 -> output "Square from server: 81"
 * 
 * Expected output:
 * 1) Square from server: 16
 * 2) Square from server: 81
 */