from unicodedata import digit
import serial
import json


class SerialComm:
    digital = {}

    '''This class translates a dictionnary send by MessageIO object into Json for serial communication'''
    def __init__(self, port, baud):
        self.port = port
        self.baud = baud
        self.serial = serial.Serial(port, baud, timeout=1)

    def sendJSon(self, JSon):
        j = (json.dumps(JSon)+'\n').encode()
        self.serial.write(j)


    def readJSon(self):
        rawJSon = self.serial.readline().decode()
        return json.loads(rawJSon)

    def readMessage(self):
        raw     = self.serial.readline()
        decode  = raw.decode()
        replace = decode.replace("\r\n", "")
        array   = replace.split("|")
        return array

    def isGoodMsg(self, array):
        # Nine elements is expeted in array (8 digitals + 1 empty)
        if len(array) != 9:
            return False

        # Transform string array to digital array
        try:
            self.digital = {}
            self.digital['Mode'] = int(array[0])
            self.digital['Flex'] = [int(x) for x in array[1:5]]
            self.digital['IMU']  = [float(x) for x in array[5:8]]
        except (Exception):
            return False

        return True

    def getDigital(self):
        return self.digital

    def isMessageAvailable(self):
        return self.serial.in_waiting > 0