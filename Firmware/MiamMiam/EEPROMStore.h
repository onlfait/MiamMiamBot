/*
* EEPROMStore.h - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef EEPROM_STORE_H
#define EEPROM_STORE_H

#include "Arduino.h"
#include <EEPROM.h>
#include "Scheduler.h"

#define EEPROM_STORE_HASH "#yol2"
#define EEPROM_STORE_HASH_SIZE 6

typedef struct {
  char hash[EEPROM_STORE_HASH_SIZE];
  int stepsByRotation;
  int microstepping;
  AlarmStruct alarm0;
  AlarmStruct alarm1;
  AlarmStruct alarm2;
  AlarmStruct alarm3;
  AlarmStruct alarm4;
  AlarmStruct alarm5;
  AlarmStruct alarm6;
  AlarmStruct alarm7;
  AlarmStruct alarm8;
  AlarmStruct alarm9;
} EEPROMStoreStruct;

class EEPROMStore {
  public:
    EEPROMStore();
    void begin();
    bool load();
    void save();
    EEPROMStoreStruct data;
  private:
    bool _validate();
};

#endif
