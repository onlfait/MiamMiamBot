/*
* Motor.cpp - Part of MiamMiamBot Firmware
* Created by Sébastien Mischler & Raymond Humbert
* Released into the public domain
* http://www.onlfait.ch/MiamMiam
*/
#include "Motor.h"

AccelStepper stepper (AccelStepper::DRIVER, MOTOR_STEP_PIN, MOTOR_DIR_PIN);

Motor::Motor () {
  _motor = {
    MOTOR_STEPS,
    MOTOR_MICROSTEPPING,
    MOTOR_DIR_INV,
    MOTOR_STEP_INV,
    MOTOR_EN_INV
  };
}

void Motor::_updatePinsInverted () {
  stepper.setPinsInverted(
    (bool) _motor.invertDirPin,
    (bool) _motor.invertStepPin,
    (bool) _motor.invertEnPin
  );
}

void Motor::begin () {
  stepper.setEnablePin(MOTOR_EN_PIN);
  stepper.setMaxSpeed(400);     // step/s
  stepper.setAcceleration(400); // step/s/s
  _updatePinsInverted();
  disable();
}

void Motor::setMotor (Motor_t motor) {
  _motor = motor;
  _updatePinsInverted();
}

void Motor::enable () {
  stepper.enableOutputs();
}

void Motor::disable () {
  stepper.disableOutputs();
}

void Motor::move (long steps) {
  Serial.println(steps);
  stepper.move(steps);
  enable();
}

void Motor::watch () {
  if (stepper.distanceToGo() == 0) {
    disable();
  }
  stepper.run();
}
