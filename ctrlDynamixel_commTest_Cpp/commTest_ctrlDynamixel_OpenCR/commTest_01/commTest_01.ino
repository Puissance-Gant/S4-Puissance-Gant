#include <Dynamixel2Arduino.h>
#include <actuator.h>
using namespace ControlTableItem;

#if defined(ARDUINO_OpenCR) 
  #define DXL_SERIAL   Serial3
  #define DEBUG_SERIAL Serial
  const int DXL_DIR_PIN = 84; // OpenCR Board's DIR PIN.
#endif

const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

struct Moteur{
  int id;           //ID du moteur
  int posGoalActu;  //but de position actuel
  int vitActu;     //vitesse actuelle
};


void setup() {

  //creation instance Moteur
  struct Moteur ID_POS_V;

  //creation var asignation
  int motID   = ID_POS_V.id;
  int motPOS  = ID_POS_V.posGoalActu;
  int motV    = ID_POS_V.vitActu;

  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID);

  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_POSITION);
  dxl.torqueOn(DXL_ID);

  // Limit the maximum velocity in Position Control Mode. Use 0 for Max speed
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID, 30);
}

void loop() {
  // put your main code here, to run repeatedly:

  //lire port seriel

  //
  dxl.setGoalPosition(DXL_ID, 1000);

  int i_present_position = 0;
  float f_present_position = 0.0;

  while (abs(1000 - i_present_position) > 10)
  {
    i_present_position = dxl.getPresentPosition(DXL_ID);
    DEBUG_SERIAL.print("Present_Position(raw) : ");
    DEBUG_SERIAL.println(i_present_position);
  }
  delay(1000);

  // Set Goal Position in DEGREE value
  dxl.setGoalPosition(DXL_ID, 5.7, UNIT_DEGREE);
  
  while (abs(5.7 - f_present_position) > 2.0)
  {
    f_present_position = dxl.getPresentPosition(DXL_ID, UNIT_DEGREE);
    DEBUG_SERIAL.print("Present_Position(degree) : ");
    DEBUG_SERIAL.println(f_present_position);
  }
  delay(1000);
}