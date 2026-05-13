import java.rmi.*;
import java.rmi.server.*;
import java.sql.ResultSet;

public class Server extends UnicastRemoteObject implements DBInterface {
    int flag = 0, n, i, j;
    String name3;
    ResultSet r;

    public Server() throws RemoteException {
        try {
            System.out.println("Initializing Server\nServer Ready");
        } catch (Exception e) {
            System.out.println("ERROR: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        try {
            Server rs = new Server();
            java.rmi.registry.LocateRegistry.createRegistry(1030).rebind("DBServ", rs);
        } catch (Exception e) {
            System.out.println("ERROR: " + e.getMessage());
        }
    }

    public String input(String name1, String name2) {
        try {
            name3 = name1.concat(name2);
        } catch (Exception e) {
            System.out.println("ERROR: " + e.getMessage());
        }
        return name3;
    }
}

/*
 * Steps to run:
 * 1) javac DBInterface.java Server.java Client.java
 * 2) java Server (keep this running), then in another terminal run: java Client
 * 
 * Input examples (from client):
 * 1) "Hello" and "World"
 * 2) "Good" and "Morning"
 * 
 * Expected output:
 * 1) Client receives "HelloWorld"
 * 2) Client receives "GoodMorning"
 */