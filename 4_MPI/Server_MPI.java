import java.io.BufferedReader;
import java.io.InputStreamReader;

import mpi.MPI;

public class Server_MPI {
    public static void run(String[] args) throws Exception {
        MPI.Init(args);

        int rank = MPI.COMM_WORLD.Rank();
        int size = MPI.COMM_WORLD.Size();

        if (size < 2) {
            if (rank == 0) {
                System.out.println("Run with at least 2 processes.");
            }
            MPI.Finalize();
            return;
        }

        if (rank == 0) {
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            System.out.print("Enter first integer: ");
            int a = Integer.parseInt(reader.readLine());
            System.out.print("Enter second integer: ");
            int b = Integer.parseInt(reader.readLine());

            int[] payload = new int[] { a, b };
            MPI.COMM_WORLD.Send(payload, 0, 2, MPI.INT, 1, 100);

            int[] result = new int[1];
            MPI.COMM_WORLD.Recv(result, 0, 1, MPI.INT, 1, 200);

            System.out.println("Sum from worker: " + result[0]);
        } else if (rank == 1) {
            int[] payload = new int[2];
            MPI.COMM_WORLD.Recv(payload, 0, 2, MPI.INT, 0, 100);

            int[] result = new int[] { payload[0] + payload[1] };
            MPI.COMM_WORLD.Send(result, 0, 1, MPI.INT, 0, 200);
        }

        MPI.Finalize();
    }

    public static void main(String[] args) {
        try {
            run(args);
        } catch (Exception e) {
            System.out.println("ERROR: " + e.getMessage());
        }
    }
}

/*
 * Steps to run (MPJ Express example):
 * 1) javac -cp .;path\\to\\mpj\\lib\\mpj.jar Server_MPI.java Client_MPI.java
 * 2) mpjrun.bat -np 2 Server_MPI
 * 
 * Input examples:
 * 1) 10 and 20 -> output 30
 * 2) 7 and 5 -> output 12
 * 
 * Expected output:
 * 1) Sum from worker: 30
 * 2) Sum from worker: 12
 */
