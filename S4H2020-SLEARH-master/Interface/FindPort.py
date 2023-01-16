import sys
import glob
import serial
import serial.tools.list_ports
import json
import time

def serial_ports():

    if sys.platform.startswith('win'):
        ports = list(serial.tools.list_ports.comports())


    elif sys.platform.startswith('linux'):
        ports = list(serial.tools.list_ports.comports())
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        j = 0
        list2 = []
        for i in port:
            list2.insert(j,i)

            j+=1
        if sys.platform.startswith('win'):
            if "Arduino" in port[1] or "USB" in port[1]:
                result.append(port[0])
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            #print(port[1])
            if "ACM0" in port[1] or "AMA0" in port[1]:
                result.append(port[0])
        else:
            raise EnvironmentError('Unsupported platform')
    return result


def SendStrToOpenCR(port,msg):
    
    ser = serial.Serial("/dev/ttyACM0", 57600, timeout=1)
    #ser = serial.Serial(port, 57600, timeout=1)
    ser.flush()
    line = ""

    ser.write(msg.encode('utf-8'))
    while line != "IM DONE" :
        #print(ser)
        line = ser.readline().decode('utf-8').rstrip()
        #print("patate")
        print(line)
        sys.stdout.flush()
        time.sleep(1)



if __name__ == '__main__':
    list = serial_ports()
    Str_port = list[0]
    #print(Str_port)
    string = input("Quel est le string?:\t")
    

    SendStrToOpenCR(Str_port,string)
