/*
* EEPROMStore.cpp - Part of MiamMiamBot Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "EEPROMStore.h"

EEPROMStore::EEPROMStore () {
  data = (EEPROMStore_t) {
    // Store hash
    EEPROM_STORE_HASH,
    // Moteur
    {
      MOTOR_STEPS,
      MOTOR_MAX_SPEED,
      MOTOR_ACCELERATION,
      MOTOR_MICROSTEPPING,
      MOTOR_DIR_INV,
      MOTOR_STEP_INV,
      MOTOR_EN_INV
    },
    // Alarmes { hour, minute, quantity }
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 }
  };
}

void EEPROMStore::begin () {
  if (!load()) {
    save();
  }
}

bool EEPROMStore::_validate () {
  for (uint8_t i = 0; i < EEPROM_STORE_HASH_SIZE; i++) {
    if (EEPROM.read(i) != EEPROM_STORE_HASH[i]) {
      return false;
    }
  }
  return true;
}

bool EEPROMStore::load () {
  if (_validate()) {
    for (uint8_t i = 0; i < sizeof(data); i++) {
      *((char*)&data + i) = EEPROM.read(i);
    }
    return true;
  }
  return false;
}

void EEPROMStore::save () {
  for (uint8_t i = 0; i < sizeof(data); i++) {
    EEPROM.write(i, *((char*)&data + i));
  }
}

void EEPROMStore::setMotor (Motor_t motor) {
  data.motor = motor;
}

void EEPROMStore::setAlarm (uint8_t index, SchedulerAlarm_t alarm) {
  switch (index) {
    case 0: data.alarm0 = alarm; break;
    case 1: data.alarm1 = alarm; break;
    case 2: data.alarm2 = alarm; break;
    case 3: data.alarm3 = alarm; break;
    case 4: data.alarm4 = alarm; break;
  }
}
