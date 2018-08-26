/*
* Scheduler.cpp - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "Scheduler.h"

static bool isSameAlarm (LastAlarmStruct a, LastAlarmStruct b) {
  return (
    a.day == b.day
    && a.alarm.hour == b.alarm.hour
    && a.alarm.minute == b.alarm.minute
  );
}

Scheduler::Scheduler () {}

void Scheduler::setAlarm (int index, AlarmStruct alarm) {
  _alarms[index] = alarm;
}

void Scheduler::setAlarmCallback (AlarmCallback alarmCallback) {
  _alarmCallback = alarmCallback;
}

DateTime Scheduler::getDateTime () {
  return RTC.now();
}

void Scheduler::setDateTime (DateTime dateTime) {
  RTC.adjust(dateTime);
}

bool Scheduler::begin () {
  return RTC.begin() && RTC.isrunning();
}

void Scheduler::watch () {
  DateTime now = RTC.now();

  for (int i = 0; i < ALARMS_COUNT; i++) {
    AlarmStruct alarm = _alarms[i];
    LastAlarmStruct currentAlarm = { now.day(), alarm };

    if (!alarm.quantity || isSameAlarm(currentAlarm, _lastAlarm)) {
      continue;
    }

    if (alarm.hour == now.hour() && alarm.minute == now.minute()) {
      if (_alarmCallback != NULL) {
        _alarmCallback(alarm);
      }
      _lastAlarm = currentAlarm;
      return;
    }
  }
}
