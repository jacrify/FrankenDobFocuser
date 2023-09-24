#include "Logging.h"

#include <Preferences.h>
#include <WiFi.h>

#define HOMEWIFISSID "HOMEWIFISSID"
#define HOMEWIFIPASS "HOMEWIFIPASS"

void logIP() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    log("Waiting for connection...");
  }

  // Once connected, log the IP address
  IPAddress ip = WiFi.localIP();
  log("Connected! IP address: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

void setupWifiHome(Preferences &prefs) {
  prefs.putString(HOMEWIFISSID,"Radar");
  prefs.putString(HOMEWIFIPASS, "dwn@t75dwn@t75");
  if (prefs.isKey(HOMEWIFISSID) && prefs.isKey(HOMEWIFIPASS)) {
    
    log("Connecting to home wifi");
    WiFi.begin(prefs.getString(HOMEWIFISSID), prefs.getString(HOMEWIFIPASS));
    logIP();

  } else {
    log("Prefs not found :()");
  }
  }
void setupWifi() {

  log("Creating access point");
  WiFi.softAP("dontlookup", "dontlookdown");
}
