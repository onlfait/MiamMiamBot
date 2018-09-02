/*
* Scheduler.h - Part of MiamMiamBot Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Arduino.h"
#include "RTClib.h"

#define SCHEDULER_MAX_ALARMS 5

#ifndef SCHEDULER_RTC
  #define SCHEDULER_RTC 1
#endif

#if SCHEDULER_RTC == 2
  extern RTC_DS3231 RTC;
#elif SCHEDULER_RTC == 3
  extern RTC_PCF8523 RTC;
#else
  extern RTC_DS1307 RTC;
#endif

//extern RTC_DS1307 RTC;

struct SchedulerAlarm_t {
  uint8_t hour;
  uint8_t minute;
  uint8_t quantity;
};

struct SchedulerLastAlarm_t {
  uint8_t day;
  SchedulerAlarm_t alarm;
};

typedef void (*SchedulerAlarmCallback_f) (uint8_t index, SchedulerAlarm_t alarm);

class Scheduler {
  public:
    Scheduler();
    bool begin();
    DateTime getDateTime();
    void setDateTime(DateTime dateTime);
    SchedulerAlarm_t getAlarm(uint8_t index);
    void setAlarm(uint8_t index, SchedulerAlarm_t alarm);
    void setAlarmCallback(SchedulerAlarmCallback_f alarmCallback);
    void watch();
  private:
    SchedulerAlarm_t _alarms[SCHEDULER_MAX_ALARMS];
    SchedulerAlarmCallback_f _alarmCallback;
};

#endif
