#include "Logging.h"
#include "MotorUnit.h"
#include "NunChuk.h"
#include <Arduino.h>
#include "Network.h"
#include "alpacaWebServer.h"



#include <LittleFS.h>


int maxSpeed = 100000;
int deadZone = 10;
int wimax = 101;
MotorUnit motorUnit;
NunChuk nunChuk;


void setup() {
  Serial.begin(115200);
  
  Serial.println("Booting");

  setupWifi();
  delay(500);
  LittleFS.begin();

  motorUnit.setupMotor();
  nunChuk.setUpNunChuk();
  setupWebServer(motorUnit);

}

void loop() {
  

  nunChuk.nunChukLoop();
  if (nunChuk.resetLimitRequested()) {
    motorUnit.resetLimit(); // set position to zero.
  }

  
  int speed = nunChuk.getSpeed();
  
  if (speed == 0)
    motorUnit.stop();
  else {

    if (nunChuk.isLimitFinding()) {
      motorUnit.move(speed);
      // log("In limit finding mode");
    } else {
      motorUnit.moveSafely(speed);
      // log("In normal move mode");
    }
  }
  delay(20);
}
