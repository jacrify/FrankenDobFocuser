
#include "ChuckController.h"
#include "CurrentChukState.h"
#include "NunchukShared.h"
#include <unity.h> // Include the Unity test framework.

void testModeChangeTo1(void) {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  state.z = 1;
  state.c = 0;
  state.x = 100;
  state.y = 0;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getMode(), "Mode should be 1");
  TEST_ASSERT_EQUAL_INT_MESSAGE(2, controller.getLedsFlashCycle1(),
                                "Led 1 should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(
      0, controller.getLedsFlashCycle2(),
      "Led  should be flashing to indicate mode change");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getFlashFast(),
                                "Flashing should be slow");
}

void testModeChangeTo1Diag(void) {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  state.z = 1;
  state.c = 0;
  state.x = 70;
  state.y = 100;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getMode(), "Mode should be 0");
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getLedsFlashCycle1(),
                                "Led 0 should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(
      0, controller.getLedsFlashCycle2(),
      "Led should be flashing to indicate mode change");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getFlashFast(),
                                "Flashing should be slow");

  state.z = 1;
  state.c = 0;
  state.x = 70;
  state.y = -100;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(2, controller.getMode(), "Mode should be 2");

  TEST_ASSERT_EQUAL_INT_MESSAGE(4, controller.getLedsFlashCycle1(),
                                "Led 2 should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(
      0, controller.getLedsFlashCycle2(),
      "Led should be flashing to indicate mode change");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getFlashFast(),
                                "Flashing should be slow");
}

void testModeChangeTo2(void) {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  state.z = 1;
  state.c = 0;
  state.x = 0;
  state.y = -100;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(2, controller.getMode(), "Mode should be 2");
  TEST_ASSERT_EQUAL_INT_MESSAGE(4, controller.getLedsFlashCycle1(),
                                "Led 2 should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(
      0, controller.getLedsFlashCycle2(),
      "Led should be flashing to indicate mode change");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getFlashFast(),
                                "Flashing should be slow");
}

void testModeChangeTo3(void) {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  state.z = 1;
  state.c = 0;
  state.x = -100;
  state.y = 0;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(3, controller.getMode(), "Mode should be 3");
  TEST_ASSERT_EQUAL_INT_MESSAGE(8, controller.getLedsFlashCycle1(),
                                "Led 3 should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(
      0, controller.getLedsFlashCycle2(),
      "Led should be flashing to indicate mode change");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getFlashFast(),
                                "Flashing should be slow");
}

void testSpeedAndModeChange() {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  controller.setModeParameters(0, 100, 1100);
  controller.setModeParameters(1, 1000, 11000);
  controller.setMode(0);

  state.z = 0;
  state.c = 0;
  state.x = 0;
  state.y = -100;

  // Go
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(-1100, controller.getSpeed(),
                                "Should be running backward");

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getLedsFlashCycle1(),
                                "Led 0 should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getLedsFlashCycle2(),
                                "Led 0 should be flashing");
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getFlashFast(),
                                "Flashing should be fast");

  // change to mode 1
  state.z = 1;
  state.c = 0;
  state.x = 100;
  state.y = 0;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getMode(),
                                "Should have changed to mode 1");
  TEST_ASSERT_EQUAL_INT_MESSAGE(2, controller.getLedsFlashCycle1(),
                                "Led 1 should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getLedsFlashCycle2(),
                                "Led should be flashing");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getFlashFast(),
                                "Flashing should be slow");

  state.z = 0;
  state.c = 0;
  state.x = 0;
  state.y = -100;

  // Go
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(-11000, controller.getSpeed(),
                                "Should be running faster backward");
  TEST_ASSERT_EQUAL_INT_MESSAGE(2, controller.getLedsFlashCycle1(),
                                "Led 1 should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getLedsFlashCycle2(),
                                "Led 1 should be flashing");
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getFlashFast(),
                                "Flashing should be fast");
}

void testSpeedMappingDown() {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  controller.setModeParameters(0, 100, 1100);
  controller.setMode(0);

  state.z = 0;
  state.c = 0;
  state.x = 0;
  state.y = -100;

  // Go
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(-1100, controller.getSpeed(),
                                "Should be running backward");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getMode(), "Should be mode 0");
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getLedsFlashCycle1(),
                                "Led 0 should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getLedsFlashCycle2(),
                                "Led should be flashing");
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getFlashFast(),
                                "Flashing should be fast");
}

