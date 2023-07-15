#include "ChuckController.h"
#include <Math.h>
#include <stdexcept>

#define DEADZONE 20
#define MAX_AXIS 100

#define FIND_LIMIT_MODE_NUMBER 4

#define TWO_BUTTON_TIME_MILLIS 3000

bool isRight(wii_i2c_nunchuk_state state) {
  if (state.x >= DEADZONE)
    if (state.x > abs(state.y))
      return true;

  return false;
}

bool isLeft(wii_i2c_nunchuk_state state) {
  if (state.x <= -DEADZONE)
    if (abs(state.x) > abs(state.y))
      return true;

  return false;
}

bool isDown(wii_i2c_nunchuk_state state) {
  if (state.y <= -DEADZONE)
    if (abs(state.y) > abs(state.x))
      return true;

  return false;
}

bool isUp(wii_i2c_nunchuk_state state) {
  if (state.y >= DEADZONE)
    if (abs(state.y) > abs(state.x))
      return true;

  return false;
}
int getNewMode(wii_i2c_nunchuk_state state) {

  if (isRight(state))
    return 1;
  if (isDown(state))
    return 2;
  if (isLeft(state))
    return 3;
  if (isUp(state))
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

unsigned long twoButtonsPressedTime = 0;
bool twoButtonPressedFlag = false;

int isLimitFindingMode = 0;
bool ChuckController::isLimitFindingModeOn() { return isLimitFindingMode; }

void ChuckController::processChuckData(wii_i2c_nunchuk_state state) {

  if ((state.z == 0 or state.c == 0) and twoButtonPressedFlag) {
    // user let go of buttons, after holding them down
    if (state.millis - twoButtonsPressedTime > TWO_BUTTON_TIME_MILLIS) {
      // held down for long enough. Flag for limit reset
      limitFlag = 1;
    }
    twoButtonsPressedTime = 0; // reset time
    twoButtonPressedFlag = false;
    speed = 0;
    isLimitFindingMode = 0;
  }
  // check for mode change mode
  if (state.z == 1 and state.c == 0) {
    int newMode = getNewMode(state);
    if (newMode != -1) {
      mode = newMode;

      ledFlashCycle1 = pow(2, mode);
      ledFlashCycle2 = 0;
      flashFast = 0;
    }
    return;
  }
  if (state.z == 0 and state.c == 0) {
    // basic speed interpolation mode
    if (isUp(state)) {
      speed = interpolate(DEADZONE, MAX_AXIS, minSpeeds[mode], maxSpeeds[mode],
                          state.y);

      ledFlashCycle2 = 0; // flash when moving
      flashFast = 1;

      return;
    }
    if (isDown(state)) {
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

  if (state.z == 1 and state.c == 1) {
    speed = 0;
    if (twoButtonPressedFlag) {
      if (state.millis - twoButtonsPressedTime > TWO_BUTTON_TIME_MILLIS) {
        // Both buttons held for 3 seconds. We're in find limit mode. Let them
        // move up and down. When they stop, that's the limit.
        isLimitFindingMode = 1;

        if (isUp(state)) {
          speed =
              interpolate(DEADZONE, MAX_AXIS, minSpeeds[FIND_LIMIT_MODE_NUMBER],
                          maxSpeeds[FIND_LIMIT_MODE_NUMBER], state.y);
          return;
        }
        if (isDown(state)) {
          speed = -interpolate(DEADZONE, MAX_AXIS,
                               minSpeeds[FIND_LIMIT_MODE_NUMBER],
                               maxSpeeds[FIND_LIMIT_MODE_NUMBER], -state.y);
          return;
        }

        speed = 0;
        // dir=0;
        return;
      } else
        return; // still waiting, nothing to see here
    } else {
      // use just pressed both. Store time and wait.
      twoButtonsPressedTime = state.millis;
      twoButtonPressedFlag = true;
      return;
    }
  }
  return;
}

int ChuckController::getSpeed() { return speed; }

// int ChuckController::getDir() { return dir; }

int ChuckController::getMode() { return mode; }

void ChuckController::setMode(int m) {
  mode = m;
  ledFlashCycle1 = pow(2, mode);
}

int ChuckController::getAndFlipLimitFlag() {
  if (limitFlag == 1) {
    limitFlag = 0;
    return 1;
  } else
    return 0;
}

void ChuckController::setModeParameters(int mode, int minSpeedInHz,
                                        int maxSpeedInHz) {
  if (mode > MODE_COUNT) {
    throw std::runtime_error("Mode number passed is out of range");
  }
  minSpeeds[mode] = minSpeedInHz;
  maxSpeeds[mode] = maxSpeedInHz;
}

int ChuckController::getLedsFlashCycle1() { return ledFlashCycle1; }
int ChuckController::getLedsFlashCycle2() { return ledFlashCycle2; }
bool ChuckController::getFlashFast() { return flashFast; }