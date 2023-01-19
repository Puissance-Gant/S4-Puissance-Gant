/** @file Communication.h
 * The file Communication.h contains the class COM_Arduino.
 */

 /** @class COM_Arduino
  * @brief Class containing functions related to the communication.
  *
  * This class contains the state, the message of the communication between OpenCr and the code.
  * @author RACINE, ISAAC
  * @date 13/04/2021
  */

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include <Arduino.h>

///< Class to initiate serial communication with the raspberryPi
class COM_Arduino
{
public:
  ///< Constructor of class COM_Arduino
  COM_Arduino()
  {
   state = state1;// initial state 
  };

  ///< Methode to send information (a string) to the raspberryPi
  void SendMessage(String str)
  {
    Serial.println(str);
  }

  ///< Methode to read information from the raspberryPi
  String ReadMessage()
  {
    String msg = Serial.readStringUntil('\n');
    if (msg == "")
    {
      msg = "Signal received but no message with it";
      return msg;
    }
    state = state2;
    return msg;
  };

  //< Methode to set a new state to the object
  void Set_state(int new_state)
  {
    state = new_state;
  };

  //< Methode to get the state of the object
  int Get_state()
  {
    return state; 
  };

  //< Methode to get the status of the object
  void Get_status()
  {
      if (state == state1)
    {
      Serial.println("The robot is wiating for an input");
    }
    if (state == state2)
    {
      Serial.println("Message received");
    }
    if (state == state3)
    {
      Serial.println("Robot is doing a sign");
    } 
  };  
private:
  //< Viariables for the state of the object
  int state;
  // Types of state possible
  int state1 = 1; // Waiting for input
  int state2 = 2; // Message received
  int state3 = 3; // robot is doing a sign
};

#endif  // _COMMUNICATION_H
