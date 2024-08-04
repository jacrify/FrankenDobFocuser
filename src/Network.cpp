#include "Logging.h"

#include <Preferences.h>
#include <WiFi.h>

#define HOMEWIFISSID "HOMEWIFISSID"
#define HOMEWIFIPASS "HOMEWIFIPASS"

IPAddress local_IP(172, 20, 10, 6);
IPAddress gateway(172, 20, 10, 1);
IPAddress subnet(255, 255, 255, 240);
IPAddress primaryDNS(172, 20, 10, 1);  

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
  // prefs.putString(HOMEWIFISSID,"");
  // prefs.putString(HOMEWIFIPASS, "");
  if (prefs.isKey(HOMEWIFISSID) && prefs.isKey(HOMEWIFIPASS)) {
    
    log("Connecting to home wifi");
    WiFi.begin(prefs.getString(HOMEWIFISSID).c_str(), prefs.getString(HOMEWIFIPASS).c_str());
    logIP();

  } else {
    log("Prefs not found :()");
  }
  }
void setupWifi() {
  // 
  // if local hotspot exists, connect to it
  // if local hotspot does not exist, create one

  log("Scanning for networks...");

  int n = WiFi.scanNetworks();
  // prefs.putString(HOMEWIFISSID, "");
  // prefs.putString(HOMEWIFIPASS, "");

  //Check for local hotspot. If it exists, connect to it and set fixed IP address.

  for (int i = 0; i < n; i++) {
    if (WiFi.SSID(i) == "dontlookup") {
      log("Connecting to 'dontlookup'...");

      if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
        log("STA Failed to configure");
        WiFi.begin("dontlookup", "dontlookdown");
      
        logIP();
      }
      
      return;
    }
  }

  log("Creating access point");
  WiFi.softAP("dontlookup", "dontlookdown");
}
