import java.io.BufferedReader;
import java.io.InputStreamReader;

import mpi.MPI;

public class DistributedSumMPI {

        public static void main(String[] args) throws Exception {

                MPI.Init(args);

                int rank = MPI.COMM_WORLD.Rank();

                if (rank == 0) {

                        int a = 10;
                        int b = 20;

                        System.out.println("First Number = " + a);
                        System.out.println("Second Number = " + b);

                        int[] data = { a, b };

                        MPI.COMM_WORLD.Send(data, 0, 2,
                                        MPI.INT, 1, 100);

                        int[] result = new int[1];

                        MPI.COMM_WORLD.Recv(result, 0, 1,
                                        MPI.INT, 1, 200);

                        System.out.println("Sum received from worker = "
                                        + result[0]);

                } else if (rank == 1) {

                        int[] data = new int[2];

                        MPI.COMM_WORLD.Recv(data, 0, 2,
                                        MPI.INT, 0, 100);

                        int sum = data[0] + data[1];

                        int[] result = { sum };

                        MPI.COMM_WORLD.Send(result, 0, 1,
                                        MPI.INT, 0, 200);
                }

                MPI.Finalize();
        }
}

/*
 * Steps to run (MPJ Express example) run in powershell :
 * Already MPJ .zip is in the repo folder 4_MPI , just extract it to any
 * folder,
 * My : D:soft/
 * 1) $env:MPJ_HOME="D:\soft\mpj-v0_44"
 * 2) javac -cp ".;D:\soft\mpj-v0_44\lib\mpj.jar" DistributedSumMPI.java
 * 3) D:\soft\mpj-v0_44\bin\mpjrun.bat -np 2 DistributedSumMPI
 * 
 * 
 * Expected output:
 * MPJ Express (0.44) is started in the multicore configuration
 * First Number = 10
 * Second Number = 20
 * Sum received from worker = 30
 */
