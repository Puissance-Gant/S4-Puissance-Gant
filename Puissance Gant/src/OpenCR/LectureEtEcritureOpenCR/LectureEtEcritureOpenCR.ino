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
#include <Arduino.h>
#include <iostream>
#include <string>

/*
Le code de communication présenté ici est adapté à partir de l'exemple disponible ici : https://forum.arduino.cc/t/pc-arduino-comms-using-python-updated/574496

Il permet la communication sérielle rapide entre un script python et un script en c++ sous forme de fonction.

Le code en français représente les modifications apportées.
*/

const byte numChars = 128;
const int numMoteurs = 5;
//Complete list of recieved characters
char receivedChars[numChars];
//Donnée recue qui doit être ajoutée à un moteur.
String nouvelleDonnee;

//Instance de moteur
struct Moteur{
    int id; //ID du moteur
    int posGoalActu; //but de position actuel (en degrés)
    int vitActu; //vitesse actuelle
};

Moteur moteurs[numMoteurs];
const int8_t POUCE = 0;
const int8_t INDEX = 0;
const int8_t MAJEUR = 0;
const int8_t POIGNET_INCL = 0;
const int8_t POIGNET_ROT = 0;

boolean newData = false;

byte ledPin = 13;   // the onboard LED

//===============

void setup() {
    Serial.begin(115200);

    //Instanciation des moteurs
    moteurs[POUCE] = {1, 0, 50};
    moteurs[INDEX] = {2, 0, 50};
    moteurs[MAJEUR] = {3, 0, 50};
    moteurs[POIGNET_INCL] = {4, 0, 50};
    moteurs[POIGNET_ROT] = {5, 0, 50};


    Serial.println("<Arduino is ready>");
}

//===============

void loop() {
    recvWithStartEndMarkers();
    replyToPython();
}

//===============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
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
                }
                switch(rc)
                {
                    case 'A': //Pouce
                    {
                        moteurs[POUCE].posGoalActu = nouvelleDonnee.toInt();
                        Serial.println("<" + nouvelleDonnee + "A" + ">");
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'B': //Index
                    {
                        moteurs[INDEX].posGoalActu = nouvelleDonnee.toInt();
                        Serial.println("<" + nouvelleDonnee + "B" + ">");
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'C': //Majeur
                    {
                        moteurs[MAJEUR].posGoalActu = nouvelleDonnee.toInt();
                        Serial.println("<" + nouvelleDonnee + "C" + ">");
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'D': //Poignet en inclinaison
                    {
                        moteurs[POIGNET_INCL].posGoalActu = nouvelleDonnee.toInt();
                        Serial.println("<" + nouvelleDonnee + "D" + ">");
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'E': //Poignet en rotation
                    {
                        moteurs[POIGNET_ROT].posGoalActu = nouvelleDonnee.toInt();
                        Serial.println("<" + nouvelleDonnee + "E" + ">");
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