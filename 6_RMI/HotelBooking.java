import java.rmi.Remote;
import java.rmi.RemoteException;

public interface HotelBooking extends Remote {
    String getAvailability() throws RemoteException;

    int bookRoom(String guestName, String roomType, int nights) throws RemoteException;

    boolean cancelBooking(int bookingId) throws RemoteException;

    String getBooking(int bookingId) throws RemoteException;
}
