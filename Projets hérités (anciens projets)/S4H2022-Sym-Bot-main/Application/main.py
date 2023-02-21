import serial

from Comm.translate import translate
from Comm.MessageIO import MessageIO
from Comm.BluetoothComm import Bluetooth
from Comm.SerialComm import SerialComm

if __name__ == '__main__':

    # Select Communicatino's type between "USB" (Simuling UI) or "BT" (Glove)
    TypeComm = "BT" 

    messageIO = MessageIO()                                 # Create Object to charge the communication
    SymBot = translate(messageIO)                           # Create Object to charge the control
    messageIO.addDevice(SerialComm("/dev/ttyACM0", 57600))  # Robotic Arm (OpenCr Board)

    if TypeComm == "BT":
        ''' Commmunication Bluetooth '''
        MAC_ = '58:BF:25:37:A6:9A'                          # your MAC BT device
        uuid_ = "00001101-0000-1000-8000-00805f9b34fb"      # your uuid BT device
        bt = Bluetooth(MAC_,1,uuid_)
        while True:
            if bt.isMessageAvailable():
                msg = bt.readMessage()
                if bt.isValidMsg():
                    print(msg)                      # View message flow and detect any problem (like latency)
                    SymBot.chooseMode(msg)          # Process the message

    elif TypeComm == "USB":
        ''' Communication USB '''
        Serial1 = SerialComm("/dev/ttyACM1", 57600) # Alternative for BT Communication

        while True:
            while Serial1.isMessageAvailable():
                msg = Serial1.readMessage()
                if Serial1.isGoodMsg(msg):
                    msg = Serial1.getDigital()
                    print(msg)                      # View message flow and detect any problem (like latency)
                    SymBot.chooseMode(msg)          # Process the message
    else:
        print(TypeComm + " not supported")
