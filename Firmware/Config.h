/*
* MiamMiam.ino - Part of MiamMiamBot Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BAUD_RATE 115200  // 9600, ..., 115200, ...

#define SCHEDULER_RTC 1  // RTC module [1 = DS1307, 2 = DS3231, 3 = PCF8523]

#define MOTOR_EN_PIN   4 // Enable Pin
#define MOTOR_STEP_PIN 5 // Step pin
#define MOTOR_DIR_PIN  6 // Direction pin

#define MOTOR_STEPS         200 // Step par tour
#define MOTOR_MAX_SPEED     500 // Vitesse max. en steps par secondes
#define MOTOR_ACCELERATION  500 // Accélération en secondes par secondes
#define MOTOR_MICROSTEPPING   1 // Microstepping [1, 2, 4, 8, 16, ...]
#define MOTOR_DIR_INVERT      0 // Inverse sense de rotation [0 = false, 1 = true]

#define MOTOR_EN_PIN_INVERT   1 // Inverse enable Pin [0 = false, 1 = true]
#define MOTOR_STEP_PIN_INVERT 0 // Inverse step Pin [0 = false, 1 = true]
#define MOTOR_DIR_PIN_INVERT  0 // Inverse direction Pin [0 = false, 1 = true]

#endif
