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
CHANGEMENT_MIN = 3 #Déplacement nécessaire (%) pour faire bouger le moteur

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
        #print("stringToSend = " + stringToSend)
    if msg != "<>" :
        #print(msg)
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
