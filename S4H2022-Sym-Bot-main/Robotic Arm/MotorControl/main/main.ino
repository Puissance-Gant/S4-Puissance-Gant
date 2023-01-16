/**
 *  Implement all the motor control part on OpenCr Board
 *    and communication management over Serial/Json
 * 
 *  Autors  : Handling-GRO, 65th promotion students, UdeS Genie de Robotique
 *  Date    : 2021-04-16
 * 
 *  Modification
 *    Initialise all the motors in setup
 *    Motor's Inheritance (Servomotor & Dynamixel)
 *    Use only CMD : GO_TO_JOINT_INDEX, GO_TO_HOME_INDEX & GO_TO_JOG
 *  Autors  : Sym-Bot, 66th promotion students, UdeS Genie de Robotique
 *  Date    : 2021-04-14
 * 
 */

// ============================================================================
// ================================= INCLUDE ==================================
// ============================================================================
#include <Arduino.h>

#include "MessageIO.h"
#include "SerialComm.h"

#include "DynamixMotor.h"
#include "ServoMotor.h"

// ============================================================================
// =============================== DEFINE/CONST ===============================
// ============================================================================
#define DEBUG 0                           // 0: Disable | 1 : Enable debug code
                                          // Comment to disable print
#define DEBUG_MSG(msg)                    // Serial.print(msg)
#define DEBUG_MSG_LN(msg)                 // Serial.println(msg)

#define BAUD              57600           // Serial's Baud

#define NbrMotorDynamixel 3
#define NbrMotorServo     1
const uint8_t NBR_MOTOR   = NbrMotorDynamixel + NbrMotorServo;

// ============================================================================
// ================================ ENUM/STRUCT ===============================
// ============================================================================
// Command index received from PI
enum CMD
{
  // ACTION IN [21, 29]
  GO_TO_JOINT_INDEX                =  1,       // USED BY SYM-BOT PROJECT
  GO_TO_HOME_INDEX                =  2,       // USED BY SYM-BOT PROJECT
  GO_TO_JOG_INDEX                 =  3,       // USED BY SYM-BOT PROJECT
  ACTION_OPEN_GRIPPER             =  5,
  ACTION_CLOSE_GRIPPER            =  6,
  ACTION_POSITION_GRIPPER         =  7,
  // SET VALUE IN [01, 09]
  SET_ZERO_POSITION_INDEX         = 11,
  SET_HOME_POSITION_INDEX         = 12,
  SET_MAX_POSITION_INDEX          = 13,
  // RETURN VALUE IN [11, 19]
  RETURN_ZERO_POSITION_INDEX      = 21,
  RETURN_HOME_POSITION_INDEX      = 22,
  RETURN_MAX_POSITION_INDEX       = 23,
  RETURN_ACTUAL_POSITION_INDEX    = 27
};

// ============================================================================
// ============================= VARIABLE GLOBALE =============================
// ============================================================================
MessageIO IO;
SerialComm SC(&Serial);
Motor *motors[NBR_MOTOR];

// ============================================================================
// ============================ DECLARED FUCNTION =============================
// ============================================================================
void init_dynamix();
void init_servo();

// ============================================================================
// =============================== SETUP/LOOP =================================
// ============================================================================
void setup() {
  // Initialise Serial Comm
  Serial.begin(BAUD);
  IO.addDevice(&SC);

  delay(10);

  // Initialise all motors and goto home each
  init_dynamix();
  init_servo();
}

