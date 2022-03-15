from socket import *
import os
import sys, getopt
import struct
import time
import select
import binascii

ICMP_ECHO_REQUEST = 8


def checksum(string):
    csum = 0
    countTo = (len(string) // 2) * 2  #除二向下取整再乘2，取接近的偶数？
    count = 0

    while count < countTo:
        thisVal = string[count] * 256 + string[count + 1]  #以二进制形式，count左移8位，右边八位并count+1(下八位）
        csum = csum + thisVal
        csum = csum & 0xffffffff      #与16位全1做逻辑与    不还是自己吗？
        count = count + 2

    if countTo < len(string):
        csum = csum + string[len(string) - 1]
        csum = csum & 0xffffffff

    csum = (csum >> 16) + (csum & 0xffff)
    csum = csum + (csum >> 16)
    answer = ~csum
    answer = answer & 0xffff
    answer = answer >> 8 | (answer << 8 & 0xff00)
    return answer


def receiveOnePong(mySocket, destAddr, ID, sequence, timeout):
    timeLeft = timeout

    while 1:
        startedSelect = time.time()
        whatReady = select.select([mySocket], [], [], timeLeft)
        #select.select(rlist, wlist, xlist[, timeout]) select函数阻塞程序运行，传递三个参数，一个为输入而观察的文件对象列表，
        # 一个为输出而观察的文件对象列表和一个观察错误异常的文件列表。
        # 第四个是一个可选参数，表示超时秒数。其返回3个tuple，每个tuple都是一个准备好的对象列表，它和前边的参数是一样的顺序。
        howLongInSelect = (time.time() - startedSelect)
        if whatReady[0] == []:  # Timeout
            return None
        timeReceived = time.time()
        recPacket, addr = mySocket.recvfrom(1024)

        # Fill in start
        header = recPacket[20: 28]
        type, code, checksum, packetID, sequence = struct.unpack("!BBHHH", header)
        if type == 0 and packetID == ID:
            byte_in_double = struct.calcsize("!d")
            timesent = struct.unpack("!d", recPacket[28: 28 + byte_in_double])[0]
            delay = timeReceived - timesent
            ttl = ord(struct.unpack("!c", recPacket[8:9])[0])
            return (delay, ttl, byte_in_double)
        # Fetch the ICMP header from the IP packet

        # Fill in end

        timeLeft = timeLeft - howLongInSelect
        if timeLeft <= 0:
            return None


def sendOnePing(mySocket, destAddr, ID, sequence):
    # Header is type (8), code (8), checksum (16), id (16), sequence (16)

    myChecksum = 0
    # Make a dummy header with a 0 checksum
    # struct -- Interpret strings as packed binary data
    header = struct.pack("!BBHHH", ICMP_ECHO_REQUEST, 0, myChecksum, ID, sequence)
    data = struct.pack("!d", time.time())

    # Calculate the checksum on the data and the dummy header.
    myChecksum = checksum(header + data)

    # Get the right checksum, and put in the header
    if sys.platform == 'darwin':                 #Mac OS
        # Convert 16-bit integers from host to network byte order
        myChecksum = htons(myChecksum) & 0xffff
    else:
        myChecksum = htons(myChecksum)           #把本机字节顺序转化为网络字节顺序

    header = struct.pack("!BBHHH", ICMP_ECHO_REQUEST, 0, myChecksum, ID, sequence)
    packet = header + data

    mySocket.sendto(packet, (destAddr, 1))  # AF_INET address must be tuple, not str
    # Both LISTS and TUPLES consist of a number of objects
    # which can be referenced by their position number within the object.


def doOnePing(destAddr, ID, sequence, timeout):
    icmp = getprotobyname("icmp")

    # SOCK_RAW is a powerful socket type. For more details:
    # http://sock-raw.org/papers/sock_raw
    # Fill in start
    mySocket = socket(AF_INET, SOCK_RAW, icmp)
    # Create Socket here

    # Fill in end

    sendOnePing(mySocket, destAddr, ID, sequence)
    rtt = receiveOnePong(mySocket, destAddr, ID, sequence, timeout)

    mySocket.close()
    return rtt


def ping(dest, count):
    # timeout=1 means: If one second goes by without a reply from the server,
    # the client assumes that either the client's ping or the server's pong is lost
    timeout = 1

    myID = os.getpid() & 0xFFFF  # Return the current process i
    loss = 0

    # Send ping requests to a server separated by approximately one second
    artt=0
    minrtt=0
    maxrtt=0
    for i in range(count):
        result = doOnePing(dest, myID, i, timeout)

        # Fill in start

        # Print response information of each pong packet:
        # No pong packet, then display "Request timed out."
        # Receive pong packet, then display "Reply from host_ipaddr : bytes=… time=… TTL=…"
        if not result:
            print("Request timed out.")
            loss =loss+ 1
        else:
            delay = int(result[0] * 1000)
            if i==0:
                minrtt=delay
                maxrtt=delay
            artt=artt+delay
            if minrtt>delay:
                minrtt=delay
            if maxrtt<delay:
                maxrtt=delay
            ttl = result[1]
            bytes = result[2]
            print("Reply from " + dest + ": bytes=" + str(bytes) + " time=" + str(delay) + "ms TTL=" + str(ttl))
        # Fill in end

        time.sleep(1)  # one second

    # Fill in start
    losspersent=100*loss/count
    if count==loss:
        artt=0
    else:
        artt=artt/(count-loss)
    # Print Ping statistics
    print("Ping: sent = " + str(count) + " received = " + str((count - loss)) + " loss = " + str(loss)+' losspersent='+str(losspersent)+'%')
    print('minrtt='+str(minrtt)+'ms maxrtt='+str(maxrtt)+'ms averagertt='+str(artt)+'ms')
    # Fill in end

    return


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print('IcmpPing.py dest_host [-n <count>]')
        sys.exit()
    host = sys.argv[1]
    try:
        dest = gethostbyname(host)
    except:
        print('Can not find the host "%s". Please check your input, then try again.' % (host))
        exit()

    count = 4
    try:
        opts, args = getopt.getopt(sys.argv[2:], "n:")
    except getopt.GetoptError:
        print('IcmpPing.py dest_host [-n <count>]')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-n':
            count = int(arg)

    print("Pinging " + host + " [" + dest + "] using Python:")
    print("")
    ping(dest, count)
