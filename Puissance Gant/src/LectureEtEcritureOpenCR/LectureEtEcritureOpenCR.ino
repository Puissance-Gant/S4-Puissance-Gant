//#pragma region FreeRTOS include
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
// #ifdef INCLUDE_vTaskDelayUntil //S'assurer que la méthode vTaskDelayUntil est disponible
//     #undef INCLUDE_vTaskDelayUntil 
//     #define INCLUDE_vTaskDelayUntil 1
// #endif 
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

//#pragma endregion FreeRTOS include

#include <stdlib.h>
#include "lectureEtEcritureSerielle.hpp"


/*
Voici le code de fonctionnement de la carte OpenCR pour le projet Puissance Gant

Ce programme utilise les fichiers hpp controleMoteurs.hpp et lectureEtEcritureSerielle.hpp, soit deux fichiers créés par l'équipe.
La tâche de lecture des données est appelée à un intervalle régulier et transmet les données au moteurs.
Un message est envoyé à l'interface indiquant les positions actuelles des moteurs par après.
@AUTHORS : Ludovic Comtois (coml1502)
           Oscar Daniel Den Hartog (deno2501)
*/

void envoyerInfoMoteurs()
{
    static float a,b,c,d,e;

    a = random(0, 359);
    b = random(0, 359);
    c = random(0, 359);
    d = random(0, 359);
    e = random(0, 359);
    //Serial.println(String(a) + " " + String(b) + " " + String(c) + " " + String(d) + " " + String(e));

    moteurs[POUCE].posGoalActu = a;
    changerPosMoteurs(moteurs[POUCE]);
    moteurs[INDEX].posGoalActu = b;
    changerPosMoteurs(moteurs[INDEX]);
    // moteurs[MAJEUR].posGoalActu = c;
    // changerPosMoteurs(moteurs[MAJEUR]);
    moteurs[POIGNET_INCL].posGoalActu = d;
    changerPosMoteurs(moteurs[POIGNET_INCL]);
    moteurs[POIGNET_ROT].posGoalActu = e;
    changerPosMoteurs(moteurs[POIGNET_ROT]);
}

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
    Serial.println("Configuration des moteurs");
    for(int i = 0; i < NB_MOTEURS; i++)
        setupMoteurs(moteurs[i]);
}

//===============

void loop() 
{
    //Délai entre deux envois d'informations aux moteurs
    static const uint32_t DELAI = 100;

    static int tempsDepart;
    static int tempsFin;
    tempsDepart = millis();
    envoyerInfoMoteurs();
    tempsFin = millis();
    Serial.println("temps écoulé : " + String(tempsFin - tempsDepart));    
    delay(DELAI);// - (tempsFin - tempsDepart));

}