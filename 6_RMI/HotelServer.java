import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.HashMap;
import java.util.Map;

public class HotelServer extends UnicastRemoteObject implements HotelBooking {
    private static class Booking {
        private final int id;
        private final String guestName;
        private final String roomType;
        private final int nights;

        Booking(int id, String guestName, String roomType, int nights) {
            this.id = id;
            this.guestName = guestName;
            this.roomType = roomType;
            this.nights = nights;
        }

        @Override
        public String toString() {
            return "Booking " + id + ": " + guestName + " - " + roomType + " for " + nights + " night(s)";
        }
    }

    private final Map<Integer, Booking> bookings = new HashMap<>();
    private final Map<String, Integer> availability = new HashMap<>();
    private int nextId = 1001;

    protected HotelServer() throws RemoteException {
        availability.put("SINGLE", 3);
        availability.put("DOUBLE", 2);
        availability.put("DELUXE", 1);
    }

    @Override
    public synchronized String getAvailability() {
        return "SINGLE=" + availability.get("SINGLE")
                + ", DOUBLE=" + availability.get("DOUBLE")
                + ", DELUXE=" + availability.get("DELUXE");
    }

    @Override
    public synchronized int bookRoom(String guestName, String roomType, int nights) {
        if (guestName == null || guestName.trim().isEmpty()) {
            return -1;
        }
        if (roomType == null || !availability.containsKey(roomType)) {
            return -1;
        }
        if (nights <= 0) {
            return -1;
        }

        int roomsLeft = availability.get(roomType);
        if (roomsLeft <= 0) {
            return -1;
        }

        int id = nextId++;
        Booking booking = new Booking(id, guestName.trim(), roomType, nights);
        bookings.put(id, booking);
        availability.put(roomType, roomsLeft - 1);
        return id;
    }

    @Override
    public synchronized boolean cancelBooking(int bookingId) {
        Booking booking = bookings.remove(bookingId);
        if (booking == null) {
            return false;
        }
        availability.put(booking.roomType, availability.get(booking.roomType) + 1);
        return true;
    }

    @Override
    public synchronized String getBooking(int bookingId) {
        Booking booking = bookings.get(bookingId);
        if (booking == null) {
            return "Booking not found";
        }
        return booking.toString();
    }

    public static void main(String[] args) {
        try {
            HotelServer server = new HotelServer();
            Registry registry = LocateRegistry.createRegistry(1099);
            registry.rebind("HotelBooking", server);
            System.out.println("Hotel RMI server ready on port 1099");
        } catch (Exception e) {
            System.out.println("ERROR: " + e.getMessage());
        }
    }
}

/*
 * Steps to run:
 * 1) javac HotelBooking.java HotelServer.java HotelClient.java
 * 2) java HotelServer (keep this running), then in another terminal run: java
 * HotelClient
 * 
 * Input examples (from client):
 * 1) Book: Name=Asha, Room=DELUXE, Nights=2
 * 2) Book: Name=Ravi, Room=SINGLE, Nights=1
 * 
 * Expected output (client side):
 * 1) Booking successful, ID=1001
 * 2) Booking successful, ID=1002
 */
