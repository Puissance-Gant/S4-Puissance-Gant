#include <Arduino.h>
#include <SparkFun_ADS1015_Arduino_Library.h>
#include <MPU9250.h>
#include <Wire.h>  
#include "BluetoothSerial.h"
#include "analogWrite.h"

//Structs:

/// RGB color struct
struct Color{
  uint8_t red; // R in (R,G,B)
  uint8_t green; // G in (R,G,B)
  uint8_t blue; // B in (R,G,B)
};

/// Struct that contains all the data from the glove
struct Hand{
  static const uint8_t FINGERS = 3; // Max number of fingers with flexs
  static const uint8_t AXIS = 3; // Number of MPU9250's axis data to send

  uint16_t max[FINGERS] = {0, 0, 0, 0}; // Contains the initial values of flexs
  uint16_t fingers[FINGERS] = {0, 0, 0, 0}; // Order for the right hand: {INDEX MIDDLE RING PINKY}
  uint16_t offsets[FINGERS] = {0,0,0,0};// Offset that can be applied if flex's data are different 
  float imu[AXIS] = {0, 0, 0}; // Order for data: {LINEAR_X LINEAR_Y ROTATION_Z}
} hand = {};

// Const:
const unsigned MILLISECONDS = 10; // DELAY  
const uint8_t MODE_MAX = 3; // Max number of modes
const Color RED     = {255,  0,  0}; // FATAL ERROR
const Color ORANGE  = {255,155,  0}; // JOG MODE
const Color GREEN   = { 70,255,  0}; // CARTESIAN MODE
const Color BLUE    = {  0,240,255}; // AI MODE
// Pins:
#define LED         2
#define PIN_RED     18
#define PIN_GREEN   19
#define PIN_BLUE    5
#define BUTTON      23

// Mode:

uint8_t mode = 0;
Color colorsModes[MODE_MAX] = {ORANGE, GREEN, BLUE};

// Flex Sensors:

ADS1015 pinkySensor;
ADS1015 indexSensor;

// IMU:

MPU9250 mpu;

// Communication:

BluetoothSerial SerialBT;
String serialData;

// Flags:

bool isFatalError[2] = {false, false};

/*-------------------------------------------------- setup functions --------------------------------------------------*/

/// Setting up ESP32's bluetooth
void setupBluetooth() {
  SerialBT.begin("ESP32-Symbot-Glove"); //Bluetooth device name
}

/// Setting up two Sparkfun flex sensors with GND(0x48) and SDA(0x4A) addresses
void setupFlexSensors(){
  //Set up each sensor, change the addresses based on the location of each sensor
    if (indexSensor.begin(ADS1015_ADDRESS_GND) == false) {
     SerialBT.println("Index not found. Check wiring.");
     isFatalError[0] = true;
  } 
  if (pinkySensor.begin(ADS1015_ADDRESS_SDA) == false) {
     SerialBT.println("Pinky not found. Check wiring.");
     isFatalError[0] = true;
  }
  indexSensor.setGain(ADS1015_CONFIG_PGA_TWOTHIRDS); // Gain of 2/3 to works well with flex glove board voltage swings (default is gain of 2)
  pinkySensor.setGain(ADS1015_CONFIG_PGA_TWOTHIRDS); // Gain of 2/3 to works well with flex glove board voltage swings (default is gain of 2)
  
  for (int finger = 0; finger < 2; finger++) {
    hand.max[finger] = indexSensor.getAnalogData(!finger);
    hand.max[finger + 2] = pinkySensor.getAnalogData(!finger);
  }
}

/// Setting up MPU9250 9-DOF IMU I2C(0x68) with accel and gyro calibration
void setupIMUSensor(){
  isFatalError[1] = !mpu.setup(0x68);
  if (isFatalError[1]){
    SerialBT.println("MPU9250 not found. Check wiring.");
  } else {
    mpu.calibrateAccelGyro();
  }
}

/// Setting up ALL LEDs pins mode
void setupLEDs(){
  pinMode(LED, OUTPUT);
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
}

/// Setting up pin for button mode
void setupButton(){
  pinMode(BUTTON, INPUT_PULLUP);
}

/*-------------------------------------------------- loop functions --------------------------------------------------*/

/// Gathers flex sensors data 
void flexSensorsData(){
  for (int finger = 0; finger < 4; finger++){
    hand.fingers[finger] = 0;
  }
  for (int finger = 0; finger < 2; finger++) {
    if (indexSensor.getAnalogData(!finger) < hand.max[finger]){
      hand.fingers[finger] = hand.max[finger] - indexSensor.getAnalogData(!finger) - hand.offsets[finger];
      if (hand.fingers[finger] > 500){
        hand.fingers[finger] = 0;
      }
    }
    if (pinkySensor.getAnalogData(!finger) < hand.max[finger + 2]){
      hand.fingers[finger + 2] = hand.max[finger + 2] - pinkySensor.getAnalogData(!finger) - hand.offsets[finger + 2];
      if (hand.fingers[finger + 2] > 500){
        hand.fingers[finger + 2] = 0;
      }
    }
  }
}

