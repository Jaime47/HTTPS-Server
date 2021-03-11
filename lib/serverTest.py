from socket import *;

serverName = "clienTest"
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName,serverPort))
sentence = input('Esto es un super mensaje de prueba')
for _ in range(5000):
    clientSocket.send(sentence)
    modSentence = clientSocket.recv(1024)
clientSocket.close()