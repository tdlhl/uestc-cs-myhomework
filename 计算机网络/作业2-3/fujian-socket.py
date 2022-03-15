from socket import *
import base64

# Mail content
subject = "I love computer networks!"
contenttype = "text/plain"
msg = "I love computer networks!"
endmsg = "\r\n.\r\n"
msgboundary=b'--BOUNDARY\r\n'

# Choose a mail server (e.g. Google mail server) and call it mailserver
mailserver ='smtp.qq.com'  #Fill-in-start  #Fill-in-end

# Sender and reciever
fromaddress ='2354664922@qq.com'  #Fill-in-start  #Fill-in-end
toaddress ='haolinli888@gmail.com'  #Fill-in-start  #Fill-in-end

# Auth information (Encode with base64)
#Fill-in-start  #Fill-in-end 使用encode（utf-8）先转化成bytes类型，用b64encode编码后再用decode转化成字符串类型
username =base64.b64encode(fromaddress.encode()).decode()
password =base64.b64encode('agfeezwaxwaqebia'.encode()).decode()  #agfeezwaxwaqebia   pckamlzlgmmqeaci

# Create socket called clientSocket and establish a TCP connection with mailserver
#Fill-in-start
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((mailserver, 25))
#Fill-in-end

recv = clientSocket.recv(1024) .decode()
print(recv)

# Send HELO command and print server response.
#Fill-in-start
scom='HELO Batman\r\n'
clientSocket.send(scom.encode())
response = clientSocket.recv(1024).decode()
print(response)
#Fill-in-end

# Send AUTH LOGIN command and print server response.
clientSocket.sendall('AUTH LOGIN\r\n'.encode())
recv = clientSocket.recv(1024).decode()
print(recv)

clientSocket.sendall((username + '\r\n').encode())
recv = clientSocket.recv(1024).decode()
print(recv)

clientSocket.sendall((password + '\r\n').encode())
recv = clientSocket.recv(1024).decode()
print(recv)

# Send MAIL FROM command and print server response.
#Fill-in-start
clientSocket.sendall(('MAIL FROM: <' + fromaddress + '>\r\n').encode())
response = clientSocket.recv(1024).decode()
print(response)
#Fill-in-end

# Send RCPT TO command and print server response.
#Fill-in-start
clientSocket.sendall(('RCPT TO: <' + toaddress + '>\r\n').encode())
response = clientSocket.recv(1024).decode()
print(response)
#Fill-in-end

# Send DATA command and print server response.
#Fill-in-start
clientSocket.send('DATA\r\n'.encode())
response = clientSocket.recv(1024).decode()
print(response)
#Fill-in-end

# Send message data.
#Fill-in-start
message = 'from:' + fromaddress + '\r\n'
message += 'to:' + toaddress + '\r\n'
message += 'subject:' + subject + '\r\n'
message += 'Content-Type:' + contenttype + '\t\n'
message += '\r\n' + msg
clientSocket.sendall(message.encode())
#Fill-in-end

msg = b'\r\n'
clientSocket.send(b'\r\n\r\n')#发送分割标志
clientSocket.send(msg)
fb=open('D:\\李昊霖\\编程\\计网作业\\2-3\\2-3运行结果报告.docx','rb')
while True:
    filedata=fb.read(1024)
    print(filedata)
    if not filedata:
        break
    clientSocket.send(base64.b64encode(filedata))
fb.close()

# Message ends with a single period and print server response.
#Fill-in-start
clientSocket.sendall(endmsg.encode())
response = clientSocket.recv(1024).decode()
print(response)
#Fill-in-end

# Send QUIT command and print server response.
#Fill-in-start
clientSocket.sendall('QUIT\r\n'.encode())
#Fill-in-end

# Close connection
clientSocket.close()
