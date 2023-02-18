#include <Arduino.h>
#include <iostream>
#include <string>
#include <stdlib.h>

const byte numChars = 128;
//Complete list of recieved characters
char receivedChars[numChars];
String nouvelleDonnee;

boolean newData = false;

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
void replyToPython() {
    if (newData == true) {
        //Changer les serial print afin de mettre le message voulu
        //Serial.print("<This just in ... ");
        Serial.print("<" + String(receivedChars) + ">");
        //Serial.print("   ");
        //Serial.print(millis());
        //Serial.print('>');
        newData = false;
    }
}