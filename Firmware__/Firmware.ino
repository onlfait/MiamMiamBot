/*
* MiamMiam.ino - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "CommandLine.h"

// Configuration
#define BAUD_RATE 115200

// Initialisation des variables
CommandLine commandLine;

// -----------------------------------------------------------------------------
// Initialisation du programme
// -----------------------------------------------------------------------------
void setup () {
  // Démarrage de la ligne de commande
  commandLine.begin(BAUD_RATE);

  // Ajout des fonctions de rappel pour chaque commande
  commandLine.addCommand(F("getBanner"), getBanner); // getBanner
  commandLine.addCommand(F("setTime"), setTime);     // setTime|yyyy|mm|dd|hh|mm|ss
  commandLine.addCommand(F("getTime"), getTime);     // getTime
  commandLine.addCommand(F("setAlarm"), setAlarm);   // setAlarm|index|hh|mm|quantity
  commandLine.addCommand(F("getAlarm"), getAlarm);   // getAlarm
  commandLine.addCommand(F("setMotor"), setMotor);   // setMotor|steps|microstepping|reverse
  commandLine.addCommand(F("getMotor"), getMotor);   // getMotor
  commandLine.addCommand(F("save"), save);

  commandLine.send(F("Pouet"));
}

// -----------------------------------------------------------------------------
// Boucle principal du programme
// -----------------------------------------------------------------------------
void loop () {
  // Ecoute la ligne de commande
  commandLine.watch();
}

// -----------------------------------------------------------------------------
// Commandes
// -----------------------------------------------------------------------------
void getBanner () {}
void setTime () {}
void getTime () {}
void setAlarm () {}
void getAlarm () {}
void setMotor () {}
void getMotor () {}
void save () {}
