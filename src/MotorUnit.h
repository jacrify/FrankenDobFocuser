#ifndef MOTORUNIT_H
#define MOTORUNIT_H

#define ENDPOSITION 307029
class MotorUnit {
public:
  void setupMotor();
  void motorLoop();
  void moveTo(int location);
  void move(int speed);
  void moveSafely(int speed);
  int getPosition();
  void stop();
  void resetLimit();
  int isLimitFindingModeOn();
};
#endif