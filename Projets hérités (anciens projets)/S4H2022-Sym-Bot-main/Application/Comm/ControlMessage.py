class ControlMessage:
    '''This class transforms message sent to the MessageIO object to be correctly read by it
    
    Attributes (most used for this application):
    ---------------
    SET_HOME :              Setting new
    SET_JOINT_POSITION :    Message type associated with robot being control with angle sent to joints
    GOTO_HOME :             Message type associated with robot going home (must be confortable)
    SET_JOG :               Message type associated with robot controlled with jogMode (see translate.py)
    '''
    # ACTION IN[21, 29]
    SET_JOINT_POSITION              =  1
    GOTO_HOME                       =  2
    SET_JOG                         =  3
    OPEN_GRIPPER                    =  5
    CLOSE_GRIPPER                   =  6
    ACTION_POSITION_GRIPPER         =  7
    # SET VALUE IN[11, 19]
    SET_ZERO_POSITION_INDEX         = 11
    SET_HOME_POSITION_INDEX         = 12
    SET_MAX_POSITION_INDEX          = 13
    # RETURN VALUE IN[21, 29]
    RETURN_ZERO_POSITION_INDEX      = 21
    RETURN_HOME_POSITION_INDEX      = 22
    RETURN_MAX_POSITION_INDEX       = 23
    RETURN_ACTUAL_POSITION_INDEX    = 27

    def __init__(self, type, payload=[]):
        self.type = type
        self.payload = payload

    def getType(self):
        return self.type

    def getPayloadSize(self):
        return len(self.payload)

    def getPayload(self):
        return self.payload
