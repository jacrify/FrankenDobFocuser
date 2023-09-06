#ifndef MYWEBSERVER_H
#define MYWEBSERVER_H

#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <MotorUnit.h>

void setupWebServer(MotorUnit &motor);

#endif
