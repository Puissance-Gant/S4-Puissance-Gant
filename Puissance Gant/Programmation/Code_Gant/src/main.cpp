#include <Arduino.h>
#include <Wire.h>
#include "analogWrite.h"

// Const:
const unsigned MILLISECONDS = 10; // DELAY
const uint8_t MODE_MAX = 3;       // Max number of modes

// Pins
#define POUCE 34
#define INDEX 35
#define MAJEUR 32

// IMU:
// plus tard

// Communication:
String serialData; // String qui est envoyé en Serial au pi4 pour êytre lut et transmit au OpenCR

// Struct:
/// Structure qui contient toute les information de la main
struct Hand
{
    static const uint8_t DOIGTS = 3; // Max number of fingers with flexs
    static const uint8_t AXIS = 3;   // Number of MPU9250's axis data to send

    int pin[DOIGTS] = {POUCE, INDEX, MAJEUR};
    uint16_t fingers[DOIGTS] = {0, 0, 0}; // Order for the right hand: {POUCE INDEX MAJEUR}
    uint16_t max[DOIGTS] = {0, 0, 0};     // Valeur maximum des resistances
    uint16_t min[DOIGTS] = {0, 0, 0};     // Zero

    float imu[AXIS] = {0, 0, 0}; // Order for data: {LINEAR_X LINEAR_Y ROTATION_Z}
} hand = {};

/*-------------------------------------------------- setup functions --------------------------------------------------*/
// Mode des pin pour la lecture des données
void setupFlexSensors()
{
    pinMode(POUCE, INPUT);
    pinMode(INDEX, INPUT);
    pinMode(MAJEUR, INPUT);
}

/// Parametre pour l'accelerometre
void setupIMUSensor()
{

    // PLUS TARS
    /* isFatalError[1] = !mpu.setup(0x68);
    if (isFatalError[1]){
      SerialBT.println("MPU9250 not found. Check wiring.");
    } else {
      mpu.calibrateAccelGyro();
    } */
}

/*-------------------------------------------------- loop functions --------------------------------------------------*/

/// Gathers flex sensors data
void flexSensorsData()
{
    for (int finger = 0; finger < hand.DOIGTS; finger++)
    {
        hand.fingers[finger] = analogRead(hand.pin[finger]) / 4095.0 * 360;
    }
}

/// Gathers MPU9250 sensor's data
// À FAIRE PLUS TARD

/**
 * Formats the serial communication protocol to send it as a string:
 * pouceAindexBmajeurC
 */
void stringToSend()
{
    serialData += "<";
    for (int finger = 0; finger < hand.DOIGTS; finger++)
    {
        serialData += String(hand.fingers[finger]);

        switch (hand.pin[finger])
        {
        case POUCE:
            serialData += String("A");
            break;

        case INDEX:
            serialData += String("B");
            break;
            
        case MAJEUR:
            serialData += String("C");
            break;
        
        default:
            break;
        }
    }
    serialData += ">";
}

/*-------------------------------------------------- main functions --------------------------------------------------*/

void setup()
{
    // Communication setup:
    Wire.begin();
    Serial.begin(115200);

    delay(1000);
    // Sensors and UI setup:
    setupIMUSensor();
    setupFlexSensors();
}

void loop()
{
    // Gathers data:
    flexSensorsData();

    // Send data:
    stringToSend();
    Serial.println(serialData);

    serialData = "";

    delay(500);
}
