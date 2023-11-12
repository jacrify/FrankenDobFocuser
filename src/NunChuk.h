#ifndef NUNCHUK
#define NUNCHUK

#include "ChuckController.h"
#define PIN_SDA 23
#define PIN_SCL 22


// ESP32 I2C port (0 or 1):
#define WII_I2C_PORT 0
class NunChuk {
public:
  void setUpNunChuk();
  void nunChukLoop();
  int getSpeed();
  int resetLimitRequested();
  int isLimitFinding();
  bool isZPushed();
  int getEQRaSpeed();
  int getEQDecSpeed();
  bool isEQRaStopRequired();
  bool isEQDecStopRequired();
};
#endif