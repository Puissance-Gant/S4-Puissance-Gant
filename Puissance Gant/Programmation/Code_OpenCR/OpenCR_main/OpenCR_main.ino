//#include <Arduino_FreeRTOS.h>
#include <string.h>
#include <iostream>
#include "lectureEtEcritureSerielle.hpp"


/*
Voici le code de fonctionnement de la carte OpenCR pour le projet Puissance Gant

Ce programme utilise les fichiers hpp controleMoteurs.hpp et lectureEtEcritureSerielle.hpp, soit deux fichiers créés par l'équipe.
La tâche de lecture des données est appelée à un intervalle régulier et transmet les données au moteurs.
Un message est envoyé à l'interface indiquant les positions actuelles des moteurs par après.
Auteur : Ludovic Comtois (coml1502)
*/

void setup() {
    Serial.begin(256000);
    while(!Serial);

    //Instanciation des moteurs
    moteurs[POUCE] = {31, 0, 100, 220, 63};
    moteurs[INDEX] = {36, 0, 100, 330, 130};
    moteurs[MAJEUR] = {37, 0, 100, 21, 235};
    moteurs[POIGNET] = {17, 0, 50, 105, 55};

    // Setup des moteurs
    // Démarrer le dxl avec un baudrate de 57600, le même que les moteurs
    dxl.begin(57600);
    // Déterminer la version du protocole du dxl
    dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

    for(int i = 0; i < NB_MOTEURS; i++)
        setupMoteurs(moteurs[i]);
}

//===============
void loop() {
    //static String puissance = "";
    //static uint32_t tempsPrec = millis();

    recvWithStartEndMarkers();
    
    // À chaque fois qu'on reçoit un nouveau message, vérifier pour envoyer la puissance. 
    // Commenté pour cause de performance de l'OpenCR
    //if(millis() - tempsPrec > 1000) 
    //{
    //    tempsPrec = millis();
    //    puissance = getPuissanceMoteurs();
    //    replyToPython(puissance);
        //puissance = "";
    //}
    
    replyToPython("");
}