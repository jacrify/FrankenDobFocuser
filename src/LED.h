#ifndef LED_H
#define LED_H

#include <Arduino.h>

#define LEDPIN1 12
#define LEDPIN2 25
#define LEDPIN3 26
#define LEDPIN4 27

#define BRIGHTER_PIN 33
#define DIMMER_PIN 32

#define BRIGHTNESS_STEP 5

#define CAPACITIVE_THRESHOLD 30

bool brighter_touched;
bool dimmer_touched;

unsigned long lastCycleTime = 0;

#define FASTBLINKMILLIS 50
#define SLOWBLINKMILLIS 300

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

int channels[] = {0, 1, 2, 3}; // PWM channels

// int brightnessDim = 10;    // Dim brightness level
int brightnessBright = 200; // Bright brightness level
#define DIM_RATIO 20

int ledPins[] = {LEDPIN1, LEDPIN2, LEDPIN3, LEDPIN4};

void lightLed(int ledno) {
  if (ledno >= 0 && ledno < sizeof(ledPins) / sizeof(ledPins[0])) {
    ledcWrite(channels[ledno], brightnessBright);
  }
}

void dimLed(int ledno) {
  if (ledno >= 0 && ledno < sizeof(ledPins) / sizeof(ledPins[0])) {
    ledcWrite(channels[ledno], brightnessBright / DIM_RATIO);
  }
}

void ledSetup() {
  ledcSetup(ledChannel, freq, resolution);

  for (int i = 0; i < sizeof(ledPins) / sizeof(ledPins[0]); i++) {
    ledcSetup(channels[i], 5000, 8);
    ledcAttachPin(ledPins[i], channels[i]);
    ledcWrite(channels[i], brightnessBright / DIM_RATIO);
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
  // check pins to see if brightness should change
  int brighter_value = touchRead(BRIGHTER_PIN);
  if (brighter_value < CAPACITIVE_THRESHOLD) {
    if (!brighter_touched) {
      brighter_touched = true;
      if (brightnessBright + BRIGHTNESS_STEP <= 255)
        brightnessBright +=  BRIGHTNESS_STEP;
    }
    // do nothing otherwise, we've already changed the value for this touch
  } else {
    brighter_touched = false;
  }

  int dimmer_value = touchRead(DIMMER_PIN);
  if (dimmer_value < CAPACITIVE_THRESHOLD) {
    if (!dimmer_touched) {
      dimmer_touched = true;
      if (brightnessBright - BRIGHTNESS_STEP > 0)
        brightnessBright -=   BRIGHTNESS_STEP;
    }
    // do nothing otherwise, we've already set the value changed the value for
    // this touch
  } else {
    dimmer_touched = false;
  }

  unsigned long now = millis();
  int divisor = fast ? FASTBLINKMILLIS : SLOWBLINKMILLIS;
  if ((now / divisor) % 2)
    setLeds(ledCycle1);
  else
    setLeds(ledCycle2);
}
#endif
