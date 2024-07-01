import serial, threading
from struct import *

ser = serial.Serial("COM5", 9600)

while True:

        
        tmp = str(ser.readline())[2:-5]
        r_border = str.find(tmp,'>')
        type = tmp[1:r_border]
        value = tmp[r_border+1:]
        print(type)
        print(value)

