/*
* MiamMiam.ino - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "CommandLine.h"
#include "Scheduler.h"

// Initialisation des variables
CommandLine commandLine;
Scheduler scheduler;

// -----------------------------------------------------------------------------
// Initialisation du programme
// -----------------------------------------------------------------------------
void setup () {
  // Démarrage de la ligne de commande
  commandLine.begin();
  // Ajout des fonctions de rappel pour chaque commande
  commandLine.addCommand("setDateTime", setDateTime);
  commandLine.addCommand("getDateTime", getDateTime);
  // Enregistre la fonction de rappel pour les alarmes
  scheduler.setAlarmCallback(alarmCallback);
  // Démarrage du plannificateur
  if (!scheduler.begin()) {
    commandLine.send(F("Le module RTC ne fonctionne pas correctement..."));
    commandLine.send(F("Verifiez le câblage!"));
    while (1);
  }
  // DEBUG -------------------------
  AlarmStruct alarm1 = { 17, 40, 1 };
  AlarmStruct alarm2 = { 17, 40, 2 }; // skipped -> same as alarm 1
  AlarmStruct alarm3 = { 17, 41, 555.75 };
  AlarmStruct alarm4 = { 17, 42, 0 }; // skipped -> quantity == 0
  AlarmStruct alarm5 = { 17, 43, 5 };
  scheduler.setAlarm(0, alarm1);
  scheduler.setAlarm(1, alarm2);
  scheduler.setAlarm(2, alarm3);
  scheduler.setAlarm(3, alarm4);
  scheduler.setAlarm(4, alarm5);
  scheduler.setDateTime(DateTime(2018, 8, 20, 17, 40, 55));
}

// -----------------------------------------------------------------------------
// Boucle principal du programme
// -----------------------------------------------------------------------------
void loop () {
  scheduler.watch();
  commandLine.watch();
}

// -----------------------------------------------------------------------------
// Les commands...
// -----------------------------------------------------------------------------

// Mise à jour de la date et l'heure
// ex.: setDateTime|2018|8|20|17|40|55
void setDateTime () {
  int argsCount = commandLine.getArgsCount();
  if (argsCount == 7) {
    scheduler.setDateTime(DateTime(
      commandLine.getArgAsInt(1),
      commandLine.getArgAsInt(2),
      commandLine.getArgAsInt(3),
      commandLine.getArgAsInt(4),
      commandLine.getArgAsInt(5),
      commandLine.getArgAsInt(6)
    ));
    getDateTime();
  } else {
    commandLine.send(
      F("error|Nombre d'argument attendus 7, reçu %u.\n"),
      argsCount
    );
  }
}

// Renvoie la date et l'heure courante
void getDateTime () {
  DateTime now = scheduler.getDateTime();
  commandLine.send(
    F("dateTime|%u|%u|%u|%u|%u|%u\n"),
    now.day(),
    now.month(),
    now.year(),
    now.hour(),
    now.minute(),
    now.second()
  );
}

// -----------------------------------------------------------------------------
// Alarme
// -----------------------------------------------------------------------------

// Appeler a chaque fois qu'une alarme est soulevée
void alarmCallback (int index, AlarmStruct alarm) {
  commandLine.send(
    F("alarm|%u|%u|%u|%s\n"),
    index,
    alarm.hour,
    alarm.minute,
    fts(alarm.quantity)
  );
}
