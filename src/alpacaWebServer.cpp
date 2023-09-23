#include "alpacaWebServer.h"
#include "AsyncUDP.h"
#include "Logging.h"

#include <ArduinoJson.h> // Include the library
#include <ESPAsyncWebServer.h>

#include <WebSocketsClient.h>

#include <time.h>

unsigned const int localPort = 32227; // The Alpaca Discovery test port
unsigned const int alpacaPort =
    80; // The  port that the Alpaca API would be available on

AsyncUDP alpacaUdp;

AsyncWebServer alpacaWebServer(80);

void handleNotFound(AsyncWebServerRequest *request) {
  log("Not found URL is %s", request->url().c_str());
  String method;

  switch (request->method()) {
  case HTTP_GET:
    method = "GET";
    break;
  case HTTP_POST:
    method = "POST";
    break;
  case HTTP_DELETE:
    method = "DELETE";
    break;
  case HTTP_PUT:
    method = "PUT";
    break;
  case HTTP_PATCH:
    method = "PATCH";
    break;
  case HTTP_HEAD:
    method = "HEAD";
    break;
  case HTTP_OPTIONS:
    method = "OPTIONS";
    break;
  default:
    method = "UNKNOWN";
    break;
  }

  log("Not found Method is %s", method.c_str());

  request->send(404, "application/json");
}
void returnDeviceDescription(AsyncWebServerRequest *request) {
  log("returnDeviceDescription url is  %s", request->url().c_str());

  char buffer[300];
  snprintf(buffer, sizeof(buffer),
           R"({
         "Value": {
          "ServerName": "myserver",
          "Manufacturer": "me",
          "ManufacturerVersion": "1",
          "Location": "here"
          },
        "ClientTransactionID": 0,
         "ServerTransactionID": 0
        })");

  String json = buffer;
  request->send(200, "application/json", json);
}

void returnConfiguredDevices(AsyncWebServerRequest *request) {
  log("returnConfiguredDevices url is  %s", request->url().c_str());

  char buffer[300];
  snprintf(buffer, sizeof(buffer),
           R"({
            "Value": [ {
          "DeviceName": "Focuser",
          "DeviceType": "Focuser",
         "DeviceNumber": 1,
          "UniqueID": "FrankenDobFocuser"
          }],
        "ClientTransactionID": 0,
         "ServerTransactionID": 0
        })");

  String json = buffer;
  request->send(200, "application/json", json);
}

void returnApiVersions(AsyncWebServerRequest *request) {
  log("returnApiVersions url is  %s", request->url().c_str());

  char buffer[300];
  snprintf(buffer, sizeof(buffer),
           R"({
         "Value": [1],
        "ClientTransactionID": 0,
         "ServerTransactionID": 0
        })");

  String json = buffer;
  request->send(200, "application/json", json);
}

void returnEmptyArray(AsyncWebServerRequest *request) {
  log("Empty array value url is %s", request->url().c_str());

  char buffer[300];
  snprintf(buffer, sizeof(buffer),
           R"({
             "ErrorNumber": %d,
             "ErrorMessage": "%s",
             "Value": []
      })",
           0, "");

  String json = buffer;
  request->send(200, "application/json", json);
}

void returnSingleString(AsyncWebServerRequest *request, String s) {
  log("Single string value url is %s, string is %s", request->url().c_str(), s);

  char buffer[300];
  snprintf(buffer, sizeof(buffer),
           R"({
             "ErrorNumber": %d,
             "ErrorMessage": "%s",
             "Value": "%s"
      })",
           0, "", s);

  String json = buffer;
  request->send(200, "application/json", json);
}

void returnSingleInteger(AsyncWebServerRequest *request, int value) {
  log("Single int value url is %s, int is %ld", request->url().c_str(), value);
  char buffer[300];
  snprintf(buffer, sizeof(buffer),
           R"({
             "ErrorNumber": %d,
             "ErrorMessage": "%s",
             "Value": %ld
      })",
           0, "", value);

  String json = buffer;
  request->send(200, "application/json", json);
}

void returnSingleDouble(AsyncWebServerRequest *request, double d) {
  log("Single double value url is %s, double is %lf", request->url().c_str(),
      d);
  char buffer[300];
  snprintf(buffer, sizeof(buffer),
           R"({
             "ErrorNumber": %d,
             "nrrorMessage": "%s",
             "Value": %lf
      })",
           0, "", d);

  String json = buffer;
  request->send(200, "application/json", json);
}

void returnSingleBool(AsyncWebServerRequest *request, bool b) {
  log("Single bool value url is %s, bool is %d", request->url().c_str(), b);
  char buffer[300];
  snprintf(buffer, sizeof(buffer),
           R"({
             "ErrorNumber": %d,
             "ErrorMessage": "%s",
             "Value": %s
      })",
           0, "", b ? "true" : "false");

  String json = buffer;
  request->send(200, "application/json", json);
}

