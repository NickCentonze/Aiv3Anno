import socket

s= socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

s.sendto(b'Hello from Nicolo Centonze', ('93.42.108.124', 9999))