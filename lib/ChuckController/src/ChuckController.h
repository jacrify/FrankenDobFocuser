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
  int ledFlashCycle1 = 0;
  int ledFlashCycle2 = 0;
  bool flashFast;

public:
  void processChuckData(wii_i2c_nunchuk_state state);
  int getSpeed();
  // int getDir();
  int getMode();
  void setMode(int mode);
  void setModeParameters(int mode, int minSpeedInHz, int maxSpeedInHz);
  int getAndFlipLimitFlag();
  bool isLimitFindingModeOn(); // when this mode is on we ignore limits when moving motor
  int getLedsFlashCycle1(); //bitwise leds that should be lit on cycle1. 1=1, 2=2, 4=3, 8=4.
  int getLedsFlashCycle2(); // bitwise leds that should be flashed. 1=1, 2=2, 4=3, 8=4.
  bool getFlashFast(); //if true flash fast, otherwise flash slow
};
#endif