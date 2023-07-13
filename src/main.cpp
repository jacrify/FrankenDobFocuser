#include "Logging.h"
#include "MotorUnit.h"
#include "NunChuk.h"
#include <Arduino.h>




int maxSpeed = 100000;
int deadZone = 10;
int wimax = 101;
MotorUnit motorUnit;
NunChuk nunChuk;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  
  motorUnit.setupMotor();
  nunChuk.setUpNunChuk();
}

void loop() {
  nunChuk.nunChukLoop();
  int speed=nunChuk.getSpeed();
  if (speed=0)
    motorUnit.stop();
  else if (speed >0) 
    motorUnit.moveClockwise(speed);
  else if (speed<0)
    motorUnit.moveAntiClockwise(-speed);
   
  delay(100);
}
