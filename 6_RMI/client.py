import socket 
import threading 

 

# Client class to interact with the chat server 
class ChatClient: 
    def __init__(self, host='127.0.0.1', port=65432): 
        self.host = host 
        self.port = port 
        self.client_socket = None 

 

    def start(self): 
        # Create a TCP/IP socket and connect to the server 
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
        self.client_socket.connect((self.host, self.port)) 
        print(f"Connected to chat server at {self.host}:{self.port}") 

 

        # Start a thread to listen for incoming messages 
        receive_thread = threading.Thread(target=self.receive_messages) 
        receive_thread.daemon = True 
        receive_thread.start() 


 

        # Main thread will handle sending messages 
        self.send_messages() 

 

    def receive_messages(self): 
        # Continuously listen for messages from the server 
        while True: 
            message = self.client_socket.recv(1024).decode('utf-8') 
            if message: 
                print(f"\n{message}") 

 

    def send_messages(self): 
        # Continuously prompt the user to send messages 
        while True: 
            message = input("> ") 
            if message: 
                self.client_socket.send(message.encode('utf-8')) 

 

 

if __name__ == "__main__": 
    chat_client = ChatClient() 
    chat_client.start() 

 

 