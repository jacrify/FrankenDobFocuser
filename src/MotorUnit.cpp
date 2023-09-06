#include "MotorUnit.h"
#include "FastAccelStepper.h"
#include "Logging.h"

#include <Preferences.h>

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

Preferences preferences;

#define dirPinStepper 16  // BLUE
#define stepPinStepper 17 // ORANGE

#define PREF_SAVED_POS_KEY "SavedPosition"
// #define enablePinStepper 16
#define backwardSwitchPin 22

void MotorUnit::setupMotor() {
  engine.init(1);
  stepper = engine.stepperConnectToPin(stepPinStepper);
  // stepper = engine.stepperConnectToPin(stepPinStepper, DRIVER_RMT);
  // stepper = engine.stepperConnectToPin(stepPinStepper, DRIVER_MCPWM_PCNT);
  if (stepper) {
    log("Stepper ok");
        stepper->setDirectionPin(dirPinStepper);

    // stepper->setEnablePin(enablePinStepper);
    stepper->setAutoEnable(true);

    // stepper->setSpeedInHz(5000);
    stepper->setAcceleration(1000000); // 100 steps/s²

    stepper->setSpeedInHz(60000);
    preferences.begin("AutoFocuser", false);
    uint32_t savedPosition = preferences.getUInt(PREF_SAVED_POS_KEY, 0);
    log("Loaded saved position %d", savedPosition);
    stepper->setCurrentPosition(savedPosition);

  } else {
    log("AHHHHHHHHH! Can't init stepper");
  }
}

int MotorUnit::getPosition() { return stepper->getCurrentPosition(); }

void MotorUnit::move(int speed) {

  if (speed > 0) {
    stepper->setSpeedInHz(speed);
    stepper->runBackward();
  } else {
    stepper->setSpeedInHz(abs(speed));
    stepper->runForward();
  }
}

void MotorUnit::moveSafely(int speed) {
  if (speed > 0) {
    stepper->setSpeedInHz(speed);
    stepper->moveTo(0);
  } else {
    stepper->setSpeedInHz(abs(speed));
    stepper->moveTo(ENDPOSITION);
  }
}

void MotorUnit::resetLimit() {
  stepper->setCurrentPosition(0);
  preferences.putUInt(PREF_SAVED_POS_KEY, 0);
  log("At limit, saving zero position");
}

void MotorUnit::stop() {
  if (stepper->isRunning()) {
    stepper->stopMove();
    uint32_t pos = stepper->getCurrentPosition();
    preferences.putUInt(PREF_SAVED_POS_KEY, pos);
    log("Stopped, saving position %d", pos);
  }
}
