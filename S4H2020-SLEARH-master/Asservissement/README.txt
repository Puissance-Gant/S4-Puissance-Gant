Motors control

This README will explain how to control the different motors on the hand.
In the document Test_main, you can find all the three files needed for the motor control.

Test_main.ino
•	This file is the main. 
•	This is where we initialize all the motors (servomotors and dynamixels). When started, the hand stands up at is initial position and wait for a message from the interface.
•	The main uses the class COM_Arduino in the file Communication.h for all the function connected to the communication with the interface.
•	When a message is received from the interface, the functions from class Servo of the file controlServo.h are called to execute a letter.

controlServo.h
•	This file contains the class Servo.
•	The class Servo contains the struct Servo and all the function used in for the control of the motors.
•	The struct Servo contains all the configurations of the 5 fingers, the wrist and the base for all the characters (letters and numbers).
•	The three functions called form the main (Test_main.ino) are the setupBase, servoOut and the reverseMove.
•	The function setupBase is use to move the base and wrist at is initial position depended on is initial position.
•	The function servoOut is use the move all motors in the configuration of a specific letter or number.
•	The function reverseMove is use to replace the hand in is initial position.

Communication.h
•	This file contains the class COM_Arduino.
•	The class COM_Arduino contains all the function use to communicate with the interface.
•	The functions SendMessage and ReadMessage interact with the interface.
•	The functions Set_state and Get_state are useful if you want to manage the state of the object.

