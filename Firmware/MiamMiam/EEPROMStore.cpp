/*
* EEPROMStore.cpp - Part of MiamMiam Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "EEPROMStore.h"

EEPROMStoreStruct EEPROMStoreData = {
  EEPROM_STORE_HASH, // Store hash
  200,               // Nombre de pas par tour du moteur
  1,                 // microstepping (1, 2, 4, 8, 16)
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
