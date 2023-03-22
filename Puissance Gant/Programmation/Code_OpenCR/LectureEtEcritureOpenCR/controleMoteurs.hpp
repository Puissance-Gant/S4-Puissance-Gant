/*
Le code suivant permet de préparer et de faire tourner les moteurs pour la main articulée.

Pour changer les ID des moteurs dynamixel, il faut utiliser le Dynamixel Wizard 2.0
- Uploader le code d'example OpenCR\10 etc.\usb_to_dxl sur la carte OpenCR
- Connecter un moteur à la fois (l'ID par défaut est 1. Plusieurs moteurs avec le même ID ne sont pas détectés)
- Changer les paramètres de scan pour avoir le protocole 2.0, le port COM de la carte et un baud rate de 57600 seulement (plus rapide)
- lancer le scan
- changer l'ID du moteur pour celui que vous voulez (nous avons utilisé les 2 derniers chiffres du numéro de série écrit sur l'étiquette)
*/
#ifndef CONTROLE_MOTEURS
    #define CONTROLE_MOTEURS
    #include <Arduino.h>
    #include <stdlib.h>
    #include <Dynamixel2Arduino.h>

#endif

#pragma region moteurs    
#if defined(ARDUINO_OpenCR) // When using official ROBOTIS board with DXL circuit.
    // For OpenCR, there is a DXL Power Enable pin, so you must initialize and control it.
    // Reference link : https://github.com/ROBOTIS-GIT/OpenCR/blob/master/arduino/opencr_arduino/opencr/libraries/DynamixelSDK/src/dynamixel_sdk/port_handler_arduino.cpp#L78
    #define DXL_SERIAL   Serial3
    #define DEBUG_SERIAL Serial
    const int DXL_DIR_PIN = 84; // OpenCR Board's DIR PIN.
#endif
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

const float DXL_PROTOCOL_VERSION = 2.0;

using namespace ControlTableItem;
#pragma endregion

//Instance de moteur
struct Moteur{
    uint8_t id; //ID du moteur
    float posGoalActu; //but de position actuel (en degrés)
    uint8_t vitActu; //vitesse actuelle
};

const int8_t NB_MOTEURS = 5;
//
const int8_t POUCE = 0;
const int8_t INDEX = 1;
const int8_t MAJEUR = 2;
const int8_t POIGNET_INCL = 3;
const int8_t POIGNET_ROT = 4;

//Tableau contenant les informations des moteurs du robot
Moteur moteurs[NB_MOTEURS];

// Fonctions =====
//Changer l'objectif de positionnement et la vitesse d'un moteur
void changerPosMoteurs(Moteur mot)
{
    Serial.print("<" + String(mot.posGoalActu) + ">");
    dxl.setGoalPosition(mot.id, mot.posGoalActu, UNIT_DEGREE);
}

void setupMoteurs(Moteur mot)
{
    // Get DYNAMIXEL information
    Serial.print("<" + String(dxl.ping(mot.id)) + " : " + String(mot.id) + ">");

    // Turn off torque when configuring items in EEPROM area
    dxl.torqueOff(mot.id);
    dxl.setOperatingMode(mot.id, OP_POSITION);
    dxl.torqueOn(mot.id);

    dxl.writeControlTableItem(PROFILE_VELOCITY, mot.id, 50);
    changerPosMoteurs(mot);
}

