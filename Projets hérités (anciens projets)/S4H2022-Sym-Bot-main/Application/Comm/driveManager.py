from Comm.MessageIO import *
from Comm.ControlMessage import *


class DriveManager:
    def __init__(self, controller, msgIO,controllerIndexOfGripper=0):
        # Indexes of joints controlled by controllers
        self.jointIndex = controller
        self.msgManager = msgIO
        self.controllerIndexOfGripper = controllerIndexOfGripper
    def getJointIndex(self):
        return self.jointIndex

    def setJoinPosition(self, position):
        msg = ControlMessage(ControlMessage.SET_JOIN_POSITION, position)
        self.sendToMsgToAllController(msg)

    def setHome(self, newHome):
        msg = ControlMessage(ControlMessage.SET_HOME, newHome)
        self.sendToMsgToAllController(msg)

    def gotoHome(self):
        msg = ControlMessage(ControlMessage.GOTO_HOME)
        self.sendToMsgToAllControllerwithoutPayload(msg)

    def sendToMsgToAllController(self, ctrlmsg):
        readPayload = ctrlmsg.getPayload()
        msgType = ctrlmsg.getType()
        # Create a dictionnary to store the axis with controller number as keys
        controllerDict = {}
        for x in range(0, len(self.jointIndex)):
            if controllerDict.get(self.jointIndex[x]) is None:
                controllerDict[self.jointIndex[x]] = [readPayload[x]]
            else:
                controllerDict[self.jointIndex[x]].append(readPayload[x])
        for controllerId in controllerDict:
            newMessage = ControlMessage(msgType, controllerDict[controllerId])
            self.msgManager.sendMessage(controllerId, newMessage)

    def sendToMsgToAllControllerwithoutPayload(self, msg):
        c = set(self.jointIndex)
        for index in c:
            self.msgManager.sendMessage(index, msg)

    def openGripper(self, rate):
        msg = ControlMessage(ControlMessage.OPEN_GRIPPER, [rate])
        self.msgManager.sendMessage(self.controllerIndexOfGripper, msg)

    def closeGripper(self, rate):
        msg = ControlMessage(ControlMessage.CLOSE_GRIPPER, [rate])
        self.msgManager.sendMessage(self.controllerIndexOfGripper, msg)

    def setGripperPosition(self, position):
        msg = ControlMessage(ControlMessage.SET_GRIPPER_POSITION, [position])
        self.msgManager.sendMessage(self.controllerIndexOfGripper, msg)

