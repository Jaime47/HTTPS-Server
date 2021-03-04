from socket import *

serverName = 'servername'
serverPort = 12000
clientSocket = socket(socket.AF_INET,socket.SOCK_STREAM)
clientSocket.connect((serverName,serverPort))
sentence = raw_imput('Ingrese su texto en minusculas')
clientSocket.send(sentence)
modifiedSentence = clientSocket.recv(1024)
print('Desde el servidor: ', modifiedSentence)
socket.close()