#include "MotorUnit.h"
#include "FastAccelStepper.h"
#include "Logging.h"

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

#define dirPinStepper 16  // BLUE
#define stepPinStepper 17 // ORANGE

#define PREF_SAVED_POS_KEY "SavedPosition"
// #define enablePinStepper 16
#define backwardSwitchPin 22

MotorUnit::MotorUnit(Preferences &p) : preferences(p) {
  alpacaMoveInProgress = false;
  alpacaTargetPosition = -1;
}
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

    lastSavedPos = preferences.getUInt(PREF_SAVED_POS_KEY, 0);
    log("Loaded saved position %d", lastSavedPos);
    stepper->setCurrentPosition(lastSavedPos);

  } else {
    log("AHHHHHHHHH! Can't init stepper");
  }
}

int32_t MotorUnit::getPosition() { return stepper->getCurrentPosition(); }

bool MotorUnit::checkAlpacaMoveInProgress() {

  // check if move in progress. If so stop, save pos.
  // need to call this before allowing other moves
  if (alpacaMoveInProgress) {
    uint32_t pos = stepper->getCurrentPosition();
    if (pos == alpacaTargetPosition) {
      alpacaMoveInProgress = false;
      // stepper->stopMove();
      savePosition(pos);
      //  preferences.putUInt(PREF_SAVED_POS_KEY, pos);

      return false;
    } else {
      return true;
    }
  }
  return false;
}

void MotorUnit::savePosition(uint32_t pos) {
  log("Stopped, saving position %d", pos);
  lastSavedPos = pos;
  preferences.putUInt(PREF_SAVED_POS_KEY, pos);
}

void MotorUnit::move(int speed) {
  if (checkAlpacaMoveInProgress())
    return;

  if (speed > 0) {
    stepper->setSpeedInHz(speed);
    stepper->runBackward();
  } else {
    stepper->setSpeedInHz(abs(speed));
    stepper->runForward();
  }
}

void MotorUnit::moveSafely(int speed) {
  // ignore moves when alpaca move in progress
  if (checkAlpacaMoveInProgress())
    return;
  if (speed > 0) {
    stepper->setSpeedInHz(speed);
    stepper->moveTo(0);
  } else {
    stepper->setSpeedInHz(abs(speed));
    stepper->moveTo(ENDPOSITION);
  }
}

void MotorUnit::moveTo(int32_t pos, int speedInHz) {
  // only called from alpaca.
  alpacaTargetPosition = pos;
  alpacaMoveInProgress = true;
  stepper->setSpeedInHz(speedInHz);
  stepper->moveTo(pos);
}
void MotorUnit::resetLimit() {
  stepper->setCurrentPosition(0);
  savePosition(0);
  // preferences.putUInt(PREF_SAVED_POS_KEY, 0);
  log("At limit, saving zero position");
}

void MotorUnit::stop() {
  if (checkAlpacaMoveInProgress())
    return;
  // TODO little bug here. Stepper could already be stopped, so position
  // never gets saved.

  if (stepper->isRunning()) {
    stepper->stopMove();
  }
  uint32_t pos = stepper->getCurrentPosition();
  if (pos != lastSavedPos) {
    savePosition(pos);
    // preferences.putUInt(PREF_SAVED_POS_KEY, pos);
    log("Stopped, saving position %d", pos);
  }
}

boolean MotorUnit::isMoving() { return stepper->isRunning(); }

int32_t MotorUnit::getLimitPosition() { return ENDPOSITION; }

int32_t MotorUnit::getMaxIncrement() { return MAXINCREMENT; }