#ifndef NETWORK_H
#define NETWORK_H

#include <Preferences.h>

void setupWifi();
void setupWifiHome(Preferences &prefs);
void loopNetwork(Preferences &prefs);
#endif
