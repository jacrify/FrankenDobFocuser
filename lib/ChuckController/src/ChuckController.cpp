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
- If c button pushed, then direction chooses a memory location (release to
choose and move to position in that slot). Releasing in middle chooses no
memory. Further moves by user will update that memory location.
Leds slow flash inverted showing selected memory slot- ie if user is pointing to
slot 1, leds 0 2 and 3 flash. At middle all leds slow flash. Once released
led revert to normal behaviour?
- If c button pushed and held without a direction, then direction chooses a
memory location (release to choose and save current position to that slot).
Releasing in middle chooses no memory. Further moves by user will update that
memory location. Leds fast flash inverted showing selected memory slot- ie if
user is pointing to slot 1, leds 0 2 and 3 flash. At middle all leds fast flash.
*/
void ChuckController::processChuckData(wii_i2c_nunchuk_state state) {

  if ((state.z == 0 or state.c == 0) and twoButtonPressedFlag) {
    // user let go of buttons, after holding them down
    if (state.millis - twoButtonsPressedTime > TWO_BUTTON_TIME_MILLIS) {
      // held down for long enough. Flag for limit reset
      limitFlag = 1;
      setMode(mode); // to set leds
    }
    twoButtonsPressedTime = 0; // reset time
    twoButtonPressedFlag = false;
    speed = 0;
    isLimitFindingMode = 0;
  }
  if (state.c == 0 and memoryMoveInitiatatedFlag ==1) {
    //User has selected a memory location and releases button
    memoryMoveInitiatatedFlag=false;
    memoryMoveFlag=true;
  }
  if (state.z == 0 and state.c == 1) {
    // memory select mode
    if (isUp(state)) {
      memorySlot = 0;
      ledFlashCycle1 = 16 - pow(2, 1);
      ledFlashCycle2 = 0;
      flashFast=false;
      memoryMoveInitiatatedFlag = true;
      return;
    } else {
      if (isRight(state)) {
        memorySlot = 1;
        ledFlashCycle1 = 16 - pow(2, 1);
        ledFlashCycle2 = 0;
        memoryMoveInitiatatedFlag = true;
        flashFast = false;
        return;
      } else {
        if (isDown(state)) {
          memorySlot = 2;
          ledFlashCycle1 = 16 - pow(2, 2);
          ledFlashCycle2 = 0;
          memoryMoveInitiatatedFlag = true;
          flashFast = false;
          return;
        } else {
          if (isLeft(state)) {
            memorySlot = 3;
            ledFlashCycle1 = 16 - pow(2, 3);
            ledFlashCycle2 = 0;
            memoryMoveInitiatatedFlag = true;
            flashFast = false;
            return;
          } else {
            //non direction selected
            memorySlot = -1;
            ledFlashCycle1 = 15;
            ledFlashCycle2 = 0;
            
            flashFast = false;
            return;
          }
        }
      }
    };
    
  };

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
        ledFlashCycle1 = 1 + 2 + 4 + 8;
        flashFast = 1;

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

bool ChuckController::getAndFlipLimitFlag() {
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

// return whether a memory
// move is required
bool ChuckController::getAndFlipMemoryMoveFlag() {

  if (memoryMoveFlag) {
    memoryMoveFlag = false;
    return true;
  }
  return false;
}

int ChuckController::getMemoryPosition() {

} // returns the currently active memory  slot. -1 means no slot selected.