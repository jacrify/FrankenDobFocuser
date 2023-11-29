#include "UDPSender.h"
#include "AsyncUDP.h"
#include "Logging.h"
#include "WiFi.h"
#include <ArduinoJson.h>

#define IPBROADCASTPERIOD 500
#define IPBROADCASTPORT 50375
AsyncUDP udp;
unsigned long lastCommandTime;

void sendMoveAxisPercentCommand(int axis,int percentToMove) {

  long now = millis();
  // always send stop commands. Other than that don't flood:
  // loop is 20ms, that's too fast.
  if (percentToMove == 0 || (now - lastCommandTime) > IPBROADCASTPERIOD) {
    lastCommandTime = now;
    // Check if the device is connected to the WiFi
    if (WiFi.status() != WL_CONNECTED) {
      log("Wifi not connected");
      return;
    }
    if (udp.connect(
            IPAddress(255, 255, 255, 255),
            IPBROADCASTPORT)) { // Choose any available port, e.g., 12345

      // Estimate JSON capacity
      const size_t capacity = JSON_OBJECT_SIZE(4);

      DynamicJsonDocument doc(capacity);
      // Populate the JSON object
      doc["command"] = "moveaxispercentage";
      doc["parameter1"] = axis;
      doc["parameter2"] = percentToMove;

      String json;
      serializeJson(doc, json);
      json = "EQ:" + json;
      udp.print(json.c_str());

      log("EQ Command command sent %s", json.c_str());
    }
    // }
  }
}