#!/usr/bin/python
import smtplib, ssl
import sys



server = smtplib.SMTP('smtp.gmail.com', 587)
server.ehlo()  # Can be omitted
server.starttls()
server.ehlo()  # Can be omittedI
sender_email = "satyamehta96@gmail.com"
receiver_email = "satyamehta1996@gmail.com"
password = 'viratkohli99'
message = """\
Subject: Hi there

This message is sent from Python."""

if(sys.argv[1] == "satya"):
        server.login(sender_email, password)
        server.sendmail(sender_email, receiver_email, message)
        server.quit()
        print("Email sent to destination")
else:
    print("Email sent failed")