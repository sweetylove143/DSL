import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class HotelClient {
    public static void main(String[] args) {
        try {
            Registry registry = LocateRegistry.getRegistry("localhost", 1099);
            HotelBooking booking = (HotelBooking) registry.lookup("HotelBooking");

            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            int choice;

            do {
                System.out.println("\n*** Hotel Booking Menu ***");
                System.out.println("1. View availability");
                System.out.println("2. Book a room");
                System.out.println("3. View booking by ID");
                System.out.println("4. Cancel booking");
                System.out.println("0. Exit");
                System.out.print("Enter your choice: ");

                choice = Integer.parseInt(reader.readLine());

                switch (choice) {
                    case 1:
                        System.out.println("Availability: " + booking.getAvailability());
                        break;
                    case 2:
                        System.out.print("Guest name: ");
                        String guest = reader.readLine();
                        System.out.print("Room type (SINGLE/DOUBLE/DELUXE): ");
                        String roomType = reader.readLine().toUpperCase();
                        System.out.print("Nights: ");
                        int nights = Integer.parseInt(reader.readLine());

                        int bookingId = booking.bookRoom(guest, roomType, nights);
                        if (bookingId > 0) {
                            System.out.println("Booking successful, ID=" + bookingId);
                        } else {
                            System.out.println("Booking failed (room unavailable or invalid input)");
                        }
                        break;
                    case 3:
                        System.out.print("Booking ID: ");
                        int id = Integer.parseInt(reader.readLine());
                        System.out.println(booking.getBooking(id));
                        break;
                    case 4:
                        System.out.print("Booking ID: ");
                        int cancelId = Integer.parseInt(reader.readLine());
                        boolean cancelled = booking.cancelBooking(cancelId);
                        System.out.println(cancelled ? "Booking cancelled" : "Booking not found");
                        break;
                    case 0:
                        System.out.println("Exiting client...");
                        break;
                    default:
                        System.out.println("Invalid choice");
                        break;
                }
            } while (choice != 0);
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
 * Input examples:
 * 1) Book: Name=Asha, Room=DELUXE, Nights=2 -> Booking ID=1001
 * 2) View booking: ID=1001 -> "Booking 1001: Asha - DELUXE for 2 night(s)"
 * 
 * Expected output:
 * 1) Booking successful, ID=1001
 * 2) Booking 1001: Asha - DELUXE for 2 night(s)
 */
