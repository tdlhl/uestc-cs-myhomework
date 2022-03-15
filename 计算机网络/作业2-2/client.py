from socket import *
import sys
import time

server_address=sys.argv[1]
server_port=int(sys.argv[2])
#server_address,server_port=input('输入地址 端口：').split()
#server_port=int(server_port)
#server_address = '127.0.0.1'
#server_port = 9987
clientSocket = socket(AF_INET, SOCK_DGRAM)
totalrtt=0
rttmax=0
rttmin=0
lost=0
youx=10
clientSocket.settimeout(1)
for i in range(1, 11):
    stime = time.time()
    otime =time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(stime))
    message='Ping %d ' % (i)
    message+=otime
    message =message.encode()
    try:
        clientSocket.sendto(message, (server_address, server_port))
        response, serverAddress = clientSocket.recvfrom(1024)
        rtt=time.time()-stime
        rtt=rtt*1000
        totalrtt+=rtt
        if rtt>rttmax:
            rttmax=rtt
        if rtt<rttmin:
            rttmin=rtt
        #output="Reply from"+server_address+':'+response+',RTT='+rtt+'ms'
        print('Reply from %s: %s,RTT=%.3f ms'%(server_address,response,rtt))


    except Exception as e:
        lost=lost+1
        youx=youx-1
        print('Request timed out')
if youx:
    artt=totalrtt/youx
    lose=lost*10
    print("丢包率:%d%% ，平均RTT=%.3f ms, 最大RTT=%.3f ms, 最小RTT=%.3f ms"%(lose,artt,rttmax,rttmin))
else:
    print("丢包率:100%")
clientSocket.close()
