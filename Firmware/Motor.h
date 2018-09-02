/*
* Motor.h - Part of MiamMiamBot Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"
#include <AccelStepper.h>

#ifndef MOTOR_EN_PIN
  #define MOTOR_EN_PIN 4
#endif

#ifndef MOTOR_STEP_PIN
  #define MOTOR_STEP_PIN 5
#endif

#ifndef MOTOR_DIR_PIN
  #define MOTOR_DIR_PIN 6
#endif

#ifndef MOTOR_EN_INV
  #define MOTOR_EN_INV 1
#endif

#ifndef MOTOR_STEP_INV
  #define MOTOR_STEP_INV 0
#endif

#ifndef MOTOR_DIR_INV
  #define MOTOR_DIR_INV 0
#endif

#ifndef MOTOR_STEPS
  #define MOTOR_STEPS 200
#endif

#ifndef MOTOR_MICROSTEPPING
  #define MOTOR_MICROSTEPPING 1
#endif

struct Motor_t {
  unsigned int steps;
  uint8_t microstepping;
  uint8_t invertDirPin;
  uint8_t invertStepPin;
  uint8_t invertEnPin;
};

class Motor {
  public:
    Motor();
    void begin();
    void setMotor(Motor_t motor);
    void enable();
    void disable();
    void move(long steps);
    void watch();
  private:
    Motor_t _motor;
    void _updatePinsInverted();
};

#endif
