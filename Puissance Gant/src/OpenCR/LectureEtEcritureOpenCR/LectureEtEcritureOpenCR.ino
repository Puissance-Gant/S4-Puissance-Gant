#pragma region FreeRTOS include
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <RTOS.h>
#include <StackMacros.h>
#include <cmsis_os.h>
#include <croutine.h>
#include <deprecated_definitions.h>
#include <event_groups.h>
#include <list.h>
#include <mpu_wrappers.h>
#include <portable.h>
#include <portmacro.h>
#include <projdefs.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>
#include <timers.h>
#pragma endregion FreeRTOS include

#include "lectureEtEcritureSerielle.hpp"

/*
Voici le code de fonctionnement de la carte OpenCR pour le projet Puissance Gant

Ce programme utilise les fichiers hpp controleMoteurs.hpp et lectureEtEcritureSerielle.hpp, soit deux fichiers créés par l'équipe.
La tâche de lecture des données est appelée à un intervalle régulier et transmet les données au moteurs.
Un message est envoyé à l'interface indiquant les positions actuelles des moteurs par après.
Auteurs : Ludovic Comtois (coml1502)
          Oscar Daniel Den Hartog (deno2501)
*/

/*
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

//This namespace is required to use Control table item names
using namespace ControlTableItem;
#pragma endregion
*/
/*
const byte numChars = 128;
//const int numMoteurs = 5;
//Complete list of recieved characters
char receivedChars[numChars];
//Donnée recue qui doit être ajoutée à un moteur.
String nouvelleDonnee;
*/
/*
//Instance de moteur
struct Moteur{
    uint8_t id; //ID du moteur
    float posGoalActu; //but de position actuel (en degrés)
    uint8_t vitActu; //vitesse actuelle
}


Moteur moteurs[numMoteurs];
const int8_t POUCE = 0;
const int8_t INDEX = 1;
const int8_t MAJEUR = 2;
const int8_t POIGNET_INCL = 3;
const int8_t POIGNET_ROT = 4;

const int8_t NB_MOTEURS = 5;
*/
//boolean newData = false;

//byte ledPin = 13;   // the onboard LED

//===============
/*
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
*/

void setup() {
    Serial.begin(115200);
    while(!Serial);

    //Instanciation des moteurs
    moteurs[POUCE] = {31, 0, 50};
    moteurs[INDEX] = {36, 0, 50};
    moteurs[MAJEUR] = {37, 0, 50};
    moteurs[POIGNET_INCL] = {17, 0, 50};
    moteurs[POIGNET_ROT] = {19, 0, 50};

    //Setup des moteurs
    // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
    dxl.begin(57600);
    // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
    dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

    for(int i = 0; i < NB_MOTEURS; i++)
        setupMoteurs(moteurs[i]);
}

//===============

void loop() {
    recvWithStartEndMarkers();
    replyToPython();
}

//===============

/*
//Changer l'objectif de positionnement et la vitesse d'un moteur
void changerPosMoteurs(Moteur mot)
{
    Serial.print("<" + String(mot.posGoalActu) + ">");
    bool res = dxl.setGoalPosition(mot.id, mot.posGoalActu, UNIT_DEGREE);
    if(res)
        Serial.print("<Suck my benis you bitch>");
    else
        Serial.print("<No benises?>");
}
*/
/*
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    static char startMarker = '<';
    static char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        //Recieved character
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;

                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                    nouvelleDonnee = "";
                }
                switch(rc)
                {
                    case 'A': //Pouce
                    {
                        moteurs[POUCE].posGoalActu = nouvelleDonnee.toFloat();
                        changerPosMoteurs(moteurs[POUCE]);
                        //Serial.println("<" + nouvelleDonnee + "A" + ">");
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'B': //Index
                    {
                        moteurs[INDEX].posGoalActu = nouvelleDonnee.toFloat();
                        changerPosMoteurs(moteurs[INDEX]);
                        //Serial.println("<" + String(nouvelleDonnee.toInt()) + ">");
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'C': //Majeur
                    {
                        moteurs[MAJEUR].posGoalActu = nouvelleDonnee.toFloat();
                        changerPosMoteurs(moteurs[MAJEUR]);
                        //Serial.println("<" + nouvelleDonnee + "C" + ">");
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'D': //Poignet en inclinaison
                    {
                        moteurs[POIGNET_INCL].posGoalActu = nouvelleDonnee.toFloat();
                        changerPosMoteurs(moteurs[POIGNET_INCL]);
                        //Serial.println("<" + nouvelleDonnee + "D" + ">");
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'E': //Poignet en rotation
                    {
                        moteurs[POIGNET_ROT].posGoalActu = nouvelleDonnee.toFloat();
                        changerPosMoteurs(moteurs[POIGNET_ROT]);
                        //Serial.println("<" + nouvelleDonnee + "E" + ">");
                        nouvelleDonnee = "";
                        break;
                    }
                    default:
                    {
                        nouvelleDonnee += rc;
                        break;
                    }
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//===============

//Changer les serial print afin de mettre le message voulu
void replyToPython() {
    if (newData == true) {
        Serial.print("<This just in ... ");
        Serial.print(receivedChars);
        Serial.print("   ");
        //Serial.print(millis());
        Serial.print('>');
            // change the state of the LED everytime a reply is sent
        digitalWrite(ledPin, ! digitalRead(ledPin));
        newData = false;
    }
}
*/
