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
  commandLine.addCommand(F("undefinedCommand"), undefinedCommand);
  commandLine.addCommand(F("getFingerprint"), getFingerprint);
  commandLine.addCommand(F("setDateTime"), setDateTime);
  commandLine.addCommand(F("getDateTime"), getDateTime);
  commandLine.addCommand(F("setAlarm"), setAlarm);
  commandLine.addCommand(F("getAlarm"), getAlarm);
  commandLine.addCommand(F("setMotor"), setMotor);
  commandLine.addCommand(F("getMotor"), getMotor);
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
void undefinedCommand () {}
void getFingerprint () {}
void setDateTime () {}
void getDateTime () {}
void setAlarm () {}
void getAlarm () {}
void setMotor () {}
void getMotor () {}
void save () {}
