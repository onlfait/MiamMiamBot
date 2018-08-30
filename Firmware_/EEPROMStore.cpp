/*
* EEPROMStore.cpp - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "EEPROMStore.h"

EEPROMStoreStruct EEPROMStoreData = {
  EEPROM_STORE_HASH, // Store hash
  { 200, 1, false }, // Motor { steps, microstepping, inverse }
  { 0, 0, 0 },       // Alarmes { hour, minute, quantity }
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 },
  { 0, 0, 0 }
};

EEPROMStore::EEPROMStore () {
  data = EEPROMStoreData;
}

void EEPROMStore::begin () {
  if (!load()) {
    save();
  }
}

bool EEPROMStore::_validate () {
  for (int i = 0; i < EEPROM_STORE_HASH_SIZE; i++) {
    if (EEPROM.read(i) != EEPROM_STORE_HASH[i]) {
      return false;
    }
  }
  return true;
}

bool EEPROMStore::load () {
  if (_validate()) {
    for (int i = 0; i < sizeof(data); i++) {
      *((char*)&data + i) = EEPROM.read(i);
    }
    return true;
  }
  return false;
}

void EEPROMStore::save () {
  for (int i = 0; i < sizeof(data); i++) {
    EEPROM.write(i, *((char*)&data + i));
  }
}

void EEPROMStore::setMotor (MotorStruct motor) {
  data.motor = motor;
}

void EEPROMStore::setAlarm (int index, AlarmStruct alarm) {
  switch (index) {
    case 0: data.alarm0 = alarm; break;
    case 1: data.alarm1 = alarm; break;
    case 2: data.alarm2 = alarm; break;
    case 3: data.alarm3 = alarm; break;
    case 4: data.alarm4 = alarm; break;
    case 5: data.alarm5 = alarm; break;
    case 6: data.alarm6 = alarm; break;
    case 7: data.alarm7 = alarm; break;
    case 8: data.alarm8 = alarm; break;
    case 9: data.alarm9 = alarm; break;
  }
}
