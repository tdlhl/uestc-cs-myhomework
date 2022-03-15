from socket import *
import threading
import os
import sys


# Define thread process
def Server(tcpCliSock, addr):
    BUFSIZE = 1024
    print('Received a connection from:', addr)
    data = tcpCliSock.recv(BUFSIZE)
    data = data.decode()
    print(data)

    if len(data):
        # Extract the filename from the received message
        getFile = data.split()[1]
        print('getFile:', getFile)

        # Form a legal filename
        filename = getFile.partition("/")[2]
        print('filename:', filename)

        # Check whether the file exist in the cache
        if os.path.exists(filename):
            print('File exist')
            # ProxyServer finds a cache hit and generates a response message
            f = open(filename, "r")
            outputdata = f.read()
            CACHE_PAGE = f.read()
            # ProxyServer sends the cache to the client
            tcpCliSock.send("HTTP/1.1 200 OK\r\n".encode())
            tcpCliSock.send("Content-Type:text/html\r\n\r\n".encode())
            tcpCliSock.send(outputdata)
            # print('Read from cache')
            print('Send the cache to the client')
            tcpClisock.close()
        else:
            print('File not exist')
            # Handling for file not found in cache
            # Create a socket on the ProxyServer
            c = socket(AF_INET, SOCK_STREAM)
            try:
                # Connect to the WebServer socket to port 80
                hostn = getFile.partition("/")[2].partition("/")[0]
                serverName = hostn.partition("/")[0]
                serverPort = 80
                print((serverName, serverPort))
                c.connect((serverName, serverPort))
                askFile = ''.join(filename.partition('/')[1:])
                # print("askFile:", askFile)
                print('Connect to', hostn)

                # Some information in client request must be replaced before it can be sent to the server
                url = ("GET ".encode() + askFile.encode() + " HTTP/1.1\r\nHost: ".encode() + serverName.encode() +
                         "\r\n\r\n".encode())
                # fileobj = c.makefile('rwb', 0) fileobj.write("GET ".encode() + askFile.encode() + "
                # HTTP/1.0\r\nHost: ".encode() + serverName.encode() + "\r\n\r\n".encode())

                # Send the modified client request to the server
                c.send(url)

                # Read the response into buffer
                buff = c.recv(4096)
                print('recvbuff len:', len(buff))

                # Send the response in the buffer to client socket
                tcpClisock.send(buff)
                print('Send to client\r\n')
                # Create a new file to save the response in the cache for the requested file
                tmpFile = open("./" + filename, "w")
                for line in buff:
                    tmpFile.write(line);
                    tcpCliSock.send(line);
            except:
                print("Illegal request")
            tcpClisock.close()


# Main process of  ProxyServer
if __name__ == '__main__':

    # Create a server socket, bind it to a port and start listening
    tcpSersock = socket(AF_INET, SOCK_STREAM)
    tcpSerPort = 80
    # tcpSersock.bind("127.0.0.1", tcpSerPort)
    # tcpSerPort = int(sys.argv[2])
    tcpSersock.bind((sys.argv[1], tcpSerPort))
    # print(tcpSerPort)
    tcpSersock.listen(10)

    print("Ready to serve......\n")
    while True:
        tcpClisock, addr = tcpSersock.accept()
        thread = threading.Thread(target=Server, args=(tcpClisock, addr))
        thread.start()
    tcpSersock.close()
