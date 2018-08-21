/*
Scheduler.cpp  - Part of MiamMiam Firmware
Created by Sébastien Mischler & Raymond Humbert
Released into the public domain
http://www.onlfait.ch/MiamMiam
First commit: 19/08/2018
*/
#include "Scheduler.h"

typedef struct {
  bool enabled;
  int hour;
  int minute;
  float quantity;
} Alarm;

int alarmSize = sizeof(Alarm);

const int ALARMS_COUNT = 10;

Alarm alarms[ALARMS_COUNT];
Alarm lastAlarm;
uint8_t lastAlarmDay;

static void printTwoDigits (int8_t input) {
  if (input < 10) {
    Serial.print("0");
  }
  Serial.print(input, DEC);
}

static bool isSameAlarm (Alarm a, Alarm b) {
  return (
    a.enabled == b.enabled
    && a.hour == b.hour
    && a.minute == b.minute
    && a.quantity == b.quantity
  );
}

static bool isLockedAlarm (DateTime now, Alarm a, Alarm b) {
  return (isSameAlarm(a, b) && now.day() == lastAlarmDay);
}

static void defaultCallback (float quantity) {
  Serial.println("Aucune fonction de rappel définie !");
}

Scheduler::Scheduler() {
  for (int i = 0; i < ALARMS_COUNT; i++) {
    Alarm alarm;
    EEPROM.get(i * alarmSize, alarm);
    alarms[i] = alarm;
  }
  _callback = defaultCallback;
}

void Scheduler::setAlarmCallback(Callback callback) {
  _callback = callback;
}

bool Scheduler::begin() {
  RTC.begin();
  RTC.adjust(DateTime(2018, 8, 20, 17, 40, 55));

  return RTC.begin() && RTC.isrunning();
}

void Scheduler::watch() {
  DateTime now = RTC.now();

  for (int i = 0; i < ALARMS_COUNT; i++) {
    Alarm alarm = alarms[i];

    if (!alarm.enabled || isLockedAlarm(now, alarm, lastAlarm)) {
      continue;
    }

    if (alarm.hour == now.hour() && alarm.minute == now.minute()) {
      Serial.print("> ");
      printAlarm(i);
      lastAlarm = alarm;
      lastAlarmDay = now.day();
      _callback(alarm.quantity);
    }
  }
}

void Scheduler::printAlarm (int i) {
  Alarm alarm = alarms[i];

  Serial.print("Alarme ");
  printTwoDigits(i + 1);
  Serial.print(" | ");

  printTwoDigits(alarm.hour);
  Serial.print(":");
  printTwoDigits(alarm.minute);
  Serial.print(" | ");

  Serial.print("quantité: ");
  printTwoDigits(alarm.quantity);
  Serial.print(" | ");

  if (alarm.enabled) {
    Serial.println("activée");
  } else {
    Serial.println("desactivée");
  }
}

void Scheduler::printAlarms () {
  for (int i = 0; i < ALARMS_COUNT; i++) {
    printAlarm(i);
  }
}

void Scheduler::printCurrentTime () {
  DateTime now = RTC.now();

  printTwoDigits(now.day());
  Serial.print("/");
  printTwoDigits(now.month());
  Serial.print("/");
  Serial.print(now.year(), DEC);

  Serial.print(" ");
  printTwoDigits(now.hour());
  Serial.print(":");
  printTwoDigits(now.minute());
  Serial.print(":");
  printTwoDigits(now.second());

  Serial.println();
}
