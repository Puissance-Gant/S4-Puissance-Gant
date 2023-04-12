/*
Le code de communication présenté ici est adapté à partir de l'exemple disponible ici : https://forum.arduino.cc/t/pc-arduino-comms-using-python-updated/574496

Il permet la communication sérielle rapide entre un script python et un script en c++ sous forme de fonction.

Le code en français représente les modifications apportées.
*/

#include <Arduino.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "controleMoteurs.hpp"

void replyToPython(String msg);

const byte numChars = 128;
//Complete list of recieved characters
char receivedChars[numChars];
String nouvelleDonnee;

boolean newData = false;
int tempsDepart = millis();

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
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'B': //Index
                    {
                        moteurs[INDEX].posGoalActu = nouvelleDonnee.toFloat();
                        changerPosMoteurs(moteurs[INDEX]);
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'C': //Majeur
                    {
                        moteurs[MAJEUR].posGoalActu = nouvelleDonnee.toFloat();
                        changerPosMoteurs(moteurs[MAJEUR]);
                        nouvelleDonnee = "";
                        break;
                    }
                    case 'D': //Poignet en inclinaison
                    {
                        moteurs[POIGNET].posGoalActu = nouvelleDonnee.toFloat();
                        changerPosMoteurs(moteurs[POIGNET]);
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

/*Envoyer un message périodique à python

Contient les informations du robot pour l'affichage*/
void replyToPython(String msg) {
    //if (newData == true) {
        //Changer les serial print afin de mettre le message voulu
        //Serial.print("<This just in ... ");
        if(msg != "")
        {
            Serial.print("<" + msg + ">");            
        }

        //Serial.print("   ");
        //Serial.print(millis());
        //Serial.print('>');
        if(newData)
            newData = false;
    //}
}