void returnNoError(AsyncWebServerRequest *request) {

  // log("Returning no error for url %s ", request->url().c_str());

  static const char *json = R"({
           "ClientTransactionID": 0,
           "ServerTransactionID": 0,
           "ErrorNumber": 0,
           "ErrorMessage": ""
          })";
  request->send(200, "application/json", json);
}

void setupWebServer(MotorUnit &motor) {

  // set up alpaca discovery udp
  if (alpacaUdp.listen(32227)) {
    log("Listening for alpaca discovery requests...");
    alpacaUdp.onPacket([](AsyncUDPPacket packet) {
      log("Received alpaca UDP Discovery packet ");
      if ((packet.length() >= 16) &&
          (strncmp("alpacadiscovery1", (char *)packet.data(), 16) == 0)) {
        log("Responding to alpaca UDP Discovery packet with my port number "
            "%d",
            alpacaPort);

        packet.printf("{\"AlpacaPort\": %d}", alpacaPort);
      }
    });
  }

  // GETS. Mostly default flags.
  alpacaWebServer.on(
      "^\\/api\\/v1\\/focuser\\/1\\/.*$", HTTP_GET,
      [&motor](AsyncWebServerRequest *request) {
        String url = request->url();
        // log("Processing GET on url %s", url.c_str());

        // Strip off the initial portion of the URL
        String subPath = url.substring(String("/api/v1/focuser/1/").length());
        
        if (subPath == "absolute")
          return returnSingleBool(request, true);
        if (subPath == "ismoving")
          return returnSingleBool(request, motor.isMoving());
        if (subPath == "maxincrement")
          return returnSingleInteger(request, motor.getMaxIncrement());
        if (subPath == "maxstep")
          return returnSingleInteger(request, motor.getLimitPosition());
        if (subPath == "tempcomp" || subPath == "tempcompavailable")
          return returnSingleBool(request, false);
        if (subPath == "temperature")
          return returnSingleDouble(request, 0); // brrrr
        if (subPath == "position")
          return returnSingleInteger(request, motor.getPosition());
        if (subPath == "interfaceversion") {
          return returnSingleInteger(request, 3);
        }

        if (subPath == "driverversion" || subPath == "driverinfo" ||
            subPath == "description" || subPath == "name") {
          String responseValue = "Frankendob"; // Default
          if (subPath == "driverversion")
            responseValue = "1.0";
          else if (subPath == "driverinfo")
            responseValue = "Hackypoo";
          return returnSingleString(request, responseValue);
        }

        if (subPath == "supportedactions") {
          return returnEmptyArray(request);
        }

        if (subPath == "connected") {
          return returnSingleBool(request, true);
        }
        return handleNotFound(request);
      });

  // Mangement API ================
  alpacaWebServer.on(
      "^\\/management\\/.*$", HTTP_GET, [](AsyncWebServerRequest *request) {
        String url = request->url().c_str();
        log("Processing GET on management url %s", url.c_str());
        // Strip off the initial portion of the URL
        String subPath = url.substring(String("/management/").length());

        if (subPath == "apiversions")
          return returnApiVersions(request);

        if (subPath == "v1/configureddevices")
          return returnConfiguredDevices(request);

        if (subPath == "v1/description")
          return returnDeviceDescription(request);
          

        return handleNotFound(request);
      });

  // ======================================
  //  PUTS implementation

  alpacaWebServer.on("^\\/api\\/v1\\/focuser\\/1\\/.*$", HTTP_PUT,
                     [&motor](AsyncWebServerRequest *request) {
                       String url = request->url();
                       log("Processing PUT on url %s", url.c_str());
                       // Strip off the initial portion of the URL
                       String subPath =
                           url.substring(String("/api/v1/focuser/1/").length());

                       if (subPath== "halt") {
                         motor.stop();
                         return returnNoError(request);
                       }
                       if (subPath == "move") {
                         String pos = request->arg("Position");
                         if (pos != NULL) {
                           log("Received parameterName: %s", pos.c_str());

                           try {
                             int32_t parsedValue = pos.toInt();
                             log("Moving to %ld", parsedValue);
                                 motor.moveTo(parsedValue, 64000);
                           } catch (const std::invalid_argument &e) {
                             log("Error parsing payload, invalid arg %s", e);
                           } catch (const std::out_of_range &e) {
                             log("Error parsing payload , out of range %s", e);
                           }
                         }
                         return returnNoError(request);
                       }

                       //  return returnNoError(request);
                       if (subPath == "connected")
                         return returnNoError(request);

                       return handleNotFound(request);
                     });

  // ===============================

  // alpacaWebServer.serveStatic("/", LittleFS, "/fs/");

  alpacaWebServer.onNotFound(
      [](AsyncWebServerRequest *request) { handleNotFound(request); });

  alpacaWebServer.begin();
  log("Server started");
  return;
}
