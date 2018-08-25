/*
* MiamMiam.ino - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "CommandLine.h"

// Initialisation des variables
CommandLine commandLine;

// Initialisation du programme
void setup () {
  commandLine.begin();
}

// Boucle principal du programme
void loop () {
  if (commandLine.read()) {
    Serial.println(commandLine.getLine());
    if (commandLine.argMatch(0, "print")) {
      if (commandLine.argMatch(1, "alarms")) {
        //scheduler.printAlarms();
        Serial.println("Alarms...");
      }
    }
  }
}
