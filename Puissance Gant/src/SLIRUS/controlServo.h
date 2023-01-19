/** @file controlServo.h
 * The file controlServo.h contains the class Servo.
 */

 /** @class Servo
  * @brief Class containing functions related to the servomotors.
  *
  * This class contains the structures of characters and the functions used to control the servo output.
  * @author CHARBONNEAU, EMILE
  * @date 19/04/2020
  * Modify by BEAUMIER,FANNY
  * @date 13/04/2021
  */

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>                        ///< Library needed to control the PWM of servo drive.
#include <Wire.h>
#include <DynamixelWorkbench.h>

  // Section to change when adding servos or characters:
#define NB_MOTORS             10                            ///< Number of active motors.
#define NB_FINGERS            5                             ///< Number of active fingers.
#define NB_LETTERS            36                            ///< Number of characters managed.

// The next defined are the different articulated positions for a finger:
#define VERTICAL              0                             ///< Finger fully unfold.
#define HORIZONTAL            1                             ///< Finger with first knuckle fold.
#define SEC_90				  2                             ///< Finger with second knuckle fold.
#define FULLY_INCLINED        3                             ///< Finger with botch knuckles fold.
#define CONFIG_MRLA			  4                             ///< Finger with 80 degree angle on both knuckle.
#define CONFIG_MRLB			  5                             ///< Finger with 100 degree angle on both knuckle.
#define CONFIG_MRLC			  6                             ///< Finger with 110 degree angle on both knuckle.
#define VERTICAL_CROSS		  7								///< Index fully unfold and cross.
#define VERTICAL_HCROSS		  8								///< Index half unfold and cross.
#define HALF_THUMB			  9							///< Index half unfold and cross.


/** @brief The next defined are the different fingers implemented.
 *
 * The increment per finger is 2 because there are 2 motors per finger that requires to move. (except for the index and the thumb)
 * This implementation eases the working principal of the function moveFinge(int,int).
 */
#define THUMB                 0
#define INDEX                 2
#define MIDDLE                4
#define RING                  6
#define LITTLE                8

 /**Defining fingers moving angles.
  */
#define KNUCKLE1_UP				0							///< Angle for a straigth knuckle.
#define KNUCKLE1_90             180                         ///< Angle for an inclined knuckle.
#define KNUCKLE2_UP				0							///< Angle for a straigth knuckle.
#define KNUCKLE2_90             180                         ///< Angle for an inclined knuckle.
#define KNUCKLE_80				80							///< Angle for an 80 degrees inclined knuckle.
#define KNUCKLE_100				100							///< Angle for an 100 degrees inclined knuckle.
#define KNUCKLE_110				110                         ///< Angle for an 110 degrees inclined knuckle.
#define NOT_CROSS				0							///< Angle for a index not cross.
#define FULLY_CROSS				110							///< Angle for a index fully cross.
#define HALF_CROSS				70							///< Angle for a index half cross.
#define THUMB_UP				105							///< Angle for a thumb up.
#define THUMB_DOWN				140							///< Angle for a thumb down.
#define THUMB_HALF				110							///< Angle for a thumb half.

/**Defining base and wrist moving angles.
*/
#define FRONT					0							///< Hand face front.
#define DOWN_RIGHT				1							///< Hand turn rigth and wrist at 90 degrees.
#define HALF_LEFT				2							///< Hand turn rigth and wrist at 45 degrees.
#define LEFT					3							///< Hand turn left.
#define DOWN					4							///< Hand have wrist at 90 degrees.
#define MOVE_J					5							///< Hand move for J letter.
#define MOVE_Z					6							///< Hand move for Z letter.
#define HALF_DOWN				7							///< Hand move half down.

/**Defining wrist moving angles.
*/
#define WRIST_UP				2							///< Angle for a vertical wrist.
#define WRIST_HALF				500							///< Angle for a 45 degrees wirst.
#define WRIST_DOWN				1000						///< Angle for a horizontal wrist.

/**Global variable for the base moving angles.
*/
float b_front;												///< Hand face front.
float b_rigth;												///< Hand turn rigth.
float b_left;												///< Hand turn left.
float b_half_right;											///< Hand turn half rigth.
float b_half_left;											///< Hand turn half left.


