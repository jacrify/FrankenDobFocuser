#include "Logging.h"
#include <WiFiManager.h>
WiFiManager wifiManager;
#include <Preferences.h>
// #include <WiFi.h>

void loopNetwork(Preferences &prefs) {
  if (digitalRead(0) == LOW) {
    prefs.putBool("homeWifi", true);

    delay(300);
    esp_restart();
  }
}

void setupWifiHome(Preferences &prefs) {
  log("Connecting to home wifi");
  wifiManager.setConnectTimeout(10);
  wifiManager.autoConnect();
}
void setupWifi() {

  log("Creating access point");
  WiFi.softAP("dontlookup", "dontlookdown");
}
