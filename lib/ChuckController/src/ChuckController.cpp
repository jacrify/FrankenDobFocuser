#include "ChuckController.h"
#include <Math.h>
#include <stdexcept>


#define DEADZONE 20
#define MAX_AXIS 100
#define MODE_COUNT 4




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

void ChuckController::processChuckData(wii_i2c_nunchuk_state state) {
  // check for mode change
  if (state.z == 1 and state.c == 0) {
    int newMode = getNewMode(state);
    if (newMode != -1) {
      mode = newMode;
    }
    return;
  }
  if (state.z == 0 and state.c == 0) {
    // basic speed interpolation mode
    if (isUp(state)) {
      speed = interpolate(DEADZONE, MAX_AXIS, minSpeeds[mode], maxSpeeds[mode],
                          state.y);
                          return;
    }
    if (isDown(state)) {
                          speed =
                              interpolate(DEADZONE, MAX_AXIS, minSpeeds[mode],
                                          maxSpeeds[mode], -state.y);
                          return;
    }

    speed=0;
    return;
  }

  return;
}

int ChuckController::getSpeed() { return speed; }

int ChuckController::getMode() { return mode; }

void ChuckController::setMode(int m) { mode = m; }

void ChuckController::setModeParameters(int mode, int minSpeedInHz,
                                        int maxSpeedInHz) {
  if (mode > MODE_COUNT) {
    throw std::runtime_error("Mode number passed is out of range");
  }
  minSpeeds[mode] = minSpeedInHz;
  maxSpeeds[mode] = maxSpeedInHz;
}
