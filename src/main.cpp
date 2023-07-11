#include "wii_i2c.h"
#include <Arduino.h>

#define PIN_SDA 23
#define PIN_SCL 22

// ESP32 I2C port (0 or 1):
#define WII_I2C_PORT 0

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  if (wii_i2c_init(WII_I2C_PORT, PIN_SDA, PIN_SCL) != 0) {
    Serial.printf("Error initializing nunchuk :(");
    return;
  }
  wii_i2c_request_state();
}

void loop() {
  const unsigned char *data = wii_i2c_read_state();
  wii_i2c_request_state();
  if (!data) {
    Serial.printf("no data available :(");
  } else {
    wii_i2c_nunchuk_state state;
    wii_i2c_decode_nunchuk(data, &state);
    Serial.printf("Stick position: (%d,%d), C button is %s, Z button is %s\n",
                  state.x, state.y, (state.c) ? "pressed" : "not pressed",
                  (state.z) ? "pressed" : "not pressed");
  }
  delay(100);
}
