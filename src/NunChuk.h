#ifndef NUNCHUK
#define NUNCHUK

#include "ChuckController.h"
#define PIN_SDA 23
#define PIN_SCL 22

ChuckController controller; // all logic delated here
// ESP32 I2C port (0 or 1):
#define WII_I2C_PORT 0
class NunChuk {
public:
  void setUpNunChuk();
  void nunChukLoop();
  int getSpeed();
};
#endif