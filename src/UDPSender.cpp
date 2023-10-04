#include "UDPSender.h"
#include "AsyncUDP.h"
#include "Logging.h"
#include "WiFi.h"

#define IPBROADCASTPERIOD 500
#define IPBROADCASTPORT 50375
AsyncUDP udp;
unsigned long lastCommandTime;

void sendMoveAxisPercentCommand(int percentToMove) {

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
      char response[200];

      snprintf(response, sizeof(response),
               "EQ:{ "
               "\"command\": \"%s\", "
               "\"parameter1\": %d,"
               "\"parameter2\": 0"
               " }\n",
               "moveaxispercentage", percentToMove);
     
      udp.print(response);
      // log("EQ Command send: %s",response);

    }
    // }
  }
}