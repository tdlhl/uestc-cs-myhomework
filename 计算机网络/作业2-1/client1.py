from socket import *
filename=input('输入文件名：')

#server_address = '127.0.0.1'
#server_port = 8888
#filename=test
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect(('127.0.0.1', 6666))
head = 'GET /'+filename+'.html HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n'
clientSocket.send(head.encode())
while True:
    data = clientSocket.recv(1024)
    print(data.decode(), end='')
clientSocket.close()
