/*
* MiamMiam.ino - Part of MiamMiamBot Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "Config.h"
#include "CommandLine.h"
#include "Scheduler.h"
#include "EEPROMStore.h"
#include "Motor.h"

// Initialisation des variables
CommandLine commandLine;
Scheduler scheduler;
EEPROMStore store;
Motor motor;

// -----------------------------------------------------------------------------
// Initialisation du programme
// -----------------------------------------------------------------------------
void setup () {
  // Démarrage de la ligne de commande
  commandLine.begin(SERIAL_BAUD_RATE);

  // Démarrage du plannificateur (horloge)
  if (!scheduler.begin()) {
    commandLine.send(F("Le module RTC ne fonctionne pas correctement..."));
    while (1); // bloque le programme ici...
  }

  // Ajoute la commande par defaut
  commandLine.defaultCommand(defaultCommand);

  // Ajoute des commandes utilisateur
  commandLine.addCommand(F("getBanner"), getBannerCommand); // getBanner
  commandLine.addCommand(F("setTime"), setDateTimeCommand); // setTime|dd|mm|yyyy|hh|mm|ss
  commandLine.addCommand(F("getTime"), getDateTimeCommand); // getTime
  commandLine.addCommand(F("setAlarm"), setAlarmCommand);   // setAlarm|index|hh|mm|quantity
  commandLine.addCommand(F("getAlarm"), getAlarmCommand);   // getAlarm
  commandLine.addCommand(F("setMotor"), setMotorCommand);   // setMotor|steps|maxSpeed|acceleration|microstepping|invertDirPin|invertStepPin|invertEnPin|invertDir
  commandLine.addCommand(F("getMotor"), getMotorCommand);   // getMotor
  commandLine.addCommand(F("save"), saveSettingsCommand);   // save
  commandLine.addCommand(F("feed"), feedCommand);           // feed|quantity

  // Démarrage du magasin (config)
  store.begin();

  // Initialisation du moteur
  motor.setMotor(store.data.motor);

  // Enregistre toutes les alarmes
  scheduler.setAlarm(0, store.data.alarm0);
  scheduler.setAlarm(1, store.data.alarm1);
  scheduler.setAlarm(2, store.data.alarm2);
  scheduler.setAlarm(3, store.data.alarm3);
  scheduler.setAlarm(4, store.data.alarm4);

  // Enregistre la fonction de rappel pour les alarmes
  scheduler.setAlarmCallback(alarmCallback);

  // Envoie la bannière du firmware
  getBannerCommand(1, NULL);
}

// -----------------------------------------------------------------------------
// Boucle principal du programme
// -----------------------------------------------------------------------------
void loop () {
  // Ecoute la ligne de commande
  commandLine.watch();
  // Ecoute les alarmes
  scheduler.watch();
  // Ecoute le moteur
  motor.watch();
}

// -----------------------------------------------------------------------------
// Commandes (helpers)
// -----------------------------------------------------------------------------
void badArgumentCount (uint8_t provided, uint8_t expected) {
  commandLine.send(F("Nombre d'argument invalid: %u/%u\n"), provided, expected);
}

bool hasArgumentCount (uint8_t provided, uint8_t expected) {
  if (provided != expected) {
    badArgumentCount(provided, expected);
    return false;
  }
  return true;
}

void sendOk () {
  commandLine.send(F("ok\n"));
}

void sendAlarm (uint8_t index, SchedulerAlarm_t alarm) {
  commandLine.send(
    F("alarm|%u|%u|%u|%u\n"),
    index,
    alarm.hour,
    alarm.minute,
    alarm.quantity
  );
}

// -----------------------------------------------------------------------------
// Commandes
// -----------------------------------------------------------------------------
void getBannerCommand (uint8_t argc, char **argv) {
  if (!hasArgumentCount(argc, 1)) return;
  commandLine.send(F("MiamMiamBot|%s\n"), store.data.hash);
}

void setDateTimeCommand (uint8_t argc, char **argv) {
  if (!hasArgumentCount(argc, 7)) return;
  scheduler.setDateTime(DateTime(
    atoi(argv[3]), atoi(argv[2]), atoi(argv[1]),
    atoi(argv[4]), atoi(argv[5]), atoi(argv[6])
  ));
  sendOk();
}

void getDateTimeCommand (uint8_t argc, char **argv) {
  if (!hasArgumentCount(argc, 1)) return;
  DateTime now = scheduler.getDateTime();
  commandLine.send(
    F("dateTime|%u|%u|%u|%u|%u|%u\n"),
    now.day(), now.month(), now.year(),
    now.hour(), now.minute(), now.second()
  );
}

void setAlarmCommand (uint8_t argc, char **argv) {
  if (!hasArgumentCount(argc, 5)) return;
  SchedulerAlarm_t alarm = {
    (uint8_t) atoi(argv[2]),
    (uint8_t) atoi(argv[3]),
    (uint8_t) atoi(argv[4])
  };
  store.setAlarm((uint8_t) atoi(argv[1]), alarm);
  scheduler.setAlarm((uint8_t) atoi(argv[1]), alarm);
  sendOk();
}

void getAlarmCommand (uint8_t argc, char **argv) {
  if (!hasArgumentCount(argc, 2)) return;
  uint8_t index = (uint8_t) atoi(argv[1]);
  SchedulerAlarm_t alarm = scheduler.getAlarm(index);
  sendAlarm(index, alarm);
}

void setMotorCommand (uint8_t argc, char **argv) {
  if (!hasArgumentCount(argc, 9)) return;
  Motor_t newMotor = {
    (unsigned int) atoi(argv[1]),
    (unsigned int) atoi(argv[2]),
    (unsigned int) atoi(argv[3]),
    (uint8_t) atoi(argv[4]),
    (uint8_t) atoi(argv[5]),
    (uint8_t) atoi(argv[6]),
    (uint8_t) atoi(argv[7]),
    (uint8_t) atoi(argv[8])
  };
  store.setMotor(newMotor);
  motor.setMotor(newMotor);
  sendOk();
}

void getMotorCommand (uint8_t argc, char **argv) {
  if (!hasArgumentCount(argc, 1)) return;
  commandLine.send(
    F("motor|%u|%u|%u|%d|%d|%d|%d|%d\n"),
    store.data.motor.steps,
    store.data.motor.maxSpeed,
    store.data.motor.acceleration,
    store.data.motor.microstepping,
    store.data.motor.invertDirPin,
    store.data.motor.invertStepPin,
    store.data.motor.invertEnPin,
    store.data.motor.invertDir
  );
}

void saveSettingsCommand (uint8_t argc, char **argv) {
  if (!hasArgumentCount(argc, 1)) return;
  store.save();
  sendOk();
}

void feedCommand (uint8_t argc, char **argv) {
  if (!hasArgumentCount(argc, 2)) return;
  motor.feed(atol(argv[1]));
  sendOk();
}

void defaultCommand(uint8_t argc, char **argv) {
  commandLine.send(F("Commande non reconnue: %s\n"), argv[0]);
}

// -----------------------------------------------------------------------------
// Alarmes
// -----------------------------------------------------------------------------
void alarmCallback (uint8_t index, SchedulerAlarm_t alarm) {
  sendAlarm(index, alarm); // DEBUG
  motor.feed(alarm.quantity);
}
