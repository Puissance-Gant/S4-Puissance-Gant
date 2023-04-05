//#include <Arduino_FreeRTOS.h>
#include <string.h>
#include <iostream>
#include "lectureEtEcritureSerielle.hpp"


/*
Voici le code de fonctionnement de la carte OpenCR pour le projet Puissance Gant

Ce programme utilise les fichiers hpp controleMoteurs.hpp et lectureEtEcritureSerielle.hpp, soit deux fichiers créés par l'équipe.
La tâche de lecture des données est appelée à un intervalle régulier et transmet les données au moteurs.
Un message est envoyé à l'interface indiquant les positions actuelles des moteurs par après.
Auteurs : Ludovic Comtois (coml1502)
          Oscar Daniel Den Hartog (deno2501)
*/

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

//===============
void loop() {
    static String puissance = "";
    //static int i = 0;
    //static const int NB_MSG_RECU_AVANT_ENVOI = 100;
    static uint32_t tempsPrec = millis();
    recvWithStartEndMarkers();
    replyToPython("");

    // Envoyer la puissance consommée chaque 500 ms
    if(millis() - tempsPrec > 500) 
    {
        tempsPrec = millis();
        puissance = getPuissanceMoteurs();
        replyToPython(puissance);
        puissance = "";
    }
 
}