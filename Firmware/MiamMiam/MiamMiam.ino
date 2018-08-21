/*
MiamMiam.ino - Part of MiamMiam Firmware
Created by Sébastien Mischler & Raymond Humbert
Released into the public domain
http://www.onlfait.ch/MiamMiam
First commit: 19/08/2018

--------------------------------------------------------------------------------
PINS
--------------------------------------------------------------------------------
RTC (I2C):
SDA => A4
SCL => A5
*/
#include "Scheduler.h"

// Initialisation des variables
Scheduler scheduler;

// Initialise la communication serial
void setupSerial () {
  while (!Serial);
  Serial.begin(115200);
  delay(1000);
}

// Initialise le planificateur
void setupScheduler () {
  if (!scheduler.begin()) {
    Serial.println("Le module RTC ne fonctionne pas correctement...");
    Serial.println("Verifier le câblage!");
    while (1);
  }

  scheduler.printAlarms();
  scheduler.setAlarmCallback(feed);
}

// Initialisation du programme
void setup () {
  setupSerial();
  setupScheduler();
}

// Boucle principal du programme
void loop () {
  // Affiche la date, l"heure et le jour
  scheduler.printCurrentTime();
  scheduler.watch();
  delay(1000);
}

// Nourriture...
void feed (float quantity) {

}
