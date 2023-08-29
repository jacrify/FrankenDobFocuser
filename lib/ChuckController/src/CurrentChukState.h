#ifndef CURRENT_CHUK
#define CURRENT_CHUK

#include "NunchukShared.h"

#define DEADZONE 20
#define HOLDTIME_MILLIS 2000

class CurrentChukState {
  // wrapper method for the current state of the chuck
public:
  
  

  void processState(wii_i2c_nunchuk_state s);
  bool isUp();
  bool isDown();
  bool isLeft();
  bool isRight();
  bool isCenter();
  bool isCPushed();
  bool isZPushed();
  bool isBothPushed();

  bool isCNewlyPushed();
  bool isZNewlyPushed();
  bool isBothNewlyPushed();
  bool isCReleased();
  bool isZReleased();
  bool isBothReleased();
  bool isCHeld();
  bool isZHeld();
  bool isBothHeld();
  bool isCReleasedAfterHeld();
  bool isZReleasedAfterHeld();
  bool isBothReleasedAfterHeld();

private:
  wii_i2c_nunchuk_state currentState;
  wii_i2c_nunchuk_state lastState;

  unsigned long cPushedTime;
  unsigned long zPushedTime;
  unsigned long bothPushedTime;

  bool isUp(wii_i2c_nunchuk_state s);
  bool isDown(wii_i2c_nunchuk_state s);
  bool isLeft(wii_i2c_nunchuk_state s);
  bool isRight(wii_i2c_nunchuk_state s);
  bool isCenter(wii_i2c_nunchuk_state s);
  bool isCPushed(wii_i2c_nunchuk_state s);
  bool isZPushed(wii_i2c_nunchuk_state s);
  bool isBothPushed(wii_i2c_nunchuk_state s);
};
#endif