from socket import *

serverName = '127.0.0.1'
serverPort = 8888
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))
# python3 改动
sentence = input("Input lowercase sentence:")
# python3 改动 socket要求byte类型数据
clientSocket.send(sentence.encode())
modifiedSentence = clientSocket.recv(1024)
# python3 改动
print("From Server:" + modifiedSentence.decode())
clientSocket.close()