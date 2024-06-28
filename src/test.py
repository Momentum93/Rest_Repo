import serial, threading
from struct import *

ser = serial.Serial("COM10", 9600)
'''
ser = serial.Serial(
    port='COM10',\
    baudrate=115200,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
    timeout=None
    )
'''
    
def t_foo():
        while True:
            inp = "<test_type>fupl"
            ser.write(bytearray(inp,'ascii'))

##t1 = threading.Thread(target=t_foo, daemon=True)

#t1.start()


#b'\x00\x00\x00\xee\x07\x00\x00struct\x00\x00\x00\x00\x00\x00\n'
#b'\x00\x00\x00\xee\x07\x00\x00struct\x00\x00\x00\x00\x00\x00\n'
#b'\x00\x00\x00\xee\x07\x00\x00struct123\x00\x00\x00\n'
#b'\x00\x00\x00\xee\x07\x00\x00struct123\x00t\x00\x00\x00\x00\x00\x00\x00\x00\x00\n'
#b'\x00\x00\x00\xee\x07\x00\x00struct123\x00t\x00\x00\x00\x00\x00\x00\x00\x00\x00foo\x00\x00\x00\x00\x00\x00\x00\x00\x00\n'
#b'\x00\x00\x00\xee\x07\x00\x00struct123\x00t\x00\x00\x00\x00\x00\x00\x00\x00\x00\xe7\x03\x00\x00foo\x00\x00\x00\x00\x00\x00\x00\x00\x00\n'
#b'\x00\x00\x00\xee\x07\x00\x00struct123\x00t\x00\x00\x00\x00\x00\x00\x00\x00\x00\xe7\x03\x00\x00foo\x00\x00\x00\x00\x00\x00\x00\x00\x00\n'
#b'\x00\x00\x00\xee\x07\x00\x00\n'
while True:
        ser.write(bytearray("<test_type>fpl",'ascii'))
        tmp = ser.readline()
        print(tmp)
        continue
        tmp = str(ser.readline())[2:-5]
        r_border = str.find(tmp,'>')
        type = tmp[1:r_border]
        value = tmp[r_border+1:]
        print(type)
        print(value)



        continue
        print( ser.read_until(b"\n"))
        print(unpack('i',b'\xe7\x03\x00\x00'))
        print(unpack('ii',b'\n\x00\x00\x00\xee\x07\x00\x00'))
        continue
        if ser.read(1) == b'\n':
                i = ser.read(7)
                s = ser.read(9)
                #print(s.__sizeof__())
                print(unpack("ii", b'\n'+i)[0])
                #print(i)
                #print(s)
                print(unpack("10p", b'\n'+s)[0])
                continue
                #ser.read(7)
                
                size = calcsize('ii')
                #print(calcsize('s'))

                #print(ser.read(6))

                
                result = ser.read(size-1)
                #print(unpack("ii", result))

                convert = b'\n'+result

                unpacked = unpack("ii", convert)

                print(unpacked)
                
