import socket

# Create TCP socket
client_socket = socket. socket (socket.AF_INET, socket. SOCK_STREAM)

# Connect to server
client_socket.connect (("127.0.0.1", 12345))

# Send message
client_socket.send (b"Hello from Client!")

# Receive response
data = client_socket.recv (1024) .decode ()
print ("Received from server:", data)

# Close connection
client_socket.close ()