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

def setupSerialESP32(baudRate, serialPortName):
    
    global  serialPortESP32
    
    serialPortESP32 = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)

    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))

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

def recvFromESP32():

    global startMarker, endMarker, serialPortESP32, dataStarted, dataBuf, messageComplete

    if serialPortESP32.inWaiting() > 0 and messageComplete == False:
        x = serialPortESP32.read().decode("utf-8") # decode needed for Python3
        
        if dataStarted == True:
            if x != endMarker:
                dataBuf = dataBuf + x
            else:
                dataStarted = False
                messageComplete = True
        elif x == startMarker:
            dataBuf = ''
            dataStarted = True
    
    # if (messageComplete):
    #     messageComplete = False
    return dataBuf
    # else:
    #     return "XXX" 
#====================
#====================
    # the program


setupSerialOpenCR(115200, "COM7")
setupSerialESP32(115200, "COM5")
count = 0
prevTime = time.time()
newData = False
while True:
        # check for a reply
    
    
    ESP32Reply = recvFromESP32()
    if not ESP32Reply == 'XXX' and newData:
        print("ESP32 + " + ESP32Reply)
        newData = False
        # send a message at intervals
    if time.time() - prevTime > 0.5:
        newData = True
        #message0 = random.randint(0,359)
        #message1 = random.randint(0,359)
        #message2 = random.randint(0,359)
        #message3 = random.randint(0,359)
        #message4 = random.randint(0,359)
        #sendToOpenCR(str(message0) + "A" + str(message1) + "B" + str(message2) + "C" + str(message3) + "D" + str(message4) + "E")
        sendToOpenCR(ESP32Reply)
        
        openCRReply = recvFromOpenCR()
        #if not (openCRReply == 'XXX'):
        print ("OpenCR + " + openCRReply)
        prevTime = time.time()
        count += 1