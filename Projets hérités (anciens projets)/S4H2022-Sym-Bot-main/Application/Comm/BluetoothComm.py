# https://github.com/pybluez/pybluez
import bluetooth
# https://docs.python.org/3/library/socket.html#socket.socket.listen
# import socket     # Wireless Communication - Low Level Alternative  

# Exemple ESP32 Specifications
# MAC   = '58:BF:25:37:A6:9A'
# uuid  = "00001101-0000-1000-8000-00805f9b34fb"

class Bluetooth:
    '''This class represents the bluetooth communication with a device;
        in this case, it's an ESP32

    Attributes:
    -------------
    validateMsg :   boolean      indicates received message is in the good shape
    '''
    validateMsg = False

    def __init__(self, mac, port=1, uuid=0):
        ''' This function initialize all what's necessary for Bluetooth class to be functional

        Parameters:
        -------------
        mac  :  string  Network Address for connection
        port :  int     That's the communication port
        uuid :  string  Give the ID of what we want to communicate with     
        '''
        self.port = port
        self.host = mac
        
        # Collect the device informations
        services = bluetooth.find_service(uuid=uuid, address=mac)
        first_match = services[0]
        self.port = first_match["port"]
        self.name = first_match["name"]
        self.host = first_match["host"]
        print("Connecting to \"{}\" on {}, port : {}".format(self.name, self.host, self.port))

        # Create the client socket
        self.sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        self.sock.connect((self.host, self.port))

        '''
        # Alternative
        # Create the client socket (Low Level)
        self.sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
        self.sock.connect((mac, self.port))
        '''
        
    def isMessageAvailable(self):
        '''This function verifies if a message is available
        
        Parameters:
        -----------------
        This function has no parameters
        '''
        self.buffer = []

        data = self.sock.recv(64)           # Get Next 64 bytes
        self.buffer = data.decode()

        # Short check if the message is in the minimal good format
        # Expected message : "0|##0|##0|##0|##0|##0.00|##0.00|##0.00|\r\n"
        # len() > 16 : To eliminate the incomplete message
        # [-2:] == "\r\n" : Terminate by Enter
        if(len(self.buffer) > 16 and self.buffer[-2:] == "\r\n"):
            return True
        else:
            return False

    def readMessage(self):
        '''It's pretty obvious what this function does
        
        Parameters:
        -----------------
        This function has no parameters
        '''
        self.validateMsg = False

        # Formatting in dictionary to translate.py module
        # try is to catch wrong data
        text = (self.buffer[:-3]).split("|")
        try:
            msg = {}
            msg['Mode'] = int(text[0])
            msg['Flex'] = [int(x) for x in text[1:5]]
            msg['IMU']  = [float(x) for x in text[5:8]]

            self.validateMsg = True
        except (Exception):
            pass

        return msg

    def isValidMsg(self):
        '''Function return the validateMsg value
        
        Parameters:
        -----------------
        This function has no parameters
        '''
       return self.validateMsg

    def closeConnection(self):
        '''Function close the socket connection
        
        Parameters:
        -----------------
        This function has no parameters
        '''
       self.sock.close()
