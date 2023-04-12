#include <Arduino.h>
#include <stdlib.h>
#include <Dynamixel2Arduino.h>
#include <math.h>
#include <stdlib.h>
#include "controleMoteurs.hpp"


void setup() {
    Serial.begin(256000);
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

void loop() {
  // put your main code here, to run repeatedly:
    static int pos = 0;
    //pos = (pos+10);
    moteurs[POIGNET_ROT].posGoalActu = pos;
    Serial.println(changerPosMoteurs(moteurs[POIGNET_ROT]));
    delay(500);

}
