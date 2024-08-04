#include "AlpacaDiscovery.h"
#include "Logging.h"
#include "MotorUnit.h"
#include "Network.h"
#include "NunChuk.h"
#include "UDPSender.h"
#include "alpacaWebServer.h"
#include <Arduino.h>
// #include <LittleFS.h>
#include <Preferences.h>

int maxSpeed = 100000;
int deadZone = 10;
int wimax = 101;

NunChuk nunChuk;

Preferences prefs;
MotorUnit motorUnit(prefs);
Network network(prefs, WE_ARE_FOCUSER);

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Booting");
  prefs.begin("AutoFocuser", false);

  nunChuk.setUpNunChuk();
  nunChuk.nunChukLoop();
  // cheat code: when booted with z button down, start access point
  if (!nunChuk.isZPushed())
    network.setupWifi();
  else
    network.setUpAccessPoint(); // otherwise start access point

  delay(500);
  // LittleFS.begin();

  motorUnit.setupMotor();

  setupWebServer(motorUnit);
  setDiscoveryOnOff(true);
}

void loop() {
  nunChuk.nunChukLoop();
  // horible hack: Sky Safari sometimes gets confused by multiple alpaca
  // instances When z is held down, hold off replying to discovery.
  setDiscoveryOnOff(!nunChuk.isZPushed());

  int eqRaSpeed = nunChuk.getEQRaSpeed();
  if (eqRaSpeed != 0) {
    sendMoveAxisPercentCommand(0,eqRaSpeed);
  } else if (nunChuk.isEQRaStopRequired()) {
    sendMoveAxisPercentCommand(0,0);
  }

  int eqDecSpeed = nunChuk.getEQDecSpeed();
  if (eqDecSpeed != 0) {
    sendMoveAxisPercentCommand(1, eqDecSpeed);
  } else if (nunChuk.isEQDecStopRequired()) {
    sendMoveAxisPercentCommand(1, 0);
  }

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
