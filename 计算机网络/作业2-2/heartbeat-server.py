from socket import *
import time
serverSocket = socket(AF_INET, SOCK_DGRAM)
server_address = '127.0.0.1'
server_port = 6666
serverSocket.bind((server_address,server_port))
stime = float(time.time())
etime = stime
print("the server is ready to receive")
while True:
    try:
        serverSocket.settimeout(0.1)
        message, address = serverSocket.recvfrom(1024)
        message = message.decode()
        rtime = float(message.split()[1])
        etime = rtime
        ping = float(time.time()) - rtime
        print(str(message.split()[0]) + ':', ping)
    except Exception as e:
        if etime == stime:
            continue
        if time.time() - etime >= 1.0:
            print('Heartbeat pause')
            break
        else:
            print('Packet lost')