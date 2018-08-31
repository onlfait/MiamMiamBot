/*
* MiamMiam.ino - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/

// Librairies
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

  // Ajoute la commande par defaut
  commandLine.defaultCommand(defaultCommand);

  // Ajoute des commandes utilisateur
  commandLine.addCommand(F("getBanner"), getBannerCommand); // getBanner
  commandLine.addCommand(F("setTime"), setTimeCommand);     // setTime|yyyy|mm|dd|hh|mm|ss
  commandLine.addCommand(F("getTime"), getTimeCommand);     // getTime
  commandLine.addCommand(F("setAlarm"), setAlarmCommand);   // setAlarm|index|hh|mm|quantity
  commandLine.addCommand(F("getAlarm"), getAlarmCommand);   // getAlarm
  commandLine.addCommand(F("setMotor"), setMotorCommand);   // setMotor|steps|microstepping|reverse
  commandLine.addCommand(F("getMotor"), getMotorCommand);   // getMotor
  commandLine.addCommand(F("save"), saveSettingsCommand);   // save

  // Envoie la bannière du firmware
  commandLine.send(F("MiamMiamBot#yolo\n"));
}

// -----------------------------------------------------------------------------
// Boucle principal du programme
// -----------------------------------------------------------------------------
void loop () {
  // Ecoute la ligne de commande
  commandLine.watch();
}

// -----------------------------------------------------------------------------
// Commandes (helpers)
// -----------------------------------------------------------------------------
void ok () {
  commandLine.send(F("ok\n"));
}

void badArgumentCount (uint8_t provided, uint8_t expected) {
  commandLine.send(F("nombre d'argument invalid: %u/%u\n"), provided, expected);
}

bool valid (uint8_t provided, uint8_t expected) {
  if (provided != expected) {
    badArgumentCount(provided, expected);
    return false;
  }
  return true;
}

// -----------------------------------------------------------------------------
// Commandes
// -----------------------------------------------------------------------------
void getBannerCommand (uint8_t argc, char **argv) {
  if (!valid(argc, 1)) return;
  ok();
}

void setTimeCommand (uint8_t argc, char **argv) {
  if (!valid(argc, 7)) return;
  ok();
}

void getTimeCommand (uint8_t argc, char **argv) {
  if (!valid(argc, 1)) return;
  ok();
}

void setAlarmCommand (uint8_t argc, char **argv) {
  if (!valid(argc, 5)) return;
  ok();
}

void getAlarmCommand (uint8_t argc, char **argv) {
  if (!valid(argc, 2)) return;
  ok();
}

void setMotorCommand (uint8_t argc, char **argv) {
  if (!valid(argc, 4)) return;
  ok();
}

void getMotorCommand (uint8_t argc, char **argv) {
  if (!valid(argc, 1)) return;
  ok();
}

void saveSettingsCommand (uint8_t argc, char **argv) {
  if (!valid(argc, 1)) return;
  ok();
}

void defaultCommand(uint8_t argc, char **argv) {
  commandLine.send(F("Commande non reconnu: %s\n"), argv[0]);
}
