#ifndef CHUCK_CONTROLLER
#define CHUCK_CONTROLLER

#include "NunchukShared.h"
#define MODE_COUNT 5 // 0 up 1 right 2 down 3 left 4 limitfinder

class ChuckController {
private:
  int mode;
  int speed;
  // int dir; // 1 clockwise , -1 anticlockwise, 0 stopped.
  int minSpeeds[MODE_COUNT];
  int maxSpeeds[MODE_COUNT];
  int limitFlag;

public:
  void processChuckData(wii_i2c_nunchuk_state state);
  int getSpeed();
  // int getDir();
  int getMode();
  void setMode(int mode);
  void setModeParameters(int mode, int minSpeedInHz, int maxSpeedInHz);
  int getAndFlipLimitFlag();
  bool isLimitFindingModeOn(); // when this mode is on we ignore limits when moving motor
};
#endif