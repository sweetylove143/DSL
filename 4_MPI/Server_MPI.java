import java.rmi.*;
import java.rmi.server.*;
import java.sql.*;

interface DBInterface extends Remote {
    public String input(String name1) throws RemoteException;
}

public class Server_MPI extends UnicastRemoteObject implements DBInterface {
    int flag = 0, n, i, j;
    String name3;
    ResultSet r;

    public String reverse_str(String s1) {
        char ch[] = s1.toCharArray();
        String rev = "";
        for (int i = ch.length - 1; i >= 0; i--) {
            rev += ch[i];
        }
        return rev;
    }

    public Server_MPI() throws RemoteException {
        try {
            System.out.println("Initializing Server\nServer Ready");
        } catch (Exception e) {
            System.out.println("ERROR:" + e.getMessage());
        }
    }

    public static void main(String[] args) {
        try {
            Server_MPI rs = new Server_MPI();
            java.rmi.registry.LocateRegistry.createRegistry(1030).rebind("DBServ", rs);
        } catch (Exception e) {
            System.out.println("ERROR:" + e.getMessage());
        }
    }

    public String input(String name1) {
        try {
            name3 = reverse_str(name1);
            // name3=name1.concat(name2);
        } catch (Exception e) {
            System.out.println("ERROR:" + e.getMessage());
        }
        return name3;
    }
}
