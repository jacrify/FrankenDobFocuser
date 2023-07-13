#include "MotorUnit.h"
#include "FastAccelStepper.h"
#include "Logging.h"

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

#define dirPinStepper 16  // BLUE
#define stepPinStepper 17 // ORANGE

// #define enablePinStepper 16
#define backwardSwitchPin 22

void MotorUnit::setupMotor() {
  engine.init();
  stepper = engine.stepperConnectToPin(stepPinStepper);
  if (stepper) {
    stepper->setDirectionPin(dirPinStepper);

    // stepper->setEnablePin(enablePinStepper);
    stepper->setAutoEnable(true);

    // stepper->setSpeedInHz(5000);
    stepper->setAcceleration(1000000); // 100 steps/s²

    stepper->setSpeedInHz(60000);
  } else {
    log("AHHHHHHHHH! Can't init stepper");
  }
}

int MotorUnit::getPosition() { return stepper->getCurrentPosition(); }

void MotorUnit::move(int speed) {
  if (speed > 0) {
    stepper->setSpeedInHz(speed);
    stepper->runForward();
  } else {
    stepper->setSpeedInHz(speed);
    stepper->runBackward();
  }
}

void MotorUnit::moveSafely(int speed) {
  if (speed > 0) {
    stepper->setSpeedInHz(speed);
    stepper->moveTo(ENDPOSITION);
  } else {
    stepper->setSpeedInHz(speed);
    stepper->moveTo(0);
  }
}

void MotorUnit::moveTo(int location) {
  stepper->keepRunning();
  stepper->moveTo(location);
  //   stepper->move(100000);
}

void MotorUnit::resetLimit() {
  stepper->setCurrentPosition(0);
}

    void
    MotorUnit::stop() {
  stepper->stopMove();
}
