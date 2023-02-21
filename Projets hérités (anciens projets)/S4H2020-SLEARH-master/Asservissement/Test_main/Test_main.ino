/** @file Test_main.ino
 * @brief This file is the main program of the motors control.
 * 
 * This file contains the main loop of the project.
 * @author BEAUMIER, FANNY
 * @date 13/04/2021
 */
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "controlServo.h"
#include <DynamixelWorkbench.h>
#include <string>
#include "Communication.h"
/**
 * @headerfile Wire.h "Wire.h"
 * @headerfile Arduino.h "Arduino.h"
 * @headerfile Adafruit_PWMServoDriver.h "Adafruit_PWMServoDriver.h"
 * @headerfile controlServo.h "controlServo.h"
 * @headerfile DynamixelWorkbench.h "DynamixelWorkbench.h"
 * @headerfile string.h "string.h"
 * @headerfile Communication.h "Communication.h"
 */

/** Communication variables:
 */
#define FREQUENCY   50                      ///< Frequency for the servo
#define BAUDRATE  57600                     ///< Baud rate for Arduino mega 2560
#define NB_FINGERS  5                       ///< Number of finger on the hand
#if defined(__OPENCM904__)
#define DEVICE_NAME "3"                     ///< Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
#define DEVICE_NAME ""
#endif 
COM_Arduino ROB; 

/** command variables
 */
String msg = "";
uint16_t model_number = 0;
uint16_t model_number2 = 0;
char result, result2;
int32_t position = 0;

/** Initiating the Servo class object.
 */
Servo servo;

void setup() {
  Serial.begin(BAUDRATE); 

  /** Initializing Dynamixel
   */
  dxl_wb.init(DEVICE_NAME, BAUDRATE); //Initialisation du systeme qui gere les moteurs
  result = dxl_wb.ping(11, &model_number);
  result2 = dxl_wb.ping(12, &model_number2);
  dxl_wb.setExtendedPositionControlMode(12);
  dxl_wb.setPositionControlMode(11);
  result = dxl_wb.setVelocityBasedProfile(11);
  result = dxl_wb.itemWrite(11,"Profile_Velocity",80 );
  result = dxl_wb.itemWrite(11,"Profile_Acceleration",20);
  result = dxl_wb.itemWrite(11,"Drive_Mode",0);
  dxl_wb.torqueOn(11);
  dxl_wb.torqueOn(12);

  /** Initializing Servo
   */
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  
  /** Initializing base
   */
  dxl_wb.itemRead(12,"Present_Position",&position);
  servo.setupBase(position);                ///< Set the base in the initial position (face front)

  /** Initializing fingers and wrist
   */
  for(int increment = 0; increment<NB_FINGERS; increment++){
      servo.servoOut('5',increment);        ///< Set the hand in the initial position
  }
}

void loop() {
  /** Change string to individual caracteres
   */
  if (ROB.Get_state() == 1){                ///< Waiting for input
    if(Serial.available()>0){               ///< Waiting for the serial port to receive an input 
        msg = ROB.ReadMessage();            ///< Reading the string sent
        ROB.Get_status();                   ///< Sending the status
        ROB.SendMessage(msg);               ///< Sending the message received
    }
  }
  if (ROB.Get_state() == 2){                ///< The hand is executing a message
    delay(500);
    char caracteres[sizeof(msg)+1];         ///< Creation of a char array that will containt all the letters of the message
    strcpy(caracteres, msg.c_str());        ///< Transfering the message to a string then in different letter in the char array
    for (int i = 0; i < (msg.length()); i++){
      delay(2000);                          ///< Delay between each letter, in initial position
      Serial.print("Interface:start:");     ///< Message to interface : start letter
      Serial.println(caracteres[i]);
      for(int increment = 0; increment<NB_FINGERS; increment++){
        servo.servoOut(caracteres[i],increment); ///< Set the hand in the letter position
      }
      delay(2000);                          ///< Time of the letter in position
        for(int decrement = NB_FINGERS-1; decrement >= 0; decrement--){
          servo.reverseMove(caracteres[i],decrement); ///< Replace the hand in the initial position
        }
      Serial.print("Interface:done:");      ///< Message to interface : finish letter
      Serial.println(caracteres[i]);
    }
   ROB.Set_state(3);                        ///< All the letter of the string/message are done
    
  }
  if (ROB.Get_state() == 3){                ///< All the letter of the string/message are done
    delay(500);
    Serial.println("Interface:exit");       ///< Message to interface : exit demonstration
    ROB.SendMessage("IM DONE");
    msg = "";                               ///< Setting the message to empty
    ROB.Set_state(1);                       ///< The hand is ready to receive another message
    
  }    
  
}
