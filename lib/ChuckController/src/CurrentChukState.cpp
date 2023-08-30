#include "CurrentChukState.h"
#include <Math.h>

void CurrentChukState::processState(wii_i2c_nunchuk_state state) {
  lastState = currentState;
  currentState = state;
  if (isCNewlyPushed())
    cPushedTime = state.millis;
  if (isZNewlyPushed())
    zPushedTime = state.millis;
  if (isBothNewlyPushed())
    bothPushedTime = state.millis;
  if (isCReleased())
    cPushedTime = 0;
  if (isZReleased())
    zPushedTime = 0;
  if (!isBothPushed())
    bothPushedTime = 0;
}

bool CurrentChukState::isRight() { return isRight(currentState); }

bool CurrentChukState::isRight(wii_i2c_nunchuk_state state) {
  if (state.x >= DEADZONE)
    if (state.x > abs(state.y))
      return true;

  return false;
}
bool CurrentChukState::isLeft() { return isLeft(currentState); }

bool CurrentChukState::isLeft(wii_i2c_nunchuk_state state) {
  if (state.x <= -DEADZONE)
    if (abs(state.x) > abs(state.y))
      return true;

  return false;
}

bool CurrentChukState::isDown() { return isDown(currentState); }
bool CurrentChukState::isDown(wii_i2c_nunchuk_state state) {
  if (state.y <= -DEADZONE)
    if (abs(state.y) > abs(state.x))
      return true;

  return false;
}
bool CurrentChukState::isUp() { return isUp(currentState); }

bool CurrentChukState::isUp(wii_i2c_nunchuk_state state) {
  if (state.y >= DEADZONE)
    if (abs(state.y) > abs(state.x))
      return true;

  return false;
}
bool CurrentChukState::isCenter() { return isCenter(currentState); }
bool CurrentChukState::isCenter(wii_i2c_nunchuk_state state) {
  return !(isUp() or isDown() or isLeft() or isRight());
}

bool CurrentChukState::isCPushed() { return isCPushed(currentState); }
bool CurrentChukState::isCPushed(wii_i2c_nunchuk_state state) {
  return (state.z == 0 && state.c == 1);
}
bool CurrentChukState::isZPushed() { return isZPushed(currentState); }
bool CurrentChukState::isZPushed(wii_i2c_nunchuk_state state) {
  return (state.z == 1 and state.c == 0);
}
bool CurrentChukState::isBothPushed() { return isBothPushed(currentState); }

bool CurrentChukState::isBothNotPushed() {
  return isNeitherPushed(currentState);
}

bool CurrentChukState::isNeitherPushed(wii_i2c_nunchuk_state state) {
  return (state.z == 0 and state.c == 0);
}

bool CurrentChukState::isBothPushed(wii_i2c_nunchuk_state state) {
  return (state.z == 1 && state.c == 1);
}

bool CurrentChukState::isCNewlyPushed() {
  return (isCPushed(currentState) && !isCPushed(lastState));
}

bool CurrentChukState::isCReleased() {
  return ((!isCPushed(currentState) && isCPushed(lastState)));
}

bool CurrentChukState::isCHeld() {
  if (cPushedTime == 0)
    return false;
  return ((currentState.millis - cPushedTime) > HOLDTIME_MILLIS);
}

bool CurrentChukState::isZNewlyPushed() {
  return (isZPushed(currentState) && !isZPushed(lastState));
}

bool CurrentChukState::isZReleased() {
  return ((!isZPushed(currentState) && isZPushed(lastState)));
}

bool CurrentChukState::isZHeld() {
  if (zPushedTime == 0)
    return false;
  return ((currentState.millis - zPushedTime) > HOLDTIME_MILLIS);
}

bool CurrentChukState::isBothNewlyPushed() {
  return (isBothPushed(currentState) && !isBothPushed(lastState));
}

bool CurrentChukState::isBothReleased() {
  return ((!isBothPushed(currentState) && isBothPushed(lastState)));
}

bool CurrentChukState::isBothHeld() {
  if (bothPushedTime == 0)
    return false;
  return ((currentState.millis - bothPushedTime) > HOLDTIME_MILLIS);
}

bool CurrentChukState::isBothReleasedAfterHeld() {
  return ( !isBothPushed(currentState) && isBothPushed(lastState) );
}