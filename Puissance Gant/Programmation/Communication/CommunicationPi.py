import serial
import time
import random

startMarker = '<'
endMarker = '>'
dataStarted = False
dataBuf = ""
messageComplete = False

#========================
#========================
    # the functions

def setupSerialOpenCR(baudRate, serialPortName):
    global  serialPortOpenCR
    serialPortOpenCR = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)
    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))

#def setupSerialESP32(baudRate, serialPortName):
#    
#    global  serialPortESP32
#    
#    serialPortESP32 = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)
#
#    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))

#========================

def sendToOpenCR(stringToSend):
    
        # this adds the start- and end-markers before sending
    global startMarker, endMarker, serialPortOpenCR
    
    stringWithMarkers = (startMarker)
    stringWithMarkers += stringToSend
    stringWithMarkers += (endMarker)

    serialPortOpenCR.write(stringWithMarkers.encode('utf-8')) # encode needed for Python3

#==================

def recvFromOpenCR():

    global startMarker, endMarker, serialPortOpenCR, dataStarted, dataBuf, messageComplete

    if serialPortOpenCR.inWaiting() > 0 and messageComplete == False:
        x = serialPortOpenCR.read().decode("utf-8") # decode needed for Python3
        
        if dataStarted == True:
            if x != endMarker:
                dataBuf = dataBuf + x
            else:
                dataStarted = False
                messageComplete = True
        elif x == startMarker:
            dataBuf = ''
            dataStarted = True
    
    if (messageComplete):
        messageComplete = False
        return dataBuf
    else:
        return "XXX" 
#====================
#====================
    # the program


setupSerialOpenCR(115200, "COM7")
prevTime = time.time()
newData = False
