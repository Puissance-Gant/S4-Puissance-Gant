import unittest
from Comm.ControlMessage import ControlMessage

class TestControlMessage(unittest.TestCase):
    def test_init(self):
        data = [1,2,3,4]
        msg = ControlMessage(1,[1,2,3,4])

        self.assertEqual(1,msg.getType())
        self.assertEqual(4,msg.getPayloadSize())

        for x in range(0, len(data)):
            self.assertEqual(data[x], msg.getPayload()[x])

    def test_OnlyTypeInit(self):
        msg = ControlMessage(1)

        self.assertEqual(1, msg.getType())
        self.assertEqual(0, msg.getPayloadSize())
