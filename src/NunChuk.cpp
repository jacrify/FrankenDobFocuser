#include "NunChuk.h"
#include "wii_i2c.h"
#include <Arduino.h>
#include "LED.h"

ChuckController controller; // all logic delated here

int NunChuk::getSpeed() { return controller.getSpeed(); }
void NunChuk::setUpNunChuk() {
  if (wii_i2c_init(WII_I2C_PORT, PIN_SDA, PIN_SCL) != 0) {
    Serial.printf("Error initializing nunchuk :(");
    return;
  }
  wii_i2c_request_state();

  controller.setModeParameters(4, 16000, 64000);
  controller.setModeParameters(3, 200, 1000);
  controller.setModeParameters(2, 1000, 4000);
  controller.setModeParameters(1, 4000, 16000);
  controller.setModeParameters(0, 16000, 64000);
  controller.setMode(0);
  ledSetup();

}
void NunChuk::nunChukLoop() {
  const unsigned char *data = wii_i2c_read_state();
  wii_i2c_request_state();
  if (!data) {
    Serial.printf("no data available :(");
  } else {
    wii_i2c_nunchuk_state state;
    wii_i2c_decode_nunchuk(data, &state);
    state.millis = millis(); // Pass current time in for long press logic
    controller.processChuckData(state);
    ledLoop(controller.getLedsFlashCycle1(), controller.getLedsFlashCycle2(), controller.getFlashFast());
  }
}
int NunChuk::isLimitFinding() { return controller.isLimitFindingModeOn(); }
// int NunChuk::memoryMoveRequested() { return controller.getAndFlipMemoryMoveFlag() ; }
// int NunChuk::isMemorySetRequested() {
//   return controller.;
// }
int NunChuk::resetLimitRequested() { return controller.getAndFlipLimitFlag(); }
