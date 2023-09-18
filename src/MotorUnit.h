#ifndef MOTORUNIT_H
#define MOTORUNIT_H

#include <Preferences.h>

#define ENDPOSITION 266494
#define MAXINCREMENT 266494

class MotorUnit {

public:
  MotorUnit(Preferences &prefs);
  void setupMotor();
  void motorLoop();
  void move(int speed);
  void moveSafely(int speed);
  void moveTo(int32_t pos, int speedInHz);
  bool isMoving();
  int32_t getLimitPosition();
  int32_t getMaxIncrement();
  int32_t getPosition();
  int32_t alpacaTargetPosition;
   void stop();
  void resetLimit();
  int isLimitFindingModeOn();
  
private:
  Preferences &preferences;
  bool alpacaMoveInProgress;
  bool checkAlpacaMoveInProgress();
  
};
#endif