void loop() {
  // MESSAGE HANDLING BLOCK, COMMUNICATION
  ControlMessage* msg = IO.readMessage(0);

  if (msg == nullptr) {
    // THIS CODE FOR TROUBLESHOOTING
    #if DEBUG == 1
      auto responce = ControlMessage(-1);
      IO.sendMessage(0,responce);
    #endif
  }
  else {
    // PAYLOAD MANAGEMENT FOR EACH ENGINE
    for (size_t i = 0; i < NBR_MOTOR; ++i) {
      // CODE FOR THE DIFFERENT OPERATIONS TO BE CARRIED OUT BY THE ENGINES
      switch (msg->getType()) {
        // VALUE THAT DOESN'T MATCH ANY OF THE CASE
        default:
          break;
        // CODE TO ACTION' MOTOR
        case GO_TO_JOINT_INDEX:
          motors[i]->go_to_degrees(msg->getPayload()[i]);
          break;
        case GO_TO_HOME_INDEX:
          motors[i]->go_to_home();
          break;
        case GO_TO_JOG_INDEX:
          if (msg->getPayload()[i] == 1)
            motors[i]->go_forward();
          else if (msg->getPayload()[i] == -1)
            motors[i]->go_backward();
          else { 
            // The arm doesn't move
          }
          break;
        case ACTION_OPEN_GRIPPER:
          if (i >= NbrMotorDynamixel)
            motors[i]->go_backward();
          break;
        case ACTION_CLOSE_GRIPPER:
          if (i >= NbrMotorDynamixel)
            motors[i]->go_forward();
          break;
        case ACTION_POSITION_GRIPPER:
          if (i >= NbrMotorDynamixel)
            motors[i]->go_to_degrees(msg->getPayload()[i]);
          break;
        
        //CODE TO SET INFORMATION
        case SET_ZERO_POSITION_INDEX:
          motors[i]->set_zero(msg->getPayload()[i]);
          break;
        case SET_HOME_POSITION_INDEX:
          motors[i]->set_home(msg->getPayload()[i]);
          break;
        case SET_MAX_POSITION_INDEX:
          motors[i]->set_max_limit(msg->getPayload()[i]);
          break;

        //CODE TO RETURN INFORMATION
        case RETURN_ZERO_POSITION_INDEX:
          msg->getPayload()[i] = (motors[i]->get_zero());
          break;
        case RETURN_HOME_POSITION_INDEX:
          msg->getPayload()[i] = motors[i]->get_home();
          break;
        case RETURN_MAX_POSITION_INDEX:
          msg->getPayload()[i] = motors[i]->get_max_limit();
          break;
        case RETURN_ACTUAL_POSITION_INDEX:
          msg->getPayload()[i] = (motors[i])->get_position();
          break;
      }
    }
    
    if(msg->getType() > 20) {
        ControlMessage response(msg->getType(), msg->getPayLoadSize(), msg->getPayload());
        IO.sendMessage(0, response);
    }

    // RELEASE msg FROM THE HEAP
    delete msg;
  }

  // delay(5);
}

// ============================================================================
// ============================ IMPLEMENT FUCNTION ============================
// ============================================================================
/* Function initialise the dynamixel motors and goto home */
void init_dynamix() {
  const uint8_t  IDs[]        = {    1,     2,     3};
  const uint16_t ZEROs[]      = {   10,  1360,  1136};
  const uint16_t HOMEs[]      = { 2047,  1920,  2160};
  const uint16_t MAXs[]       = { 4090,  2384,  3184};
  const uint16_t VELOCITYs[]  = {  128,    32,    32};
  const bool REVERSEs[]       = {false, false,  true};

  // INITIALIZE DYNAMIXEL MOTORS
  for (size_t m = 0; m < NbrMotorDynamixel; ++m) {
    if (motors[m] == nullptr) {
      motors[m] = new DynamixMotor(IDs[m], ZEROs[m], HOMEs[m], MAXs[m], VELOCITYs[m], REVERSEs[m]);
      DEBUG_MSG_LN("SET DYNAMIXEL");
    }
  }

  // GOTO HOME
  for (size_t m = 0; m < NbrMotorDynamixel; ++m) {
    motors[m]->go_to_home();

    DEBUG_MSG("Homing  ");
    DEBUG_MSG_LN(m);
  }
}

/* Function initialise the servo motors and goto home */
void init_servo() {
  const uint8_t IDs[]         = {3};

  // INITIALIZE SERVO MOTORS
  for (size_t m = NbrMotorDynamixel; m < NBR_MOTOR; ++m) {
    if (motors[m] == nullptr) {
      motors[m] = new ServoMotor(IDs[m-NbrMotorDynamixel]);
      DEBUG_MSG_LN("SET SERVO");
    }
  }

  // GOTO HOME
  for (size_t m = NbrMotorDynamixel; m < NBR_MOTOR; ++m) {
    motors[m]->go_to_home();

    DEBUG_MSG("Homing ");
    DEBUG_MSG_LN(m);
  }
}
