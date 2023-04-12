#include <Arduino.h>
#include "WiFi.h"
#include "PubSubClient.h" // Connect and publish to the MQTT broker

// Pins
#define POUCE 36
#define INDEX 39
#define MAJEUR 34
#define BOUTON 35


// Communication:
String serialData; // String qui est envoyé en Serial au pi4 pour êytre lut et transmit au OpenCR
/// Structure qui contient toute les information de la main
struct Hand
{
    static const uint8_t DOIGTS = 3; // Max number of fingers with flexs
    static const uint8_t AXIS = 3;   // Number of MPU9250's axis data to send

    int pin[DOIGTS] = {POUCE, INDEX, MAJEUR};
    uint16_t fingers[DOIGTS] = {0, 0, 0}; // Order for the right hand: {POUCE INDEX MAJEUR}
    float max[DOIGTS] = {3072, 4096, 4096};     // Valeur maximum des resistances
    float min[DOIGTS] = {1024, 0, 0};     // Zero


    float imu[AXIS] = {0, 0, 0}; // Order for data: {LINEAR_X LINEAR_Y ROTATION_Z}

    String CALIBRATION = "LOW";
} hand = {};


// Wifi
// Il est important de noter que le wifi est utilisé en connection en point d'accès avec un ordinateur portable ou un cellulaire
const char* ssid = "ludo";                // Nom du réseau personnelle
const char* wifi_password = "puissance";           // Mot de passe du réseau

// MQTT
// mosquitto est le mqtt broker utiliser du au fait qu'il est gratuit et open-source
const char* mqtt_server = "192.168.137.254";  // IP of the MQTT broker
const char* resistance_topic = "Eq7_PuissanceGant_S4/gant/commandes";
const char* calibration_topic = "Eq7_PuissanceGant_S4/gant/calibration";
const char* mqtt_username = "puissance"; // MQTT username
const char* mqtt_password = "puissance"; // MQTT password
const char* clientID = "gant"; // MQTT client ID

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

// Parametre pour l'accelerometre
void setupIMUSensor()
{
    // PLUS TARD
}

// Connexion au Wifi
void connect_WIFI(){
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // Connect to the WiFi
    WiFi.begin(ssid, wifi_password);

    // Wait until the connection has been confirmed before continuing
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println(".");
    Serial.println("Wifi connected");
}

void connect_MQTT(){
    // Debugging - Output the IP Address of the ESP8266
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (client.connect(clientID, mqtt_username, mqtt_password)) {
     Serial.println("Connected to MQTT Broker!");
    }
    else {
        Serial.println("Connection to MQTT Broker failed...");
    }
}
/*-------------------------------------------------- loop functions --------------------------------------------------*/

/// Gathers flex sensors data
void flexSensorsData()
{
    for (int finger = 0; finger < hand.DOIGTS; finger++)
    {
        int val = analogRead(hand.pin[finger]);

        if  (val >= hand.max[finger])
        {
            hand.fingers[finger] = 100; 
        }
        else if (val <= hand.min[finger])
        {
            hand.fingers[finger] = 0; 
        }

        else if ( hand.min[finger] <  val < hand.max[finger])
        {
            hand.fingers[finger] = round((val - hand.min[finger]) / (hand.max[finger] - hand.min[finger]) * 100) ;
        }
    }
}


void calibration()
{
    Serial.println(digitalRead(BOUTON)); 
}

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
    pinMode(BOUTON, INPUT);

    delay(1000);
    // Sensors and UI setup:
    setupIMUSensor();
    setupFlexSensors();

    connect_WIFI();
    connect_MQTT();
}

void loop()
{
    serialData = "";

    // Prend 
    flexSensorsData();

    // Send data:
    stringToSend();
    Serial.println(String(serialData));

    // resistance_topic
    if (client.publish(resistance_topic, String(serialData).c_str())) {
        // Serial.println("resistance sent!");
    }

    else {
        //Serial.println("Resistance failed to send. Reconnecting to MQTT Broker and trying again");
        client.connect(clientID, mqtt_username, mqtt_password);
        delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
        client.publish(resistance_topic, String(serialData).c_str());
    }

    // reset_topic

    if (client.publish(calibration_topic, String("Je suis un bouton").c_str())) {
        //Serial.println("Reset sent!");
    }

    else {
        //Serial.println("Reset failed to send. Reconnecting to MQTT Broker and trying again");
        client.connect(clientID, mqtt_username, mqtt_password);
        delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
        client.publish(calibration_topic, String("Je suis un bouton").c_str());
    }

    delay(150);
}