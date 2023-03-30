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


#========================

def sendToOpenCR(stringToSend):
    
        # this adds the start- and end-markers before sending
    global startMarker, endMarker, serialPortOpenCR
    
    stringWithMarkers = (startMarker)
    stringWithMarkers += stringToSend
    stringWithMarkers += (endMarker)
    serialPortOpenCR.write(stringWithMarkers.encode('utf-8')) # encode needed for Python3
    #rint("taille du string : " + str(len(stringWithMarkers.encode('utf-8'))))

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


setupSerialOpenCR(256000, "COM10")

count = 0
prevTime = time.time()
newData = False

valeur = 180
increment = 10
tempsDernierMsg = time.time()
tempsChangement = time.time()
while True:
    if time.time() - prevTime > 0.1:
        newData = True
        if valeur > (360-abs(increment)) or valeur < abs(increment):
            increment = -increment
            print("Changement de sens")
            tempsChangement = time.time()
            sendToOpenCR("F")
        valeur = valeur + increment
        strValeur = str(valeur)
        sendToOpenCR(strValeur + "C" + strValeur + "D" + strValeur + "E")
        #sendToOpenCR(strValeur + "E")
        prevTime = time.time()
    openCRReply = recvFromOpenCR()
    if not (openCRReply == 'XXX'):
        print ("openCr : " + openCRReply)
        if(openCRReply == "change"):
            print(time.time() - tempsChangement)
        
        
        #count += 1