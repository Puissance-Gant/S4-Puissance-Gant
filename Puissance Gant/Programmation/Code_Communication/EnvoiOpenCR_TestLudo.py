import serial
import time

startMarker = '<'
endMarker = '>'
dataStarted = False
dataBuf = ""
messageComplete = False

# anciennes positions envoyées aux moteurs
anciennePosA = int(0)
anciennePosB = int(0)
anciennePosC = int(0)
anciennePosD = int(0)
anciennePosE = int(0)
CHANGEMENT_MIN = 5

#========================

def setupSerialOpenCR(baudRate, serialPortName):
    
    global  serialPortOpenCR
    
    serialPortOpenCR = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)

    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))


#========================

def sendToOpenCR(msg):
    
        # this adds the start- and end-markers before sending
    global startMarker, endMarker, serialPortOpenCR

    # Si le message à envoyer contient des positions de moteurs, on vérifie qu'il y a un assez grand mouvement 
    # Cela permet d'optimiser la taille des messages à envoyer
    if any(c in msg for c in 'ABCD'):
        msg = verifierPosMoteurs(msg)
        print(msg)
        #print("stringToSend = " + stringToSend)

    serialPortOpenCR.write(msg.encode('utf-8')) # encode needed for Python3
    #print("taille du string : " + str(len(stringWithMarkers.encode('utf-8'))))

#========================

# Vérifier si les moteurs à envoyer ont beaucoup bougé ou non et envoyer seulement ceux qui ont assez bougé
def verifierPosMoteurs(message):
    global anciennePosA, anciennePosB, anciennePosC, anciennePosD, anciennePosE, endMarker
    valeur = ""
    msgFiltre = ""
    msgDepart = False
    try:
        for char in message:
            if msgDepart:
                match char:
                    case 'A':
                        if abs(int(valeur) - anciennePosA) > CHANGEMENT_MIN:
                            msgFiltre = msgFiltre + valeur + 'A'
                            anciennePosA = int(valeur)
                        valeur = ""
                    case 'B':
                        if abs(int(valeur) - anciennePosB) > CHANGEMENT_MIN:
                            msgFiltre = msgFiltre + valeur + 'B'
                            anciennePosB = int(valeur)
                        valeur = ""
                    case 'C':
                        if abs(int(valeur) - anciennePosC) > CHANGEMENT_MIN:
                            anciennePosC = int(valeur)
                            msgFiltre = msgFiltre + valeur + 'C'
                        valeur = ""
                    case 'D':
                        if abs(int(valeur) - anciennePosD) > CHANGEMENT_MIN:
                            anciennePosD = int(valeur)
                            msgFiltre = msgFiltre + valeur + 'D'
                        valeur = ""
                    case '>':
                        return str('<' + msgFiltre + '>')
                    case _:
                        valeur = valeur + char
                        #print(valeur)
            
            elif char == startMarker:
                msgDepart = True
    except Exception:
        return ""
    
    return msgFiltre
        

        

#========================

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


# setupSerialOpenCR(256000, "COM10")

# count = 0
# prevTime = time.time()
# newData = False

# valeur = 180
# increment = 10
# tempsDernierMsg = time.time()
# tempsChangement = time.time()
# while True:
#     # Envoyer un message à l'OpenCR
#     if time.time() - prevTime > 0.1:
#         newData = True
#         if valeur > (360-abs(increment)) or valeur < abs(increment):
#             increment = -increment
#             print("Changement de sens")
#             tempsChangement = time.time()
#             sendToOpenCR("F")
#         valeur = valeur + increment
#         strValeur = str(valeur)
#         sendToOpenCR(strValeur + "C" + strValeur + "D" + strValeur + "E")
#         #sendToOpenCR(strValeur + "E")
#         prevTime = time.time()


#     # Recevoir le message de l'OpenCR : faire régulièrement (~chaque 0.1s)
#     openCRReply = recvFromOpenCR()
#     if not (openCRReply == 'XXX'):
#         print ("openCr : " + openCRReply)
