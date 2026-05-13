public class Client_MPI {
    public static void main(String[] args) {
        try {
            Server_MPI.run(args);
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
