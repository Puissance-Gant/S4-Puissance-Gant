#include <Arduino.h>
#include "WiFi.h"
#include "PubSubClient.h" // Connect and publish to the MQTT broker

// Wifi
// Il est important de noter que le wifi est utilisé en connection en point d'accès avec un ordinateur portable 
const char* ssid = "EQ7_puissance-gant";                // Nom du réseau personnelle
const char* wifi_password = "EQ7_puissance-gant";       // Mot de passe du réseau

// MQTT
// moscito est le mqtt broker utiliser du au fait qu'il est gratuit et open-source
const char* mqtt_server = "192.168.0.8";  // IP of the MQTT broker
const char* humidity_topic = "home/livingroom/humidity";
const char* temperature_topic = "home/livingroom/temperature";
const char* mqtt_username = "cdavid"; // MQTT username
const char* mqtt_password = "cdavid"; // MQTT password
const char* clientID = "client_livingroom"; // MQTT client ID

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


// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
// 1883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wifiClient); 

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
    // PLUS TARD
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
