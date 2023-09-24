#include "AlpacaDiscovery.h"
#include "Logging.h"
#include "MotorUnit.h"
#include "Network.h"
#include "NunChuk.h"
#include "alpacaWebServer.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <Preferences.h>

int maxSpeed = 100000;
int deadZone = 10;
int wimax = 101;

NunChuk nunChuk;

Preferences prefs;
MotorUnit motorUnit(prefs);

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Booting");
  prefs.begin("AutoFocuser", false);

  nunChuk.setUpNunChuk();
  nunChuk.nunChukLoop();
  // cheat code: when booted with z button down, connect to home wifi
  if (nunChuk.isZPushed())
    setupWifiHome(prefs);
  else
    setupWifi(); // otherwise start access point

  delay(500);
  LittleFS.begin();

  motorUnit.setupMotor();

  setupWebServer(motorUnit);
  setDiscoveryOnOff(true);
}

void loop() {
  nunChuk.nunChukLoop();
  // horible hack: Sky Safari sometimes gets confused by multiple alpaca
  // instances When z is held down, hold off replying to discovery.
  setDiscoveryOnOff(!nunChuk.isZPushed());

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
