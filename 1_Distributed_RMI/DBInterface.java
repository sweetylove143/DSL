import java.rmi.Remote; 
import java.rmi.RemoteException; 

public interface DBInterface extends Remote { 
    String input(String name1, String name2) throws RemoteException; 
} 