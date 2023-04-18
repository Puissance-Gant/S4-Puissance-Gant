/*
Le code suivant permet de préparer et de faire tourner les moteurs pour la main articulée.

Pour changer les ID des moteurs dynamixel, il faut utiliser le Dynamixel Wizard 2.0
- Uploader le code d'example OpenCR\10 etc.\usb_to_dxl sur la carte OpenCR
- Connecter un moteur à la fois (l'ID par défaut est 1. Plusieurs moteurs avec le même ID ne sont pas détectés)
- Changer les paramètres de scan pour avoir le protocole 2.0, le port COM de la carte et un baud rate de 57600 seulement (plus rapide)
- lancer le scan
- changer l'ID du moteur pour celui que vous voulez (nous avons utilisé les 2 derniers chiffres du numéro de série écrit sur l'étiquette)
*/
#ifndef CONTROLE_MOTEURS
    #define CONTROLE_MOTEURS
    #include <Arduino.h>
    #include <stdlib.h>
    #include <Dynamixel2Arduino.h>
    #include <math.h>

#endif

#pragma region moteurs    
// Cette région contient du code tiré des exemples de la librairie dynamixel

#if defined(ARDUINO_OpenCR) // When using official ROBOTIS board with DXL circuit.
    // For OpenCR, there is a DXL Power Enable pin, so you must initialize and control it.
    // Reference link : https://github.com/ROBOTIS-GIT/OpenCR/blob/master/arduino/opencr_arduino/opencr/libraries/DynamixelSDK/src/dynamixel_sdk/port_handler_arduino.cpp#L78
    #define DXL_SERIAL   Serial3
    #define DEBUG_SERIAL Serial
    const int DXL_DIR_PIN = 84; // OpenCR Board's DIR PIN.
#endif
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

const float DXL_PROTOCOL_VERSION = 2.0;

// Tension d'alimentation des moteurs
const int V_IN_MOTEURS = 12;

using namespace ControlTableItem;

struct Moteur{
    uint8_t id;         // ID du moteur
    float posGoalActu;  // But de position actuel (en pourcents) 0 = Ouvert, 100 = Fermé
    uint8_t vitActu;    // Vitesse actuelle
    uint16_t posOuvert; // Angle du moteur lorsque la main est ouverte
    uint16_t posFerme;  // Angle du moteur lorsque la main est fermée
};

#pragma endregion

//Instance de moteur

const int8_t NB_MOTEURS = 4;

const int8_t POUCE = 0;
const int8_t INDEX = 1;
const int8_t MAJEUR = 2;
const int8_t POIGNET = 3;

//Tableau contenant les informations des moteurs du robot
Moteur moteurs[NB_MOTEURS];

// Fonctions =====

//Changer l'objectif de positionnement d'un moteur
void changerPosMoteurs(Moteur mot)
{
    float pente = (float)(mot.posFerme - mot.posOuvert)/100;


    // Le delta représente la différence de position que les moteurs doivent prendre selon l'inclinaison du poignet
    // Le poignet raccourcit/rallonge les cordes selon son inclinaison. On doit ajouter ce delta pour garder la position voulue de la main.
    float delta;
    Moteur poignet = moteurs[POIGNET];
    // Ces valeurs ont été calculées selon l'assemblage de la main de l'équipe. Il faudra changer le delta selon vos emplacements.
    // Nous avons mesuré la position des doigts lorsque le poignet est droit, main ouvert et fermée, puis plié avec la main ouverte et fermée.
    // De cela, le delta est la plage linéaire indiquant la différence de position des moteurs selon l'angle du poignet pour garder
    // le doigt voulu à la même position
    if(mot.id == moteurs[POUCE].id)
        delta = 45*poignet.posGoalActu/100;
    else if(mot.id == moteurs[INDEX].id)
        delta = (0.19*mot.posGoalActu + 30)*poignet.posGoalActu/100;
    else if(mot.id == moteurs[MAJEUR].id)
        delta = -(0.39*mot.posGoalActu + 36)*poignet.posGoalActu/100;
    else if(mot.id == poignet.id)
        delta = 0;

    delta *= poignet.posGoalActu/100;
    
    // angle = ax + b - delta
    float angleMot = pente * mot.posGoalActu + (float)mot.posOuvert - delta;
    dxl.setGoalPosition(mot.id, angleMot, UNIT_DEGREE);

    // Si on change la position du poignet, on ajuste la position des doigts aussi
    if(mot.id == poignet.id)
    {
        changerPosMoteurs(moteurs[POUCE]);
        changerPosMoteurs(moteurs[INDEX]);
        changerPosMoteurs(moteurs[MAJEUR]);
    }
}

// Lit le courant consommé et retourne la puissance consommé par tous les moteurs (P = VI)
String getPuissanceMoteurs()
{
    float courant = 0;
    for(int i = 0; i < NB_MOTEURS; i++)
        courant += abs(dxl.getPresentCurrent(moteurs[i].id)); //courant en mA
    float puissance = courant*V_IN_MOTEURS;
    return String(int(puissance)); //en mW

    //return String(moteurs[POIGNET_ROT].posGoalActu);
}

// Allume ou éteint les moteurs selon l'état du bouton d'arrêt d'urgence
void arretUrgence(bool arretActif)
{
    if(arretActif)
    {
        for(int i=0; i < NB_MOTEURS; i++)
            dxl.torqueOff(moteurs[i].id);
    }
    else
    {
        for(int i=0; i < NB_MOTEURS; i++)
            dxl.torqueOn(moteurs[i].id);
    }

}

// Configure les moteurs pour le déplacement par position et les place à leur position initiale
void setupMoteurs(Moteur mot)
{
    // Turn off torque when configuring items in EEPROM area
    dxl.torqueOff(mot.id);
    dxl.setOperatingMode(mot.id, OP_POSITION);
    dxl.torqueOn(mot.id);

    // Lancer le moteurs à leur vitesse donnée et les mettre à 0
    dxl.writeControlTableItem(PROFILE_VELOCITY, mot.id, mot.vitActu);
    changerPosMoteurs(mot);
}

