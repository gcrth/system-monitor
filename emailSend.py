# usage in C: system("python emailSend.py");

import smtplib
import time
from email.mime.text import MIMEText
from email.header import Header

# systemmonitortest@163.com
# systemmonitor
# systemMonitor123

inputFile = 'emailIn.txt'


class Email:
    def __init__(self):
        self.HOST = 'smtp.163.com'
        self.PORT = '465'
        self.FROM = 'systemmonitortest@163.com'

        self.TO = ['systemmonitortest@163.com']

        self.smtp_obj = smtplib.SMTP_SSL(self.HOST, self.PORT)
        res = self.smtp_obj.login(user=self.FROM, password='systemMonitor123')
        print(res, '登录成功')

    def send(self, msg):
        message = MIMEText(msg, 'plain', 'utf-8')
        message['From'] = self.FROM
        message['To'] = self.TO[0]
        message['Subject'] = Header('进程状态提醒', 'utf-8')

        try:
            self.smtp_obj.sendmail(self.FROM, self.TO, message.as_string())
            print('发送成功')
        except Exception:
            print('发送失败')


if __name__ == "__main__":
    f = open(inputFile, 'r', encoding='UTF-8')
    msg = f.read()
    f.close()
    email = Email()
    email.send(msg)