void testSpeedMappingUp() {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  controller.setModeParameters(0, 100, 1100);
  controller.setMode(0);

  state.z = 0;
  state.c = 0;
  state.x = 0;
  state.y = 100;

  // Go
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(1100, controller.getSpeed(),
                                "Should be running forward");

  // Stop

  state.y = 0;
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getSpeed(), "Should be stopped");

  state.y = 60; // deadzone is 20, max is 100. 60 should be halfway
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(600, controller.getSpeed(),
                                "Should be running half speed");

  state.y = 127;
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(1100, controller.getSpeed(),
                                "Should be running");

  state.y = 19; // lower than deadzone
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getSpeed(), "Should be stopped");
}

void testFindLimit() {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  controller.setModeParameters(0, 100, 1100);
  controller.setModeParameters(4, 1000, 11000); // mode four is find limit mode
  controller.setMode(0);

  state.z = 1;
  state.c = 1;
  state.x = 0;
  state.y = 100;
  state.millis = 1000;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getSpeed(),
                                "Should be stopped, time limit not hit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.isLimitFindingModeOn(),
                                "Should not be in limit finding mode");

  state.millis = 2000;
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getSpeed(),
                                "Should be stopped, time limit not hit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.isLimitFindingModeOn(),
                                "Should not be in limit finding mode");

  state.millis = 4500;
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(11000, controller.getSpeed(),
                                "Should be running forward at limit speed");
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.isLimitFindingModeOn(),
                                "Should be in limit finding mode");

  TEST_ASSERT_EQUAL_INT_MESSAGE(15, controller.getLedsFlashCycle1(),
                                "All leds should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getLedsFlashCycle2(),
                                "Leds should be flashing");
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getFlashFast(),
                                "Flashing should be fast");

  state.z = 0;
  state.c = 0;
  state.x = 0;
  state.y = 0;
  state.millis = 4600;
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(
      0, controller.getSpeed(),
      "Should be stopped at limit position (as if manually)");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.isLimitFindingModeOn(),
                                "Should not be in limit finding mode");

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getLedsFlashCycle1(),
                                "Led1 should be lit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getLedsFlashCycle2(),
                                "Led should not be flashing");

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, controller.getAndFlipLimitFlag(),
                                "Limit flag should be set to reset motor unit");
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, controller.getAndFlipLimitFlag(),
                                "Second call for limit flag should be zero");
}

void testCPressed() {

  wii_i2c_nunchuk_state state;
  state.z = 1;
  state.c = 1;
  state.x = 0;
  state.y = 0;

  CurrentChukState chukState;
  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isCPushed(),
                                "C Should be not pressed when z is");

  state.z = 0;
  state.c = 1;
  state.x = 0;
  state.y = 0;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isCPushed(),
                                "C Should be pressed");

  state.z = 0;
  state.c = 0;
  state.x = 0;
  state.y = 0;

  chukState.processState(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isCPushed(),
                                "C Should be not pressed");
  TEST_ASSERT_EQUAL_INT_MESSAGE(
      1, chukState.isCReleased(),
      "C Should be be flagged as released after pressed");
}

void testCHeld() {

  wii_i2c_nunchuk_state state;
  state.z = 0;
  state.c = 1;
  state.x = 0;
  state.y = 0;
  state.millis = 1;

  CurrentChukState chukState;
  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isCPushed(),
                                "C Should be  pressed ");

  state.z = 0;
  state.c = 1;
  state.x = 0;
  state.y = 0;
  state.millis = 100;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isCPushed(),
                                "C Should be still be pressed ");

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isCHeld(),
                                "C Should be not yet  marked as held ");

  state.z = 0;
  state.c = 1;
  state.x = 0;
  state.y = 0;
  state.millis = 3000;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isCPushed(),
                                "C Should be pressed ");

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isCHeld(),
                                "C Should be marked as held ");

  state.z = 0;
  state.c = 0;
  state.x = 0;
  state.y = 0;
  state.millis = 4000;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isCPushed(),
                                "C Should not be pressed ");

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isCHeld(),
                                "C Should not be marked as held ");
}

