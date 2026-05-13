import java.io.*;
import java.lang.*;
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
                System.out.println("\n\t\t*** Menu ***");
                System.out.println("\n 1. Send Input Strings");
                System.out.println("\n 2. Display Reversed String ");
                System.out.println("\n Enter your choice:");
                ch = Integer.parseInt(b.readLine());
                switch (ch) {
                    case 1: {
                        System.out.println("Enter First String :");
                        name1 = b.readLine();
                        name3 = DI.input(name1);
                        break;
                    }
                    case 2: {
                        System.out.println("\nReversed String = " + name3);
                        break;
                    }
                }
            } while (ch > 0);
        } catch (Exception e) {
            System.out.println("ERROR:" + e.getMessage());
        }
    }
}
