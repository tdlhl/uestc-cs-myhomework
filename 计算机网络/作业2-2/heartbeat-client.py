from socket import *
import time
server_address='127.0.0.1'
server_port=6666
clientSocket=socket(AF_INET,SOCK_DGRAM)
for i in range(1,11):
    stime = time.time()
    message=str(i)+' '+str(time.time())
    clientSocket.sendto(message.encode(),(server_address,server_port))

clientSocket.close()