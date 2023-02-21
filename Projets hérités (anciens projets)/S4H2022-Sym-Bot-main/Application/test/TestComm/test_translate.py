import unittest

# from setuptools import setup
import sys
sys.path.insert(0,"/Users/labmi/Documents/GitHub/Sym-Bot/Application/")
from Comm.MessageIO import MessageIO
from Comm.translate import translate
from test.TestComm.fakeSerialComm import fakeSerialComm



class testTranslate(unittest.TestCase):
    '''This class tests all the translate class ; it is meant to see if all its internal components are working'''

    def setUp(self):
        '''This function is automatically called by unittest; at this point, one should create objects (translate, MessageIO); 
        make sure to put the "fakeSerialComm" to control the output
        '''
        self.msgIO = MessageIO()
        self.message = translate(self.msgIO)
        self.msgIO.addDevice(fakeSerialComm())  #Port vers open
        # driveManager = DriveManager([0, 0, 0], messageIO)

    def test_chooseMode(self):
        '''Tests if modes are accuratly taken into account'''

        for mode in range(1,6):
            fake_bt_msg = {'Mode' : mode, 'Flex': [100,100,100,100], 'IMU' : [0,0,0.75]}
            self.message.chooseMode(fake_bt_msg)
            self.assertEqual(self.message.getMode(), mode)
    
    def test_IMU_jogMode(self):
        '''Tests if the IMU accuratly changes the signs of values'''
        valIMU = 0.75
        valMotor = 1
        for i in range(0,3):
            fake_bt_msg = {'Mode' : 1, 'Flex': [100,0,0,0], 'IMU' : [0,0,valIMU]}
            self.message.chooseMode(fake_bt_msg)
            self.assertEqual(self.message.getMsgMotor(0), valMotor)
            valIMU -= 0.75
            valMotor -= 1
    
    def test_flex_jogMode(self):
        '''Tests if flexion of finger modifies only if treshold is busted'''
        valFlex = 30
        valMotor = 1
        for i in range(0,2):
            fake_bt_msg = {'Mode' : 1, 'Flex': [valFlex,0,0,0], 'IMU' : [0,0,0.75]}
            self.message.chooseMode(fake_bt_msg)
            self.assertEqual(self.message.getMsgMotor(0), valMotor)
            valFlex -= 5
            valMotor -= 1       
    
    def test_gripper_jogMode(self):
        '''Tests if gripper is activated correctly with the fourth finger'''
        for id in range(0,3):
            valFlex = 30
            valMotor = 1
            fake_bt_msg = {'Mode' : 1, 'Flex': [valFlex,valFlex,valFlex,valFlex], 'IMU' : [0,0,0.75]}
            self.message.chooseMode(fake_bt_msg)  
            self.assertEqual(self.message.getMsgMotor(-1), valMotor)


        valFlex = 30
        valMotor = 1
        fake_bt_msg = {'Mode' : 1, 'Flex': [valFlex,valFlex,valFlex,valFlex], 'IMU' : [0,0,0.75]}
        self.message.chooseMode(fake_bt_msg)
        self.assertEqual(self.message.getMsgMotor(-1), valMotor)
        
        valMotor = 0
        fake_bt_msg = {'Mode' : 1, 'Flex': [valFlex,valFlex,valFlex,10], 'IMU' : [0,0,0.75]}
        self.message.chooseMode(fake_bt_msg)
        self.assertEqual(self.message.getMsgMotor(3), valMotor)

        fake_bt_msg = {'Mode' : 1, 'Flex': [valFlex,valFlex,10,valFlex], 'IMU' : [0,0,0.75]}
        self.message.chooseMode(fake_bt_msg)
        self.assertEqual(self.message.getMsgMotor(3), valMotor)


if __name__ == '__main__':
    unittest.main()

