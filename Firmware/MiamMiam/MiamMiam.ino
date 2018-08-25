/*
* MiamMiam.ino - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "CommandLine.h"

// Initialisation des variables
CommandLine commandLine;

// -----------------------------------------------------------------------------
// Initialisation du programme
// -----------------------------------------------------------------------------
void setup () {
  // Démarrage de la ligne de commande
  commandLine.begin();
  // Ajout des fonctions de rappel pour chaque commande
  commandLine.addCommand("setTime", setTime);
}

// -----------------------------------------------------------------------------
// Boucle principal du programme
// -----------------------------------------------------------------------------
void loop () {
  // Si une commande est reçue
  if (commandLine.read()) {
    // Execute la fonction de rappel si la commande est reconnue
    commandLine.executeCommand();
  }
}

// -----------------------------------------------------------------------------
// Les commands...
// -----------------------------------------------------------------------------

// Mise à jour de l'horloge
void setTime () {
  Serial.println("setTime!!!");
  Serial.println(commandLine.getArgAsInt(1));
}
