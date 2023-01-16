import unittest
from Comm.driveManager import *
from Comm.ControlMessage import ControlMessage
from Comm.MessageIO import MessageIO
from test.TestComm.testCommDevice import *

from enum import Enum

class TestDriveManager(unittest.TestCase):
    def test_init(self):
        controller = [1, 2, 3, 4]
        msg = MessageIO()
        msg.addDevice(testCommDevice())
        driveManager = DriveManager(controller, msg)
        self.assertEqual(driveManager.getJointIndex(), controller)

    def test_setJoinPosition(self):
        position = [40, 23, 11, 52]
        controller = [0, 0, 2, 1]
        msgManager = MessageIO()
        msgManager.addDevice(testCommDevice(True))
        msgManager.addDevice(testCommDevice(True))
        msgManager.addDevice(testCommDevice(True))
        driveManager = DriveManager(controller, msgManager)
        driveManager.setJoinPosition(position)

        test_msg_1 = msgManager.readMessage(0)
        test_msg_2 = msgManager.readMessage(1)
        test_msg_3 = msgManager.readMessage(2)

        test_ctrl_1 = [40, 23]
        test_ctrl_2 = [52]
        test_ctrl_3 = [11]

        self.assertEqual(test_ctrl_1, test_msg_1.getPayload())
        self.assertEqual(ControlMessage.SET_JOIN_POSITION, test_msg_1.getType())

        self.assertEqual(test_ctrl_2, test_msg_2.getPayload())
        self.assertEqual(ControlMessage.SET_JOIN_POSITION, test_msg_2.getType())

        self.assertEqual(test_ctrl_3, test_msg_3.getPayload())
        self.assertEqual(ControlMessage.SET_JOIN_POSITION, test_msg_3.getType())





    def test_openGripper(self):
        controller = [1, 1, 0]
        msgManager = MessageIO()
        msgManager.addDevice(testCommDevice(True))
        msgManager.addDevice(testCommDevice(True))
        driveManager = DriveManager(controller, msgManager, controllerIndexOfGripper=1)


        driveManager.openGripper(100)

        test_msg = msgManager.readMessage(1)

        test_ctrl = [100]


        self.assertEqual(test_ctrl, test_msg.getPayload())
        self.assertEqual(ControlMessage.OPEN_GRIPPER, test_msg.getType())

    def test_closeGripper(self):
        controller = [1, 1, 0]
        msgManager = MessageIO()
        msgManager.addDevice(testCommDevice(True))
        msgManager.addDevice(testCommDevice(True))
        driveManager = DriveManager(controller, msgManager, controllerIndexOfGripper=1)


        driveManager.closeGripper(100)

        test_msg = msgManager.readMessage(1)

        test_ctrl = [100]


        self.assertEqual(test_ctrl, test_msg.getPayload())
        self.assertEqual(ControlMessage.CLOSE_GRIPPER, test_msg.getType())


    def test_setGripperPosition(self):
        controller = [1, 1, 0]
        msgManager = MessageIO()
        msgManager.addDevice(testCommDevice(True))
        msgManager.addDevice(testCommDevice(True))
        driveManager = DriveManager(controller, msgManager, controllerIndexOfGripper=1)


        driveManager.setGripperPosition(100)

        test_msg = msgManager.readMessage(1)

        test_ctrl = [100]


        self.assertEqual(test_ctrl, test_msg.getPayload())
        self.assertEqual(ControlMessage.SET_GRIPPER_POSITION, test_msg.getType())

    def test_setHome(self):
        controller = [1, 1, 0]
        msgManager = MessageIO()
        msgManager.addDevice(testCommDevice(True))
        msgManager.addDevice(testCommDevice(True))
        driveManager = DriveManager(controller, msgManager)

        newHome = [40, 23, 11]
        driveManager.setHome(newHome)

        test_msg_1 = msgManager.readMessage(0)
        test_msg_2 = msgManager.readMessage(1)

        test_ctrl_1 = [11]
        test_ctrl_2 = [40, 23]

        self.assertEqual(test_ctrl_1, test_msg_1.getPayload())
        self.assertEqual(ControlMessage.SET_HOME, test_msg_1.getType())

        self.assertEqual(test_ctrl_2, test_msg_2.getPayload())
        self.assertEqual(ControlMessage.SET_HOME, test_msg_2.getType())


    def test_goToHome(self):
        controller = [1, 1, 0]
        msgManager = MessageIO()
        msgManager.addDevice(testCommDevice(True))
        msgManager.addDevice(testCommDevice(True))
        driveManager = DriveManager(controller, msgManager)
        driveManager.gotoHome()

        test_msg_1 = msgManager.readMessage(0)
        test_msg_2 = msgManager.readMessage(1)

        test_ctrl_1 = []
        test_ctrl_2 = []

        self.assertEqual(test_ctrl_1, test_msg_1.getPayload())
        self.assertEqual(ControlMessage.GOTO_HOME, test_msg_1.getType())

        self.assertEqual(test_ctrl_2, test_msg_2.getPayload())
        self.assertEqual(ControlMessage.GOTO_HOME, test_msg_2.getType())