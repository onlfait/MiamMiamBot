/*
SetTime.ino - Part of MiamMiam Firmware
Created by Sébastien Mischler & Raymond Humbert
Released into the public domain
http://www.onlfait.ch/MiamMiam
First commit: 19/08/2018
*/
#include <Wire.h>
#include "RTClib.h"
#include <EEPROM.h>

// RTC (I2C):
// SDA => A4
// SCL => A5
RTC_DS1307 RTC;
//RTC_DS3231 RTC;
//RTC_PCF8523 RTC;

typedef struct {
  bool enabled;
  int hour;
  int minute;
  float quantity;
} Alarm;

int alarmSize = sizeof(Alarm);

const int ALARMS_COUNT = 10;

void setup () {
  while (!Serial);
  Serial.begin(115200);
  delay(1000);

  if (!RTC.begin()) {
    Serial.println("Le module RTC est introuvable...");
    Serial.println("Verifiez le câblage!");
    while (1);
  }

  Serial.print("Mise à jour de l'heure: ");
  Serial.println(F(__TIME__));

  Serial.print("Mise à jour de la date: ");
  Serial.println(F(__DATE__));

  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (!RTC.isrunning()) {
    Serial.println("Le module RTC ne fonctionne pas correctement...");
    Serial.println("Verifier le câblage!");
    while (1);
  }

  Serial.print("Mise à zero des alarms ");

  int count = ALARMS_COUNT;
  int percent = round(count / 10);

  for (int i = 0; i < count; i++) {
    Alarm alarm = { true, 17, 41 + i, 4 };
    EEPROM.put(i * alarmSize, alarm);
    Serial.print(".");
    delay(100);
  }

  Serial.println("");
  Serial.println("Terminé!");
}

void loop () {
  DateTime now = RTC.now();

  printTwoDigits(now.day());
  Serial.print("/");
  printTwoDigits(now.month());
  Serial.print("/");
  Serial.print(now.year(), DEC);

  Serial.print(" ");
  printTwoDigits(now.hour());
  Serial.print(":");
  printTwoDigits(now.minute());
  Serial.print(":");
  printTwoDigits(now.second());

  Serial.println();

  delay(1000);
}

void printTwoDigits (int8_t input) {
  if (input < 10) {
    Serial.print("0");
  }
  Serial.print(input, DEC);
}
