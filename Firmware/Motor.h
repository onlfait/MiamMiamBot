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

#ifndef MOTOR_EN_PIN_INVERT
  #define MOTOR_EN_PIN_INVERT 1
#endif

#ifndef MOTOR_STEP_PIN_INVERT
  #define MOTOR_STEP_PIN_INVERT 0
#endif

#ifndef MOTOR_DIR_PIN_INVERT
  #define MOTOR_DIR_PIN_INVERT 0
#endif

#ifndef MOTOR_DIR_INVERT
  #define MOTOR_DIR_INVERT 0
#endif

#ifndef MOTOR_STEPS
  #define MOTOR_STEPS 200
#endif

#ifndef MOTOR_MAX_SPEED
  #define MOTOR_MAX_SPEED 500
#endif

#ifndef MOTOR_ACCELERATION
  #define MOTOR_ACCELERATION 500
#endif

#ifndef MOTOR_MICROSTEPPING
  #define MOTOR_MICROSTEPPING 1
#endif

struct Motor_t {
  unsigned int steps;
  unsigned int maxSpeed;
  unsigned int acceleration;
  uint8_t microstepping;
  uint8_t invertDirPin;
  uint8_t invertStepPin;
  uint8_t invertEnPin;
  uint8_t invertDir;
};

class Motor {
  public:
    Motor();
    void setMotor(Motor_t motor);
    void enable();
    void disable();
    void move(long steps);
    void feed(uint8_t quantity);
    void watch();
  private:
    Motor_t _motor;
    void _update();
};

#endif
