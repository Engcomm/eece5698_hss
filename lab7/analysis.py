#!/usr/bin python

import sys
import socket
import time
import struct

# SERVER_IP = '10.75.8.54'
SERVER_IP = '127.0.0.1'
# SERVER_PORT = 9907
SERVER_PORT = 3334 


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((SERVER_IP, SERVER_PORT))


msg = struct.pack('HH', 1, 1025)
s.send(msg)
s.send('A'*1025)

msg = s.recv(1025)
s.close()
print(msg)
