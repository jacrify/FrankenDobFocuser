#ifndef LED_H
#define LED_H

#include <Arduino.h>

#define LEDPIN1 12
#define LEDPIN2 25
#define LEDPIN3 26
#define LEDPIN4 27

unsigned long lastCycleTime = 0;

#define FASTBLINKMILLIS 50
#define SLOWBLINKMILLIS 300

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

int channels[] = {0, 1, 2, 3}; // PWM channels

int brightnessDim = 10;    // Dim brightness level
int brightnessBright = 200; // Bright brightness level

int ledPins[] = {LEDPIN1, LEDPIN2, LEDPIN3, LEDPIN4};

void lightLed(int ledno) {
  if (ledno >= 0 && ledno < sizeof(ledPins) / sizeof(ledPins[0])) {
    ledcWrite(channels[ledno], brightnessBright);
  }
}

void dimLed(int ledno) {
  if (ledno >= 0 && ledno < sizeof(ledPins) / sizeof(ledPins[0])) {
    ledcWrite(channels[ledno], brightnessDim);
  }
}

void ledSetup() {
  ledcSetup(ledChannel, freq, resolution);

  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++) {
    ledcSetup(channels[i], 5000, 8);
    ledcAttachPin(ledPins[i], channels[i]);
    ledcWrite(channels[i], brightnessDim);
  }
}

void setLeds(int bitwise) {
  for (int i = 0; i < 4; ++i) { // Assuming you have 4 LEDs
    if (bitwise & (1 << i)) {
      lightLed(i);
    } else {
      dimLed(i);
    }
  }
}

void ledLoop(int ledCycle1, int ledCycle2, bool fast) {
  unsigned long now = millis();
  int divisor = fast ? FASTBLINKMILLIS : SLOWBLINKMILLIS;
  if ((now / divisor) % 2)
    setLeds(ledCycle1);
  else
    setLeds(ledCycle2);
}
#endif