/**The next section contains defined PWM constants.
*/
#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();    ///< Setting up the pwm object with the default address for the driver (0x40).
uint8_t servonum = 1;                                       ///< Creation of a servo driver object (PCA9685).
DynamixelWorkbench dxl_wb;									///< Creation of a dynamixel.

/** @struct character
 * @brief Creating a structure that will hold informations relative to each character.
 */
struct character {
	int id;                                                   ///< The character in question converted in Ascii.
	int pattern[NB_FINGERS];                                  ///< The pattern in which order should the fingers move.
	int angle[NB_FINGERS];                                    ///< The angles the fingers should move to in accordance to the pattern.
	int rotation;											  ///< Angle for the rotation of the base and wrist.
} charact[NB_LETTERS];                                        ///< Creating a structure instance charact wich is an array the size of 

class Servo
{
public:
	Servo() {
		/** @brief Default constructor for the object servo.
		 *
		 * This constructor initializes each character coded below in accordance to the structure character.
		 * This is the structure to follow :
		 * charact[int] = {{('Character id')},{pattern},{Angles},{rotation}};
		 */
		charact[0] = { ('0'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{CONFIG_MRLC,FULLY_INCLINED,CONFIG_MRLC,CONFIG_MRLC,CONFIG_MRLC},LEFT };
		charact[1] = { ('1'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED},FRONT };
		charact[2] = { ('2'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED},FRONT };
		charact[3] = { ('3'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{HALF_THUMB,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED},FRONT };
		charact[4] = { ('4'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,VERTICAL,VERTICAL},FRONT };
		charact[5] = { ('5'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{HALF_THUMB,VERTICAL,VERTICAL,VERTICAL,VERTICAL},FRONT };
		charact[6] = { ('a'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED},FRONT };
		charact[7] = { ('b'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,VERTICAL,VERTICAL},FRONT };
		charact[8] = { ('c'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{CONFIG_MRLC,CONFIG_MRLA,CONFIG_MRLA,CONFIG_MRLA,CONFIG_MRLA},HALF_LEFT };
		charact[9] = { ('d'),{INDEX,MIDDLE,RING,LITTLE,THUMB},{VERTICAL,CONFIG_MRLC,CONFIG_MRLC,CONFIG_MRLC,CONFIG_MRLC},FRONT };
		charact[10] = { ('e'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED},FRONT };
		charact[11] = { ('f'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{CONFIG_MRLC,SEC_90,VERTICAL,VERTICAL,VERTICAL},FRONT };
		charact[12] = { ('g'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED},DOWN_RIGHT };
		charact[13] = { ('h'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED},DOWN_RIGHT };
		charact[14] = { ('i'),{INDEX,MIDDLE,RING,LITTLE,THUMB},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,VERTICAL,FULLY_INCLINED},FRONT };
		charact[15] = { ('j'),{INDEX,MIDDLE,RING,LITTLE,THUMB},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,VERTICAL,FULLY_INCLINED},MOVE_J };
		charact[16] = { ('k'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED},FRONT };
		charact[17] = { ('l'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{HORIZONTAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED},FRONT };
		charact[18] = { ('m'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,CONFIG_MRLC,CONFIG_MRLC,CONFIG_MRLC,FULLY_INCLINED},HALF_DOWN };
		charact[19] = { ('n'),{LITTLE,RING,THUMB,INDEX,MIDDLE,},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,CONFIG_MRLC,CONFIG_MRLC},HALF_DOWN };
		charact[20] = { ('o'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{CONFIG_MRLC,SEC_90,CONFIG_MRLC,CONFIG_MRLC,CONFIG_MRLC},LEFT };
		charact[21] = { ('p'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,HORIZONTAL,FULLY_INCLINED,FULLY_INCLINED},DOWN_RIGHT };
		charact[22] = { ('q'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{HORIZONTAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED},DOWN_RIGHT };
		charact[23] = { ('r'),{THUMB,RING,LITTLE,MIDDLE,INDEX},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,VERTICAL,VERTICAL_CROSS},FRONT };
		charact[24] = { ('s'),{INDEX,MIDDLE,RING,LITTLE,THUMB},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED},FRONT };
		charact[25] = { ('t'),{INDEX,MIDDLE,RING,LITTLE,THUMB},{CONFIG_MRLC,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,VERTICAL},FRONT };
		charact[26] = { ('u'),{THUMB,MIDDLE,RING,LITTLE,INDEX},{FULLY_INCLINED,VERTICAL,FULLY_INCLINED,FULLY_INCLINED,VERTICAL_HCROSS},FRONT };
		charact[27] = { ('v'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED},FRONT };
		charact[28] = { ('w'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,VERTICAL,FULLY_INCLINED},FRONT };
		charact[29] = { ('x'),{LITTLE,RING,MIDDLE,INDEX,THUMB},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,SEC_90,FULLY_INCLINED},FRONT };
		charact[30] = { ('y'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{HALF_THUMB,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,VERTICAL},FRONT };
		charact[31] = { ('z'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED},MOVE_Z };
		charact[32] = { ('6'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,VERTICAL,FULLY_INCLINED},FRONT };
		charact[33] = { ('7'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{CONFIG_MRLC,VERTICAL,VERTICAL,FULLY_INCLINED,VERTICAL},FRONT };
		charact[34] = { ('8'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{CONFIG_MRLC,VERTICAL,CONFIG_MRLC,VERTICAL,VERTICAL},FRONT };
		charact[35] = { ('9'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{CONFIG_MRLC,FULLY_INCLINED,VERTICAL,VERTICAL,VERTICAL},FRONT };
	}

	bool setupBase(float position) {
		b_front = 0 + position;
		b_rigth = 3100 + position;
		b_left = -3100 + position;
		b_half_left = -1550 + position;
		b_half_right = 1000 + position;
		#define BASE_FRONT				b_front						///< Angle for base facing front.
		#define BASE_RIGHT				b_rigth						///< Angle for base turning rigth.
		#define BASE_LEFT				b_left						///< Angle for base turning left.
		#define BASE_HALF_LEFT			b_half_left					///< Angle for base turning half left.
		#define BASE_HALF_RIGHT			b_half_right				///< Angle for base turning half right.
		return true;
	}
	/** @fn bool setupBase(float position)
	 * @brief Function of the servo class. This function will set the different angle to reach for different 
	          configuration of the base in function of the initial position of the base.
	 * @param character the character to be displayed.
	 * @param increment an integer instructing wich pattern and angle index to reach in the character's structure.
	 * @return true only once every finger moved.
	 */

	bool servoOut(int character, int increment) {
		character = adjustCommand(character);
		int finger = charact[character].pattern[increment];
		int moveOption = charact[character].angle[increment];
		int wristBase = charact[character].rotation;
		moveFinger(finger, moveOption);
		if (wristBase == MOVE_J) {
			if(increment == NB_FINGERS-1){
				moveBaseWrist(wristBase);
			}
		}
		else if (wristBase == MOVE_Z) {
			if (increment == NB_FINGERS - 1) {
				moveBaseWrist(wristBase);
			}
		}
		else {
			moveBaseWrist(wristBase);
		}
		
		if (increment < NB_FINGERS - 1) { return false; }
		else { return true; }
	}
	/** @fn bool servoOut(int character,int increment)
	 * @brief Function of the servo class. This function will send the command to move a finger.
	 * @param character the character to be displayed.
	 * @param increment an integer instructing wich pattern and angle index to reach in the character's structure.
	 * @return true only once every finger moved.
	 */

	bool reverseMove(int character, int decrement) {
		character = adjustCommand(character);
		int finger = charact[character].pattern[decrement];
		int moveOption = VERTICAL;
		int wristBase = FRONT;
		moveFinger(finger, moveOption);
		moveBaseWrist(wristBase);
		if (decrement > 0) { return false; }
		else { return true; }
	}
	/** @fn bool reverseMove(int character,int decrement)
	 * @brief This function will move a finger to the vertical (letter 5).
	 * @param character the character previously displayed.
	 * @param decrement an integer instructing wich finger to unfold.
	 * @return true only once every finger has unfold.
	 */

	int moveBaseWrist(int wristBase) {
		int nbMotor = 2;
		int rotation[nbMotor];
		bool readyToMove = false;
		bool readypart2 = false;
		bool readypart3 = false;
		bool readypart4 = false;
		for (int i = 0; i < nbMotor; i++) {
			rotation[i] = 0;
		}
		if (wristBase == 0) {
			rotation[0] = BASE_FRONT;
			rotation[1] = WRIST_UP;
			readyToMove = true;
		}
		if (wristBase == 1) {
			rotation[0] = BASE_RIGHT;
			rotation[1] = WRIST_DOWN;
			readyToMove = true;
		}
		if (wristBase == 2) {
			rotation[0] = BASE_LEFT;
			rotation[1] = WRIST_HALF;
			readyToMove = true;
		}
		if (wristBase == 3) {
			rotation[0] = BASE_LEFT;
			rotation[1] = WRIST_UP;
			readyToMove = true;
		}
		if (wristBase == 4) {
			rotation[0] = BASE_FRONT;
			rotation[1] = WRIST_DOWN;
			readyToMove = true;
		}
		if (wristBase == 5) {
			//MOUVEMENT POR LA LETTRE J
			delay(500);
			rotation[0] = BASE_HALF_LEFT;
			rotation[1] = WRIST_DOWN;
			readyToMove = true;
			if (readyToMove) {
				dxl_wb.goalPosition(12, rotation[0]);
				dxl_wb.goalPosition(11, rotation[1]);
				delay(500); //TEST	
				readyToMove = false; //A METTRE ICI??
				readypart2 = true;
			}
			if (readypart2) {
				rotation[0] = BASE_LEFT;
				rotation[1] = WRIST_UP;
				readyToMove = true;
				if (readyToMove) {
					dxl_wb.goalPosition(12, rotation[0]);
					dxl_wb.goalPosition(11, rotation[1]);
					readyToMove = false; //Ne fait pas le dernier if (commande deja les moteurs)
				}
			}
		}
		if (wristBase == 6) {
			//MOUVEMENT POR LA LETTRE Z
			delay(500);
			rotation[0] = BASE_HALF_LEFT;
			rotation[1] = WRIST_HALF;
			readyToMove = true;
			if (readyToMove) {
				dxl_wb.goalPosition(12, rotation[0]);
				dxl_wb.goalPosition(11, rotation[1]);
				delay(1000); //TEST	
				readyToMove = false; //A METTRE ICI??
				readypart2 = true;
			}
			if (readypart2) {
				rotation[0] = BASE_HALF_RIGHT;
				rotation[1] = WRIST_HALF;
				readyToMove = true;
				if (readyToMove) {
					dxl_wb.goalPosition(12, rotation[0]);
					dxl_wb.goalPosition(11, rotation[1]);
					delay(650); //TEST	
					readyToMove = false; //A METTRE ICI??
					readypart3 = true;
				}
			}
			if (readypart3) {
				rotation[0] = BASE_HALF_LEFT;
				rotation[1] = WRIST_DOWN;
				readyToMove = true;
				if (readyToMove) {
					dxl_wb.goalPosition(12, rotation[0]);
					dxl_wb.goalPosition(11, rotation[1]);
					delay(650); //TEST	
					readyToMove = false; //A METTRE ICI??
					readypart4 = true;
				}
			}
			if (readypart4) {
				rotation[0] = BASE_HALF_RIGHT;
				rotation[1] = WRIST_DOWN;
				readyToMove = true;
				if (readyToMove) {
					dxl_wb.goalPosition(12, rotation[0]);
					dxl_wb.goalPosition(11, rotation[1]);
					readyToMove = false; //Ne fait pas le dernier if (commande deja les moteurs)
				}
			}
		}
		if (wristBase == 7) {
			rotation[0] = BASE_FRONT;
			rotation[1] = WRIST_HALF;
			readyToMove = true;
		}
		if (readyToMove) {
				dxl_wb.goalPosition(12, rotation[0]);
				dxl_wb.goalPosition(11, rotation[1]);
		}
		return 0;
	}
	/** @fn int moveBaseWrist(int wristBase)
	 * @brief This function sends the PWM to the dynamixel of the base and the wrist.
	 * @param baseWrist a configuration to be reach.
	 * The different move options are : FRONT, DOWN_RIGHT, HALF_LEFT, LEFT, DOWN, MOVE_J and MOVE_Z
	 * @return 0 once every dynamixel moved.
	 */

	int moveFinger(int finger, int moveOption) {
		int nbMotor = 2;
		int angle[nbMotor+1];
		angle[2] = NOT_CROSS;
		for (int i = 0; i < nbMotor; i++) {
			angle[i] = 25;
		}
		bool readyToMove = false;
		if (moveOption == 0) {
			angle[0] = KNUCKLE1_UP;
			angle[1] = KNUCKLE2_UP;
			angle[2] = THUMB_UP;
			readyToMove = true;
		}
		if (moveOption == 1) {
			angle[0] = KNUCKLE1_90;
			angle[1] = KNUCKLE2_UP;
			if (finger == 0) {
				angle[0] = KNUCKLE1_UP;
				angle[1] = KNUCKLE2_UP;
				angle[2] = THUMB_DOWN;
			}
			
			readyToMove = true;
		}
		if (moveOption == 2) {
			angle[0] = KNUCKLE1_UP;
			angle[1] = KNUCKLE2_90;
			readyToMove = true;
		}
		if (moveOption == 3) {
			angle[0] = KNUCKLE1_90;
			angle[1] = KNUCKLE2_90;
			angle[2] = THUMB_DOWN;
			readyToMove = true;
		}
		if (moveOption == 4) {
			angle[0] = KNUCKLE_80;
			angle[1] = KNUCKLE_80;
			readyToMove = true;
		}
		if (moveOption == 5) {
			angle[0] = KNUCKLE_100;
			angle[1] = KNUCKLE2_90;
			readyToMove = true;
		}
		if (moveOption == 6) { // courber
			angle[0] = KNUCKLE_110;
			angle[1] = KNUCKLE_110;
			angle[2] = THUMB_DOWN;
			readyToMove = true;
		}
		if (moveOption == 7) {
			angle[0] = KNUCKLE1_UP;
			angle[1] = KNUCKLE2_UP;
			angle[2] = FULLY_CROSS;
			readyToMove = true;
		}
		if (moveOption == 8) {
			angle[0] = KNUCKLE1_UP;
			angle[1] = KNUCKLE2_UP;
			angle[2] = HALF_CROSS;
			readyToMove = true;
		}
		if (moveOption == 9) { // THUMB HALF UP
			angle[0] = KNUCKLE1_UP;
			angle[1] = KNUCKLE2_UP;
			angle[2] = THUMB_HALF;
			readyToMove = true;
		}
		if (readyToMove) {
			for (int i = 0; i < nbMotor; i++) {
				pwm.setPWM(finger + i, 0, pulseWidth(angle[i]));
			}
		}
		if (finger == 2) {
			pwm.setPWM(10, 0, pulseWidth(angle[2]));
		}
		if (finger == 0) {
			pwm.setPWM(13, 0, pulseWidth(angle[2]));
		}
		return 0;
	}
	/** @fn int moveFinger(int finger, int moveOption)
	 * @brief This function sends the PWM to the servos of a finger.
	 * @param finger a finger to be moved.
	 * @param moveOption a selected moveOption.
	 * The different move options are : KNUCKLE1_UP, KNUCKLE1_90, KNUCKLE2_UP, KNUCKLE2_90, KNUCKLE_80, 
	   KNUCKLE_100, KNUCKLE_110, NOT_CROSS, FULLY_CROSS and HALF_CROSS
	 * @return 0 once every motor of the finger moved.
	 */

	int pulseWidth(int angle) {
		int pulse_wide, analog_value;
		pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
		analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
		return analog_value;
	}
	/** @fn int pulseWidth(int angle)
	 * @brief This function returns the analog value to set the PWM.
	 * @param angle the desired angle shall the motor reach.
	 * This function calculates the analog value corresponding a received angle.
	 * This analog value will bet sent to the servo motor.
	 * @return the analog value of the angle.
	 */

	int adjustCommand(int command) {
		int adjustedCommand = ' ';
		bool found = false;
		if (command <= 53 && command >= 48) {
			adjustedCommand = command - 48;
			found = true;
		}
		if (command >= 97 && command <= 122) {
			adjustedCommand = command - 91;
			found = true;
		}
		if (!found) {
			adjustedCommand = ' ';
		}
		return adjustedCommand;
	}
	/** @fn int adjustCommand(int command)
	 * @brief This function adjust the command.
	 * @param command the Ascii value of the desired character to display.
	 * The Ascii value is hashed to fit neatly in the charact[NB_LETTERS] array.
	 * @The charact[adjustedCommand] contains informations relative to command.
	*/
};
