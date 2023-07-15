#ifndef MOTORUNIT_H
#define MOTORUNIT_H

#define ENDPOSITION 266494
class MotorUnit {

public:
  void setupMotor();
  void motorLoop();
  void move(int speed);
  void moveSafely(int speed);

  int getPosition();
  
  void stop();
  void resetLimit();
  int isLimitFindingModeOn();
};
#endif