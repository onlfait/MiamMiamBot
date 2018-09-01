/*
* Scheduler.cpp - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "Scheduler.h"

Scheduler::Scheduler () {}

SchedulerAlarmStruct Scheduler::getAlarm (uint8_t index) {
  return _alarms[index];
}

void Scheduler::setAlarm (uint8_t index, SchedulerAlarmStruct alarm) {
  _alarms[index] = alarm;
}
