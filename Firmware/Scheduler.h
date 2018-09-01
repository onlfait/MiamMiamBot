/*
* Scheduler.h - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Arduino.h"

#define SCHEDULER_MAX_ALARMS 5

typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t quantity;
} SchedulerAlarmStruct;

class Scheduler {
  public:
    Scheduler();
    SchedulerAlarmStruct getAlarm(uint8_t index);
    void setAlarm(uint8_t index, SchedulerAlarmStruct alarm);
  private:
    SchedulerAlarmStruct _alarms[SCHEDULER_MAX_ALARMS];
};

#endif
