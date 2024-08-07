#include "ChuckController.h"
#include <Math.h>
#include <stdexcept>

#define DEADZONE 20
#define MAX_AXIS 100

#define FIND_LIMIT_MODE_NUMBER 4

int getNewMode(CurrentChukState state) {

  if (state.isRight())
    return 1;
  if (state.isDown())
    return 2;
  if (state.isLeft())
    return 3;
  if (state.isUp())
    return 0;
  return -1;
}

int interpolate(int minIn, int maxIn, int minOut, int maxOut, int value) {
  // Check for division by zero
  if (minIn == maxIn) {
    throw std::runtime_error(
        "Invalid input: minIn and maxIn cannot be the same.");
  }

  // Clamp the value if it exceeds maxIn
  if (value > maxIn) {
    value = maxIn;
  }

  // Clamp the value if it is less than minIn
  if (value < minIn) {
    value = minIn;
  }

  // Perform interpolation
  return minOut + ((value - minIn) * (maxOut - minOut)) / (maxIn - minIn);
}

bool ChuckController::isLimitFindingModeOn() { return isLimitFindingMode; }

/*
Take the state of the nunchuk and work out what we should do, based on internal
state. Primarily sets the desired speed and LED flash patterns. General UI:
- If no buttons are pushed, pushes up or down set a proportional speed depending
on the mode. Single led flashes fast
- If z button pushed, moving in a direction chooses a mode (0 up, 1 right, 2
down, 3 left) Single led flashes slow
- If z and c buttons pushed and held, then limit finding mode. Moving up and
down moves motor. On release, the position of the  motor becomes the new end
limit (0 position) All leds flash fast

*/
void ChuckController::processChuckData(wii_i2c_nunchuk_state state) {
  currentState.processState(state);
  eqDecStop = false;
  eqDecSpeed = 0;
  eqRaStop = false;
  eqRaSpeed = 0;
  // Both buttons held for 3 seconds. We're in find limit mode. Let them
  // move up and down. When they stop, that's the limit.
  if (currentState.isBothHeld()) {
    isLimitFindingMode = 1;
    ledFlashCycle1 = 1 + 2 + 4 + 8;
    flashFast = 1;

    if (currentState.isUp()) {
      speed = interpolate(DEADZONE, MAX_AXIS, minSpeeds[FIND_LIMIT_MODE_NUMBER],
                          maxSpeeds[FIND_LIMIT_MODE_NUMBER], state.y);
      return;
    }
    if (currentState.isDown()) {
      speed =
          -interpolate(DEADZONE, MAX_AXIS, minSpeeds[FIND_LIMIT_MODE_NUMBER],
                       maxSpeeds[FIND_LIMIT_MODE_NUMBER], -state.y);
      return;
    }
    speed = 0;
    return;
  }

  // User let go of buttons, after holding them down
  // Set flag for limit reset
  if (currentState.isBothReleasedAfterHeld()) {
    limitFlag = 1;
    setMode(mode);                   // to set leds
    ledFlashCycle2 = ledFlashCycle1; // don't flash
    isLimitFindingMode = 0;
    speed = 0; //?
    return;
  }

  // check for mode change mode
  if (currentState.isZPushed()) {
    zPushed = true;
    // if (state.z == 1 and state.c == 0) {
    int newMode = getNewMode(currentState);
    if (newMode != -1) {
      mode = newMode;

      ledFlashCycle1 = pow(2, mode);
      ledFlashCycle2 = 0;
      flashFast = 0;
    }
    return;
  } else {
    zPushed = false;
  }
  // check for movements to send to eq
  if (currentState.isCPushed()) {

    if (currentState.isRight()) { // east
      eqRaSpeed = interpolate(DEADZONE, MAX_AXIS, 20, 100, state.x);
      lastEqRaSpeed = eqRaSpeed;
    }
    if (currentState.isLeft()) { // west
      eqRaSpeed = -interpolate(DEADZONE, MAX_AXIS, 20, 100, -state.x);
      lastEqRaSpeed = eqRaSpeed;
    }

    if (currentState.isUp()) { // north? 
      eqDecSpeed = interpolate(DEADZONE, MAX_AXIS, 20, 100, state.y);
      lastEqDecSpeed = eqDecSpeed;
      ;
    }
    if (currentState.isDown()) { // south
      eqDecSpeed = -interpolate(DEADZONE, MAX_AXIS, 20, 100, -state.y);
      lastEqDecSpeed = eqDecSpeed;
    }

    // only stop if last time was a move.
    if (eqRaSpeed == 0 && lastEqRaSpeed != 0) {
      eqRaStop = true;
      lastEqRaSpeed = 0;
    }
    if (eqDecSpeed == 0 && lastEqDecSpeed != 0) {
      eqDecStop = true;
      lastEqDecSpeed = 0;
    }
  }

  if (currentState.isCReleased()) {
    eqRaStop = true;
    eqDecStop = true;
    lastEqRaSpeed = 0;
    lastEqDecSpeed = 0;
    eqDecSpeed = 0;
    eqRaSpeed=0;
    return;
  }

  // Neither button held, basic interpolation mode
  if (currentState.isBothNotPushed()) {

    if (currentState.isUp()) {
      speed = interpolate(DEADZONE, MAX_AXIS, minSpeeds[mode], maxSpeeds[mode],
                          state.y);

      ledFlashCycle2 = 0; // flash when moving
      flashFast = 1;

      return;
    }
    if (currentState.isDown()) {
      speed = -interpolate(DEADZONE, MAX_AXIS, minSpeeds[mode], maxSpeeds[mode],
                           -state.y);
      ledFlashCycle2 = 0; // flash when moving
      flashFast = 1;
      return;
    }
    ledFlashCycle2 = ledFlashCycle1; // don't flash when stopped

    speed = 0;
    // dir=0;
    return;
  }
  speed = 0;
}

int ChuckController::getSpeed() { return speed; }

int ChuckController::getEQRaSpeed() { return eqRaSpeed; }
int ChuckController::getEQDecSpeed() { return eqDecSpeed; }

// int ChuckController::getDir() { return dir; }

int ChuckController::getMode() { return mode; }

void ChuckController::setMode(int m) {
  mode = m;
  ledFlashCycle1 = pow(2, mode);
}

bool ChuckController::getAndFlipLimitFlag() {
  if (limitFlag == 1) {
    limitFlag = 0;
    return 1;
  } else
    return 0;
}

bool ChuckController::isZPushed() { return zPushed; }

void ChuckController::setModeParameters(int mode, int minSpeedInHz,
                                        int maxSpeedInHz) {
  if (mode > MODE_COUNT) {
    throw std::runtime_error("Mode number passed is out of range");
  }
  minSpeeds[mode] = minSpeedInHz;
  maxSpeeds[mode] = maxSpeedInHz;
}

// Only send stop to eq once
bool ChuckController::getAndFlipEQRaStopFlag() {
  if (eqRaStop) {
    eqRaStop = false;

    return true;
  }
  return false;
}

bool ChuckController::getAndFlipEQDecStopFlag() {
  if (eqDecStop) {
    eqDecStop = false;

    return true;
  }
  return false;
}
int ChuckController::getLedsFlashCycle1() { return ledFlashCycle1; }
int ChuckController::getLedsFlashCycle2() { return ledFlashCycle2; }
bool ChuckController::getFlashFast() { return flashFast; }
