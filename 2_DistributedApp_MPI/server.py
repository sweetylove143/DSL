# server.py

from xmlrpc.server import SimpleXMLRPCServer


# Function to reverse string
def reverse_str(s1):
    ch = list(s1)
    rev = ""

    # Loop from last to first (same as Java logic)
    for i in range(len(ch) - 1, -1, -1):
        rev += ch[i]

    return rev


# Create server
server = SimpleXMLRPCServer(("localhost", 1030))

print("Initializing Server...")
print("Server Ready on port 1030")


# Register remote function
server.register_function(reverse_str, "input")


# Keep server running
server.serve_forever()