/// Gathers MPU9250 sensor's data
void IMUSensorData(){
  if (mpu.update()) {
    hand.imu[0] = 0;
    hand.imu[1] = 0;
    // Author's Note:
    // imu[0] and imu[1] were finally not used in the modes of translate.py that is in Application folder of the Project
    // We mainly wanted to use them to get the position of the hand in x(AccY) and y(AccZ) to transpose them to the robotic arm in real time
    // but didn't had the time to continue working on it
    // So, here's a piece of incomplete code to try getting the position...
    /*
    hand.imu[0] += mpu.getLinearAccY()*9.81f*(MILLISECONDS/1000.0); //Distance
    if ((mpu.getAccY() >= 0.05f || mpu.getAccY() <= -0.05f) && mpu.getAccZ() > 0.85f){ // (if the hand is well oriented we add acceleration)
      hand.imu[0] += 0;
    }
    else {
      hand.imu[0] += mpu.getLinearAccY()*9.81f*(MILLISECONDS/1000.0);
    }

    hand.imu[1] = mpu.getLinearAccZ()*9.81f; //Height 
    if ((mpu.getAccY() >= 0.05f || mpu.getAccY() <= -0.05f) && mpu.getAccZ() > 0.85f){ // (if the hand is well oriented we add acceleration)
      hand.imu[1] = 0;
    }
    */
    //Hand orientation:
    hand.imu[2] = mpu.getAccZ(); //Rotation
    if (hand.imu[2] <= 0.15f && hand.imu[2] >= -0.15f){
      hand.imu[2] = 0;
    }
  }
}
 
/**
 * Changes RGB LED colors
 * @param r R value from (R,G,B)
 * @param g G value from (R,G,B)
 * @param b B value from (R,G,B)
 */
void setColor(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(PIN_RED,   r, 2048);
  analogWrite(PIN_GREEN, g, 2048);
  analogWrite(PIN_BLUE,  b, 2048);
}

///Manages LEDs behaviour
void activationLEDs(){
  if (hand.imu[2] == 0.0f){
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
  setColor(colorsModes[mode].red, colorsModes[mode].green, colorsModes[mode].blue);
}

/**
 * Debounce Algorithm
 * It's mainly for fixing button readings instead of using the attachInterrupt 
 * and RISING (it's an alternative)
 */
void buttonRead(){
  static uint8_t lastButtonState=0;
  static uint32_t debounceDelay = 50;
  static uint32_t lastDebounceTime = 0;
  static uint8_t buttonState = 0;
  // read the state of the switch into a local variable:
  uint8_t reading = digitalRead(BUTTON);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      // only toggle the LED if the new button state is HIGH(PULLDOWN) / LOW(PULLUP)
      if (buttonState == LOW) {
        mode = (mode+1) % MODE_MAX;
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

/**
 * Formats the serial communication protocol to send it as a string:
 * MODE|INDEX|MIDDLE|RING|PINKY|X|Y|Z-ROTATION|
 */
void stringToSend(){
  serialData = String(mode) + "|";

  for (int finger = 0; finger < 4; finger++){
  //   //serialData += finger + ":";
  //   // serialData += hand[finger] + ";";
    serialData += String(hand.fingers[finger]) + "|";
  }

  for (int i = 0; i < 3; i++){
    // switch(i){
    //   case 0:
    //     serialData += "Y:";
    //     break;
    //   case 1:
    //     serialData += "Z:";
    //     break;
    //   case 2:
    //     serialData += "R:";
    //     break;
    // }
    serialData += String(hand.imu[i]) + "|";
  }
}

/*-------------------------------------------------- main functions --------------------------------------------------*/

void setup() {
  //Communication setup:
  Wire.begin();
  Serial.begin(115200);
  setupBluetooth();
  delay(1000);
  //Sensors and UI setup:
  setupLEDs();
  setupButton();
  setupIMUSensor();
  setupFlexSensors();
  // Errors flags raised verification:
  while (isFatalError[0] || isFatalError[1]){
    // red led of death:
    setColor(RED.red, RED.green, RED.blue);
  }
}

void loop() {
  // Gathers data:
  flexSensorsData();
  IMUSensorData();
  // UI States:
  activationLEDs();
  buttonRead();
  //Send data:
  stringToSend();
  Serial.println(serialData);
  SerialBT.println(serialData);

  delay(MILLISECONDS);
}
