/*
Scheduler.h - Part of MiamMiam Firmware
Created by Sébastien Mischler & Raymond Humbert
Released into the public domain
http://www.onlfait.ch/MiamMiam
First commit: 19/08/2018
*/
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Arduino.h"
#include <Wire.h>
#include "RTClib.h"
#include <EEPROM.h>

// RTC (I2C):
//   SDA => A4
//   SCL => A5
extern RTC_DS1307 RTC;
//extern RTC_DS3231 RTC;
//extern RTC_PCF8523 RTC;

typedef void (*Callback) (float quantity);

class Scheduler {
  public:
    Scheduler();
    bool begin();
    void watch();
    void printAlarms();
    void printAlarm(int i);
    void printCurrentTime();
    void setAlarmCallback(Callback callback);
  private:
    Callback _callback;
};

#endif
