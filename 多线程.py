from socket import *
import threading
def service(connectionSocket):
    try:
        message = connectionSocket.recv(1024)
        filename = message.split()[1]
        f = open(filename[1:], "rb")
        response = f.read().decode()
        f.close()
        response = 'HTTP/1.1 200 OK\r\n\r\n' + response
        for i in range(0, len(response)):
            connectionSocket.send(response[i].encode())
        connectionSocket.close()
        print("OK!")
    except IOError:
        response = 'HTTP/1.1 404 Not Found\r\n\r\n'
        for i in range(0, len(response)):
            connectionSocket.send(response[i].encode())
        connectionSocket.close()


serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(("127.0.0.1", 6666))
serverSocket.listen(9)
i=0
while True:
    print('The server is ready to receive ')
    print(i)
    i=i+1
    connectionSocket, addr = serverSocket.accept()
    thread = threading.Thread(target=service, args=(connectionSocket,))
    thread.start()
serverSocket.close()
