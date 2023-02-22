class testMessageManager:
    def __init__(self):
        self.RXFrame = None
        self.connection = None

    def receveFrame(self, frame):
        self.RXFrame = frame

    def newConnection(self, socket):
        self.connection = socket