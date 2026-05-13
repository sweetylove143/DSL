import java.io.*;
import java.rmi.registry.*;

public class Client {
    static String name1, name2, name3;

    public static void main(String args[]) {
        Client c = new Client();
        BufferedReader b = new BufferedReader(new InputStreamReader(System.in));
        int ch;
        try {
            Registry r1 = LocateRegistry.getRegistry("localhost", 1030);
            DBInterface DI = (DBInterface) r1.lookup("DBServ");
            do {
                System.out.println("1. Send input strings\n2. Display concatenated string\n0. Exit\nEnter your choice");
                ch = Integer.parseInt(b.readLine());
                switch (ch) {
                    case 1:
                        System.out.println("\nEnter first string:");
                        name1 = b.readLine();
                        System.out.println("\nEnter second string:");
                        name2 = b.readLine();
                        name3 = DI.input(name1, name2);
                        break;
                    case 2:
                        System.out.println("\nConcatenated String is: ");
                        System.out.println(name3);
                        break;
                    case 0:
                        System.out.println("Exiting client...");
                        break;
                    default:
                        System.out.println("Invalid choice");
                        break;
                }
            } while (ch != 0);
        } catch (Exception e) {
            System.out.println("ERROR: " + e.getMessage());
        }
    }
}

/*
 * Steps to run:
 * 1) javac DBInterface.java Server.java Client.java
 * 2) java Server (keep this running), then in another terminal run: java Client
 * 
 * Input examples:
 * 1) Choice 1 -> "Hello", "World", then Choice 2 -> output "HelloWorld"
 * 2) Choice 1 -> "Good", "Morning", then Choice 2 -> output "GoodMorning"
 * 
 * Expected output:
 * 1) Concatenated String is: HelloWorld
 * 2) Concatenated String is: GoodMorning
 */