void testZHeld() {

  wii_i2c_nunchuk_state state;
  state.z = 1;
  state.c = 0;
  state.x = 0;
  state.y = 0;
  state.millis = 1;

  CurrentChukState chukState;
  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isZPushed(),
                                "Z Should be  pressed ");

  state.z = 1;
  state.c = 0;
  state.x = 0;
  state.y = 0;
  state.millis = 100;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isZPushed(),
                                "Z Should be still be pressed ");

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isZHeld(),
                                "Z Should be not yet  marked as held ");

  state.z = 1;
  state.c = 0;
  state.x = 0;
  state.y = 0;
  state.millis = 3000;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isZPushed(),
                                "Z Should be pressed ");

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isZHeld(),
                                "Z Should be marked as held ");

  state.z = 0;
  state.c = 0;
  state.x = 0;
  state.y = 0;
  state.millis = 4000;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isZPushed(),
                                "Z Should not be pressed ");

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isZHeld(),
                                " Z Should not be marked as held ");
}

void testBothHeld() {

  wii_i2c_nunchuk_state state;
  state.z = 1;
  state.c = 1;
  state.x = 0;
  state.y = 0;
  state.millis = 1;

  CurrentChukState chukState;
  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isBothPushed(),
                                "Both Should be  pressed ");

  state.z = 1;
  state.c = 1;
  state.x = 0;
  state.y = 0;
  state.millis = 100;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isBothPushed(),
                                "Both Should be still be pressed ");

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isBothHeld(),
                                "Both Should be not yet  marked as held ");

  state.z = 1;
  state.c = 1;
  state.x = 0;
  state.y = 0;
  state.millis = 3000;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isBothPushed(),
                                "Both Should be pressed ");

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isBothHeld(),
                                "Both Should be marked as held ");

  state.z = 0;
  state.c = 1;
  state.x = 0;
  state.y = 0;
  state.millis = 4000;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isBothPushed(),
                                "Both Should not be pressed ");

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isBothHeld(),
                                " Both Should not be marked as held ");
  state.z = 1;
  state.c = 0;
  state.x = 0;
  state.y = 0;
  state.millis = 4000;

  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isBothPushed(),
                                "Both Should not be pressed ");

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isBothHeld(),
                                " Both Should not be marked as held ");
}

void testZPressed() {

  wii_i2c_nunchuk_state state;
  state.z = 1;
  state.c = 0;
  state.x = 0;
  state.y = 0;
  CurrentChukState chukState;
  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, chukState.isZPushed(),
                                "Z Should be pressed");

  state.z = 0;
  state.c = 0;
  state.x = 0;
  state.y = 0;

  chukState.processState(state);
  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isZPushed(),
                                "Z Should be not pressed");
  // TEST_ASSERT_EQUAL_INT_MESSAGE(
  //     0, processedState.isZReleased(),
  //     "Z Should be be flagged as released after pressed");
}

void testIsUp() {

  wii_i2c_nunchuk_state state;
  state.z = 0;
  state.c = 0;
  state.x = 0;
  state.y = -100;
  CurrentChukState chukState;
  chukState.processState(state);

  TEST_ASSERT_EQUAL_INT_MESSAGE(0, chukState.isUp(), "Should be up");
}

void setup() {
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(testModeChangeTo1);
  RUN_TEST(testModeChangeTo1Diag);
  RUN_TEST(testModeChangeTo2);
  RUN_TEST(testModeChangeTo3);
  RUN_TEST(testSpeedMappingUp);
  RUN_TEST(testSpeedMappingDown);
  RUN_TEST(testSpeedAndModeChange);
  RUN_TEST(testFindLimit);
  
  RUN_TEST(testIsUp);
  RUN_TEST(testCPressed);
  RUN_TEST(testZPressed);
  RUN_TEST(testCHeld);
  RUN_TEST(testZHeld);
  RUN_TEST(testBothHeld);

  // RUN_TEST(testMemoryModeHold);

  UNITY_END(); // IMPORTANT LINE!
}

void loop() {
  // Do nothing here.
}

int main() {
  setup();
  return 0;
}
