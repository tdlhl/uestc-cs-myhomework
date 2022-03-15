import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.application import MIMEApplication
from email.header import Header

# Choose a mail server (e.g. Google mail server) and call it mailserver
mailserver ='smtp.qq.com'  #Fill-in-start  #Fill-in-end

# Sender and reciever
fromaddress ='2354664922@qq.com'  #Fill-in-start  #Fill-in-end
toaddress ='2019270102007@std.uestc.edu.cn'  #Fill-in-start  #Fill-in-end
password='agfeezwaxwaqebia'

message = MIMEMultipart()
message['From'] = Header("qq email", 'utf-8')
message['To'] = Header("std email", 'utf-8')
subject = '带附件的邮件'
message['Subject'] = Header(subject, 'utf-8')
message.attach(MIMEText('SMTP 发送带附件的邮件', 'plain', 'utf-8'))

# 发送当前目录下的Python学习须知.txt 文件
tfile = '附件.txt'
txtfile = MIMEApplication(open(tfile, 'rb').read())
txtfile.add_header('Content-Disposition','attachment',filename=tfile)
message.attach(txtfile)

try:
    server = smtplib.SMTP('smtp.qq.com')
    server.login(fromaddress, password)
    server.sendmail(fromaddress, toaddress, message.as_string())
    print("OK")
except smtplib.SMTPException:
    print("Wrong!!!")
