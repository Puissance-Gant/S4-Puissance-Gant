import json

class testCommDevice:
    def __init__(self,isMessageAvailable=False):
        self.LastSendMessage = None
        self.LastSendRaw = None
        self.isMsgAvailable = isMessageAvailable

    def sendJSon(self, JSon):
        self.LastSendMessage = JSon
        self.LastSendRaw = json.dumps(JSon)

    def readJSon(self):
        return json.loads(self.LastSendRaw)

    def getLastMessageSend(self):
        return self.LastSendMessage

    def getLastMessageSendRaw(self):
        return self.LastSendRaw

    def isMessageAvailable(self):
        return self.isMsgAvailable