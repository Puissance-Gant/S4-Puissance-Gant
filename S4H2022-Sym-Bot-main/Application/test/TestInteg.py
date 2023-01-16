from Comm.MessageIO import *
from Comm.SerialComm import *
from Comm.driveManager import *


import time

class MotorTesting():

    def __init__ (self):
    
        msgIO = MessageIO()
        msgIO.addDevice(SerialComm("COM3", 57600))


    def Motor_position_solo (self, dep = 0):
        '''Déplacement de chacun des moteur en solo'''
        #Retour des moteur au point de départ
        print('Retour au point de départ')
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0, 0, 0])
        print(msg)
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)

        #Deplacement du moteur de la base et retour à position initiale
        print('Déplacement du moteur de base')
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [dep, 0, 0])
        print(msg)
        self.msgIO.sendMessage(0, msg)
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0,0,0])
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)

        #Deplacement du moteur de l'épaule et retour à la position initiale
        print("Déplacement du moteur de l'épaule")
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0, dep, 0])
        print(msg)
        self.msgIO.sendMessage(0, msg)
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0,0,0])
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)

        #Deplacement du moteur du coude et retour à la position initiale
        print('Déplacement du moteur du coude')
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0, 0, dep])
        print(msg)
        self.msgIO.sendMessage(0, msg)
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0,0,0])
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)

    def Motor_position_all(self, dep = 0):
        """Deplacement de chacun des moteur ensemble"""

        #Pour s'assurer de ne pas donner des valeur au-dessus des limites maximale
        if dep+200 >= 2048:
            dep = dep-200

        #Retour des moteur au point de départ
        print('Retour au point de départ')
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0, 0, 0])
        print(msg)
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)

        #Test des moteur avec toutes les même valeurs
        print('Les moteur vont tous à la même position')
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [dep, dep, dep])
        print(msg)
        self.msgIO.sendMessage(0, msg)
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0,0,0])
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)

        #Test des moteur avec des valeur différentes
        print('Les moteurs vont tous à des positions différente ')
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [dep, dep+100, dep+200])
        print(msg)
        self.msgIO.sendMessage(0, msg)
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0,0,0])
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)
    
    def Motor_set_home(self, home):
        """Initialisation d'un Home et déplacement vers le Home"""

        #Pour s'assurer de ne pas donner des valeur au-dessus des limites maximale
        if home+200 >= 2048:
            home = home - 200

        #Retour des moteur au point de départ
        print('Retour au point de départ')
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0, 0, 0])
        print(msg)
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)

        #Détermination de la position du home, pareil pour tous les moteur
        print('Homing identique pour chaque moteur')
        msg = ControlMessage(ControlMessage.SET_HOME, [home, home, home])
        print(msg)
        self.msgIO.sendMessage(0, msg)

        time.sleep(1)
        
        #Les moteurs vont au nouveau home
        msg = ControlMessage(ControlMessage.GOTO_HOME)
        print(msg)
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)

        #Retour des moteur au point de départ
        print('Retour au point de départ')
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, [0, 0, 0])
        print(msg)
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)

        #Détermination de la position du home, différent pour chaque moteur
        print('Homing différent pour chaque moteur')

        msg = ControlMessage(ControlMessage.SET_HOME, [home, home + 100, home + 200])
        print(msg)
        self.msgIO.sendMessage(0, msg)

        time.sleep(1)
        
        #Les moteurs vont au nouveau home
        msg = ControlMessage(ControlMessage.GOTO_HOME)
        print(msg)
        self.msgIO.sendMessage(0, msg)

        time.sleep(5)




        







