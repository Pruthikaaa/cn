import socket
import signal
import sys

# Handle Ctrl+C signal (SIGINT)
def handle_signal(sig, frame):
print ("\nServer shutting down ... ")
sys.exit (0)

signal.signal(signal.SIGINT, handle_signal)

# Create TCP socket
server socket = socket. socket (socket. AF_INET, socket. SOCK_STREAM)
server_socket.bind (("127.0.0.1", 12345) ) # bind to localhost and port
server_socket.listen (1)

print ("Server waiting for connection ... ")

# Accept connection
conn, addr = server_socket.accept ()
print ("Connected by", addr)

# Receive message
data = conn.recv (1024) .decode ()
print ("Received from client:", data)

# Send response
conn.send (b"Hello from Server!")

# Close connection
conn.close ()
server socket. close ()