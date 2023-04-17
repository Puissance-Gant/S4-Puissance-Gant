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
CHANGEMENT_MIN = 3 #Déplacement nécessaire (%) pour faire bouger le moteur

#========================
# Lancer la connexion sérielle avec l'OpenCR
def setupSerialOpenCR(baudRate, serialPortName):
    
    global  serialPortOpenCR
    
    serialPortOpenCR = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)

    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))


#========================

# Envoyer un messages à l'OpenCR. La syntaxe est telle que msg = "<...>" où < et > sont des marqueurs de début et de fin du message
def sendToOpenCR(msg):

    global serialPortOpenCR

    # Si le message à envoyer contient des positions de moteurs, on vérifie qu'il y a un assez grand mouvement 
    # Cela permet d'optimiser la taille des messages à envoyer
    if any(c in msg for c in 'ABCD'):
        msg = verifierPosMoteurs(msg)

    # On s'assure qu'on envoie pas un message vide
    if msg != "<>":
        serialPortOpenCR.write(msg.encode('utf-8')) # Encoder le message

#========================

# Vérifier si les positions de moteurs à envoyer sont assez différentes de la dernière commande envoyée
def verifierPosMoteurs(message):
    global anciennePosA, anciennePosB, anciennePosC, anciennePosD, startMarker
    valeur = ""         # la valeur numérique envoyée (ex. pour msg = "50A", valeur = 50)
    msgFiltre = ""      # le message contenant les valeurs de positionnement choisies
    msgDepart = False   # booléen indiquant si le marqueur de départ a été trouvé
    try:
        for char in message:
            if msgDepart:
                match char:
                    case 'A': # Pouce
                        if abs(int(valeur) - anciennePosA) > CHANGEMENT_MIN:
                            msgFiltre = msgFiltre + valeur + 'A'
                            anciennePosA = int(valeur)
                        valeur = ""
                    case 'B': # Index
                        if abs(int(valeur) - anciennePosB) > CHANGEMENT_MIN:
                            msgFiltre = msgFiltre + valeur + 'B'
                            anciennePosB = int(valeur)
                        valeur = ""
                    case 'C': # Majeur
                        if abs(int(valeur) - anciennePosC) > CHANGEMENT_MIN:
                            anciennePosC = int(valeur)
                            msgFiltre = msgFiltre + valeur + 'C'
                        valeur = ""
                    case 'D': # Poignet
                        if abs(int(valeur) - anciennePosD) > CHANGEMENT_MIN:
                            anciennePosD = int(valeur)
                            msgFiltre = msgFiltre + valeur + 'D'
                        valeur = ""
                    case '>':
                        return str('<' + msgFiltre + '>')
                    case _:
                        valeur = valeur + char

            elif char == startMarker:
                msgDepart = True
    except Exception:
        return ""
    
    return msgFiltre
        

        

#========================
# Cette fonction est tirée du lien suivant : https://forum.arduino.cc/t/pc-arduino-comms-using-python-updated/574496
# Elle permet de lire le buffer sériel en provenance de l'OpenCR efficacement
def recvFromOpenCR():

    global startMarker, endMarker, serialPortOpenCR, dataStarted, dataBuf, messageComplete
    if serialPortOpenCR.inWaiting() > 0 and messageComplete == False:
        
        x = serialPortOpenCR.read().decode("utf-8") # décoder le message
        
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
