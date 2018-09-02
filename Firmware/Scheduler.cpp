/*
* Scheduler.cpp - Part of MiamMiamBot Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "Scheduler.h"

static bool isSameAlarm (SchedulerLastAlarm_t a, SchedulerLastAlarm_t b) {
  return (
    a.day == b.day
    && a.alarm.hour == b.alarm.hour
    && a.alarm.minute == b.alarm.minute
  );
}

Scheduler::Scheduler () {}

bool Scheduler::begin () {
  if (!RTC.begin()) {
    return false;
  }
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  return RTC.isrunning();
}

DateTime Scheduler::getDateTime () {
  return RTC.now();
}

void Scheduler::setDateTime (DateTime dateTime) {
  RTC.adjust(dateTime);
}

SchedulerAlarm_t Scheduler::getAlarm (uint8_t index) {
  return _alarms[index];
}

void Scheduler::setAlarm (uint8_t index, SchedulerAlarm_t alarm) {
  _alarms[index] = alarm;
}

void Scheduler::setAlarmCallback (SchedulerAlarmCallback_f alarmCallback) {
  _alarmCallback = alarmCallback;
}

void Scheduler::watch () {
  static SchedulerLastAlarm_t lastAlarm;
  DateTime now = RTC.now();

  for (uint8_t i = 0; i < SCHEDULER_MAX_ALARMS; i++) {
    SchedulerAlarm_t alarm = _alarms[i];
    SchedulerLastAlarm_t currentAlarm = { now.day(), alarm };

    if (!alarm.quantity || isSameAlarm(currentAlarm, lastAlarm)) {
      continue;
    }

    if (alarm.hour == now.hour() && alarm.minute == now.minute()) {
      if (_alarmCallback != NULL) {
        _alarmCallback(i, alarm);
      }
      lastAlarm = currentAlarm;
      return;
    }
  }
}
