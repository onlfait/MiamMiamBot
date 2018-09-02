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
    MOTOR_MAX_SPEED,
    MOTOR_ACCELERATION,
    MOTOR_MICROSTEPPING,
    MOTOR_DIR_INV,
    MOTOR_STEP_INV,
    MOTOR_EN_INV
  };
  stepper.setEnablePin(MOTOR_EN_PIN);
  _update();
  disable();
}

void Motor::_update () {
  stepper.setMaxSpeed((float) _motor.maxSpeed);
  stepper.setAcceleration((float) _motor.acceleration);
  stepper.setPinsInverted(
    (bool) _motor.invertDirPin,
    (bool) _motor.invertStepPin,
    (bool) _motor.invertEnPin
  );
}

void Motor::setMotor (Motor_t motor) {
  _motor = motor;
  _update();
}

void Motor::enable () {
  stepper.enableOutputs();
}

void Motor::disable () {
  stepper.disableOutputs();
}

void Motor::move (long steps) {
  stepper.move(steps);
  enable();
}

void Motor::watch () {
  if (stepper.distanceToGo() == 0) {
    disable();
  }
  stepper.run();
}
