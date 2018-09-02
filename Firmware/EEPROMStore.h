/*
* EEPROMStore.h - Part of MiamMiamBot Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef EEPROM_STORE_H
#define EEPROM_STORE_H

#include "Arduino.h"
#include <EEPROM.h>
#include "Motor.h"
#include "Scheduler.h"

#define EEPROM_STORE_HASH "#yolo"
#define EEPROM_STORE_HASH_SIZE 6

struct EEPROMStore_t {
  char hash[EEPROM_STORE_HASH_SIZE];
  Motor_t motor;
  SchedulerAlarm_t alarm0;
  SchedulerAlarm_t alarm1;
  SchedulerAlarm_t alarm2;
  SchedulerAlarm_t alarm3;
  SchedulerAlarm_t alarm4;
};

class EEPROMStore {
  public:
    EEPROMStore();
    void begin();
    bool load();
    void save();
    void setMotor(Motor_t motor);
    void setAlarm(uint8_t index, SchedulerAlarm_t alarm);
    EEPROMStore_t data;
  private:
    bool _validate();
};

#endif
