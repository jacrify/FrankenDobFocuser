#ifndef MOTORUNIT_H
#define MOTORUNIT_H
class MotorUnit {
public:
  void setupMotor();
  void motorLoop();
  void moveTo(int location);
  void moveClockwise(int speed);
  void moveAntiClockwise(int speed);
  int getPosition();
  void stop();
};
#endif