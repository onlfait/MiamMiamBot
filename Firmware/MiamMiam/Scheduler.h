/*
* Scheduler.h - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Arduino.h"
#include <Wire.h>
#include "RTClib.h"
#include <EEPROM.h>

const int ALARMS_COUNT = 10;

// RTC PINS (I2C):
//   SDA => A4
//   SCL => A5
extern RTC_DS1307 RTC;
//extern RTC_DS3231 RTC;
//extern RTC_PCF8523 RTC;

typedef struct {
  int hour;
  int minute;
  float quantity;
} AlarmStruct;

typedef struct {
  int day;
  AlarmStruct alarm;
} LastAlarmStruct;

typedef void (*AlarmCallback) (int index, AlarmStruct alarm);

class Scheduler {
  public:
    Scheduler();
    bool begin();
    void watch();
    DateTime getDateTime();
    void setDateTime(DateTime dateTime);
    void setAlarm(int index, AlarmStruct alarm);
    void setAlarmCallback(AlarmCallback alarmCallback);
  private:
    AlarmCallback _alarmCallback;
    AlarmStruct _alarms[ALARMS_COUNT];
    LastAlarmStruct _lastAlarm;
};

#endif
