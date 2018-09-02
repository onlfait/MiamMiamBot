/*
* MiamMiam.ino - Part of MiamMiamBot Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BAUD_RATE    115200  // 9600, ..., 115200, ...
#define SCHEDULER_RTC            1  // 1 = DS1307, 2 = DS3231, 3 = PCF8523
#define MOTOR_STEPS            200  // Step par tour
#define MOTOR_MICROSTEPPING      1  // Microstepping [1, 2, 4, 8, 16, ...]
#define MOTOR_EN_PIN             4  // Enable Pin
#define MOTOR_STEP_PIN           5  // Step pin
#define MOTOR_DIR_PIN            6  // Direction pin
#define MOTOR_STEP_INV           0  // Inverse step Pin
#define MOTOR_DIR_INV            0  // Inverse direction Pin
#define MOTOR_EN_INV             1  // Inverse enable Pin

#endif
