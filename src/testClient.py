import socket
import sys

SIZERCV = 1024

if __name__ == "__main__":
    # We initialice our variables
    serverIP = "127.0.0.1"
    try:
        serverPort = 5000 if len(sys.argv) >= 1 else sys.argv[1]
    except ValueError:
        serverPort = 5000

    # Create a TCP/IP socket
    print ("Creating our socket.")
    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    print ("Connecting to \nIP:", serverIP, "\nPort:", serverPort)
    server_address = (serverIP, serverPort)
    clientSocket.connect(server_address)

    # Send data
    message = b"Hi socket!"
    print ("Sending: " + message)
    clientSocket.send(message)

    response = clientSocket.recv(SIZERCV)
    print ("Response:", response)

    print ("Closing socket.")
    clientSocket.close()