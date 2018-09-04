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
    MOTOR_DIR_PIN_INVERT,
    MOTOR_STEP_PIN_INVERT,
    MOTOR_EN_PIN_INVERT,
    MOTOR_DIR_INVERT
  };
  stepper.setEnablePin(MOTOR_EN_PIN);
  _update();
  disable();
}

void Motor::_update () {
  stepper.setMaxSpeed((float) (_motor.maxSpeed * _motor.microstepping));
  stepper.setAcceleration((float) (_motor.acceleration * _motor.microstepping));
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
  if (_motor.invertDir) {
    steps = -steps;
  }
  stepper.move(steps * _motor.microstepping);
  enable();
}

void Motor::feed(uint8_t quantity) {
  move((long) (quantity * _motor.steps));
}

void Motor::watch () {
  static unsigned int steps = 0;
  const unsigned int ratio = _motor.steps * _motor.microstepping;

  if (stepper.distanceToGo() == 0) {
    steps = 0;
    disable();
  }

  if (stepper.run()) {
    if ((steps % ratio) == 0) {
      Serial.println("go back!");
    } else {
      steps++;
    }
  }
}
