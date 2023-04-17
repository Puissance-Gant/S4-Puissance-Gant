#include <Arduino.h>
#include <stdlib.h>
#include <Dynamixel2Arduino.h>
#include <math.h>
#include <stdlib.h>
#include "controleMoteurs.hpp"
   
    int POS_POU = 0;
    int POS_IND = 0;
    int POS_MAJ = 0;

void setup() {
    Serial.begin(256000);
    while(!Serial);

    //Instanciation des moteurs
    moteurs[POUCE] = {31, 0, 70};
    moteurs[INDEX] = {36, 0, 70};
    moteurs[MAJEUR] = {37, 0, 81};
    moteurs[POIGNET_INCL] = {17, 0, 20};
    moteurs[POIGNET_ROT] = {19, 0, 10};
    
    
    // POS main ouvert moteurs
    //moteurs[POUCE] = (220);
    //moteurs[INDEX] = (330);
    //moteurs[MAJEUR] = (111);
    //POIGNET_INCL = (105)

    //pouce : 176.44
    //index : 300.78
    //majeur : 147.14

    //pouce : 17.6
    //index : 81.31
    //majeur : 40.48

    // POS main ferme moteurs
    //moteurs[POUCE] = (63);
    //moteurs[INDEX] = (130);
    //moteurs[MAJEUR] = (325);
    //POIGNET_INCL(55.26)
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
  while (dxl.getPresentPosition(moteurs[INDEX].id, UNIT_DEGREE)<325)
   {
    POS_POU = 220;
    POS_IND = 330;
    POS_MAJ = 110;
    
    
     moteurs[POUCE].posGoalActu = POS_POU;
    changerPosMoteurs(moteurs[POUCE]);
    moteurs[INDEX].posGoalActu = POS_IND;
   changerPosMoteurs(moteurs[INDEX]);
     moteurs[MAJEUR].posGoalActu = POS_MAJ;
    changerPosMoteurs(moteurs[MAJEUR]);
    moteurs[POIGNET_INCL].posGoalActu = 105;
   changerPosMoteurs(moteurs[POIGNET_INCL]);
    
    Serial.println(dxl.getPresentPosition(moteurs[MAJEUR].id, UNIT_DEGREE));
   }
 while (dxl.getPresentPosition(moteurs[INDEX].id, UNIT_DEGREE)>135 )
   { 
    POS_POU = 63;
    POS_IND = 130;
    POS_MAJ = 325;
     
     moteurs[POUCE].posGoalActu = POS_POU;
    changerPosMoteurs(moteurs[POUCE]);
    moteurs[INDEX].posGoalActu = POS_IND;
   changerPosMoteurs(moteurs[INDEX]);
     moteurs[MAJEUR].posGoalActu = POS_MAJ;
    changerPosMoteurs(moteurs[MAJEUR]);
    moteurs[POIGNET_INCL].posGoalActu = 55;
   changerPosMoteurs(moteurs[POIGNET_INCL]);
    Serial.println(dxl.getPresentPosition(moteurs[MAJEUR].id, UNIT_DEGREE));
    }

   
  Serial.println(dxl.getPresentPosition(moteurs[MAJEUR].id, UNIT_DEGREE)); //use angle in degree
 
  
}
