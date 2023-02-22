import unittest
from test.TestComm.testCommDevice import *
from Comm.MessageIO import *

class TestMessageIO(unittest.TestCase):
    def test_add(self):
        IOManger = MessageIO()

        self.assertEqual(0,IOManger.getNbrDevices())
        IOManger.addDevice(testCommDevice())
        self.assertEqual(1, IOManger.getNbrDevices())

    def test_sendMessage(self):
        IOManger = MessageIO()
        dev = testCommDevice()
        IOManger.addDevice(dev)
        msg = ControlMessage(1, [1, 2, 3, 4])
        IOManger.sendMessage(0, msg)

        msgSend = dev.getLastMessageSend()
        self.assertEqual(1, msgSend["type"])
        self.assertEqual(4, msgSend["PLS"])

        for x in range(0, msg.getPayloadSize()):
            self.assertEqual(msg.getPayload()[x], msgSend["data"][x])

    def test_readMessage(self):
        IOManger = MessageIO()
        dev = testCommDevice(True)
        IOManger.addDevice(dev)
        msg = ControlMessage(1, [1, 2, 3, 4])
        IOManger.sendMessage(0, msg)

        readMessage = IOManger.readMessage(0)

        self.assertEqual(msg.getType(), readMessage.getType())
        self.assertEqual(msg.getPayloadSize(), readMessage.getPayloadSize())

    def test_readNoMessage(self):
        IOManger = MessageIO()
        dev = testCommDevice()
        IOManger.addDevice(dev)

        self.assertEqual(None, IOManger.readMessage(0))