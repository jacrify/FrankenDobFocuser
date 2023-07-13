#ifndef CHUCK_CONTROLLER
#define CHUCK_CONTROLLER

#include "NunchukShared.h"

class ChuckController {
private:
  int mode;
  int speed;
  // int dir; // 1 clockwise , -1 anticlockwise, 0 stopped.
  int minSpeeds[4];
  int maxSpeeds[4];

public:
  void processChuckData(wii_i2c_nunchuk_state state);
  int getSpeed();
  // int getDir();
  int getMode();
  void setMode(int mode);
  void setModeParameters(int mode, int minSpeedInHz, int maxSpeedInHz);
};
#endif