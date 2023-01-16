import serial
import json


class fakeSerialComm:
    """This class is meant to be used for tests that need to "communicate" with a robot that doesn't exists"""
    
    def __init__(self):
        pass

    def sendJSon(self, JSon):
        print(JSon)

    def readJSon(self):
        return 1

    def isMessageAvailable(self):
        return 1