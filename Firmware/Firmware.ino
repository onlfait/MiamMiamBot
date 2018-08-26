/*
* MiamMiam.ino - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "CommandLine.h"
#include "Scheduler.h"
#include "EEPROMStore.h"

// Initialisation des variables
EEPROMStore store;
CommandLine commandLine;
Scheduler scheduler;

// -----------------------------------------------------------------------------
// Initialisation du programme
// -----------------------------------------------------------------------------
void setup () {
  // Démarrage de la ligne de commande
  commandLine.begin();

  // Démarrage du plannificateur
  if (!scheduler.begin()) {
    commandLine.send(F("Le module RTC ne fonctionne pas correctement..."));
    commandLine.send(F("Verifiez le câblage!"));
    while (1);
  }

  // Ajout des fonctions de rappel pour chaque commande
  commandLine.undefinedCommand(undefinedCommand);
  commandLine.addCommand("fingerprint", fingerprint);
  commandLine.addCommand("setDateTime", setDateTime);
  commandLine.addCommand("getDateTime", getDateTime);
  commandLine.addCommand("setAlarm", setAlarm);
  commandLine.addCommand("getAlarm", getAlarm);
  commandLine.addCommand("setMotor", setMotor);
  commandLine.addCommand("getMotor", getMotor);
  commandLine.addCommand("save", save);

  // Démarrage du magasin (config)
  store.begin();

  // Enregistre toutes les alarmes
  scheduler.setAlarm(0, store.data.alarm0);
  scheduler.setAlarm(1, store.data.alarm1);
  scheduler.setAlarm(2, store.data.alarm2);
  scheduler.setAlarm(3, store.data.alarm3);
  scheduler.setAlarm(4, store.data.alarm4);
  scheduler.setAlarm(5, store.data.alarm5);
  scheduler.setAlarm(6, store.data.alarm6);
  scheduler.setAlarm(7, store.data.alarm7);
  scheduler.setAlarm(8, store.data.alarm8);
  scheduler.setAlarm(9, store.data.alarm9);

  // Enregistre la fonction de rappel pour les alarmes
  scheduler.setAlarmCallback(alarmCallback);

  // Envoie l'emprinte digital
  fingerprint();
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

void undefinedCommand () {
  commandLine.send(
    F("error|Commande non reconnue: %s.\n"),
    commandLine.getLine()
  );
}

void fingerprint () {
  commandLine.send(F("MiamMiamBot|%s\n"), store.data.hash);
}

// Mise à jour de la date et l'heure
// ex.: setDateTime|2018|8|20|17|40|55
void setDateTime () {
  int argsCount = commandLine.getArgsCount();
  if (argsCount != 7) {
    commandLine.send(
      F("error|Nombre d'argument attendus 7, reçu %u.\n"),
      argsCount
    );
    return;
  }
  scheduler.setDateTime(DateTime(
    commandLine.getArgAsInt(1),
    commandLine.getArgAsInt(2),
    commandLine.getArgAsInt(3),
    commandLine.getArgAsInt(4),
    commandLine.getArgAsInt(5),
    commandLine.getArgAsInt(6)
  ));
  commandLine.send(F("ok\n"));
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

// ex.: setAlarm|0|17|40|4
void setAlarm () {
  int argsCount = commandLine.getArgsCount();
  if (argsCount != 5) {
    commandLine.send(
      F("error|Nombre d'argument attendus 5, reçu %u.\n"),
      argsCount
    );
    return;
  }
  int index = commandLine.getArgAsInt(1);
  AlarmStruct alarm = {
    commandLine.getArgAsInt(2),
    commandLine.getArgAsInt(3),
    commandLine.getArgAsFloat(4)
  };
  scheduler.setAlarm(index, alarm);
  store.setAlarm(index, alarm);
  commandLine.send(F("ok\n"));
}

// ex.: getAlarm|0
void getAlarm () {
  int argsCount = commandLine.getArgsCount();
  if (argsCount != 2) {
    commandLine.send(
      F("error|Nombre d'argument attendus 2, reçu %u.\n"),
      argsCount
    );
    return;
  }
  int index = commandLine.getArgAsInt(1);
  printAlarm(index, scheduler.getAlarm(index));
}

void setMotor () {
  int argsCount = commandLine.getArgsCount();
  if (argsCount != 4) {
    commandLine.send(
      F("error|Nombre d'argument attendus 4, reçu %u.\n"),
      argsCount
    );
    return;
  }
  // TODO set motor...
  MotorStruct motor = {
    commandLine.getArgAsInt(1),
    commandLine.getArgAsInt(2),
    !!commandLine.getArgAsInt(3)
  };
  store.setMotor(motor);
  commandLine.send(F("ok\n"));
}

void getMotor () {
  MotorStruct motor = store.data.motor;
  commandLine.send(
    F("motor|%d|%d|%d\n"),
    motor.steps,
    motor.microstepping,
    motor.inverse
  );
}

void save () {
  store.save();
  commandLine.send(F("ok\n"));
}

// -----------------------------------------------------------------------------
// Alarme
// -----------------------------------------------------------------------------

void printAlarm (int index, AlarmStruct alarm) {
  commandLine.send(
    F("alarm|%u|%u|%u|%s\n"),
    index,
    alarm.hour,
    alarm.minute,
    fts(alarm.quantity)
  );
}

// Appeler a chaque fois qu'une alarme est soulevée
void alarmCallback (int index, AlarmStruct alarm) {
  printAlarm(index, alarm);
}
