#include <Arduino.h>

const byte numChars = 64;
char receivedChars[numChars];

boolean newData = false;

byte ledPin = 13;   // the onboard LED

//===============

void setup() {
    Serial.begin(115200);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
    digitalWrite(ledPin, HIGH);

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
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
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
        Serial.print(millis());
        Serial.print('>');
            // change the state of the LED everytime a reply is sent
        digitalWrite(ledPin, ! digitalRead(ledPin));
        newData = false;
    }
}