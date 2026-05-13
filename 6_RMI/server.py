import socket 
import threading 

 

# Server class to handle client connections 
class ChatServer: 
    def __init__(self, host='127.0.0.1', port=65432): 
        self.host = host 
        self.port = port 
        self.server_socket = None 
        self.clients = []  # List to keep track of connected clients 
        self.client_addresses = {}  # Dictionary to store client address and socket 

 

    def start(self): 
        # Create a TCP/IP socket 
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
        self.server_socket.bind((self.host, self.port)) 
        self.server_socket.listen(5)  # Max number of queued connections 
        print(f"Server started on {self.host}:{self.port}") 

 

        # Accept incoming connections 
        while True: 
            client_socket, client_address = self.server_socket.accept() 
            print(f"New connection from {client_address}") 
            self.clients.append(client_socket) 
            self.client_addresses[client_socket] = client_address 

 

            # Start a new thread to handle the client 
            client_thread = threading.Thread(target=self.handle_client, args=(client_socket,)) 
            client_thread.start() 

 

    def handle_client(self, client_socket): 
        # Handle communication with a single client 
        try: 
            client_socket.send("Welcome to the chat server!".encode('utf-8')) 

 

            while True: 
                # Receive message from the client 
                message = client_socket.recv(1024).decode('utf-8') 
                if not message: 
                    break  # Client disconnected 

 

                # Broadcast the message to all connected clients 
                print(f"Message from {self.client_addresses[client_socket]}: {message}") 
                self.broadcast_message(message, client_socket) 


        except Exception as e: 
            print(f"Error handling client {self.client_addresses.get(client_socket)}: {e}") 
        finally: 
            # Cleanup when client disconnects 
            self.clients.remove(client_socket) 
            del self.client_addresses[client_socket] 
            client_socket.close() 

 

    def broadcast_message(self, message, sender_socket): 
        # Broadcast message to all clients except the sender 
        for client_socket in self.clients: 
            if client_socket != sender_socket: 
                try: 
                    client_socket.send(message.encode('utf-8')) 
                except Exception as e: 
                    print(f"Error broadcasting to {client_socket}: {e}") 

 

 

if __name__ == "__main__": 
    chat_server = ChatServer() 
    chat_server.start() 