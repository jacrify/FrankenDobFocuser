
#include "ChuckController.h"
#include <unity.h> // Include the Unity test framework.

void testModeChangeTo1(void) {
  wii_i2c_nunchuk_state state;
  ChuckController controller;
  
  state.z=1;
  state.c=0;
  state.x=100;
  state.y=0;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_FLOAT(1, controller.getMode()); 
}

void testModeChangeTo1Diag(void) {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  state.z = 1;
  state.c = 0;
  state.x = 70;
  state.y = 100;
  
  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_FLOAT(0, controller.getMode());

  state.z = 1;
  state.c = 0;
  state.x = 70;
  state.y = -100;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_FLOAT(2, controller.getMode());
}

void testModeChangeTo2(void) {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  state.z = 1;
  state.c = 0;
  state.x = 0;
  state.y = -100;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_FLOAT(2, controller.getMode());
}

void testModeChangeTo3(void) {
  wii_i2c_nunchuk_state state;
  ChuckController controller;

  state.z = 1;
  state.c = 0;
  state.x = -100;
  state.y = 0;

  controller.processChuckData(state);
  TEST_ASSERT_EQUAL_FLOAT(3, controller.getMode());
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
  TEST_ASSERT_EQUAL_FLOAT(1100, controller.getSpeed());
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
    TEST_ASSERT_EQUAL_FLOAT(1100, controller.getSpeed());
    // Stop

    state.y = 0;
    controller.processChuckData(state);
    TEST_ASSERT_EQUAL_FLOAT(0, controller.getSpeed());

    state.y = 60; // deadzone is 20, max is 100. 60 should be halfway
    controller.processChuckData(state);
    TEST_ASSERT_EQUAL_FLOAT(600, controller.getSpeed());

    state.y = 127;
    controller.processChuckData(state);
    TEST_ASSERT_EQUAL_FLOAT(1100, controller.getSpeed());

    state.y = 19; // lower than deadzone
    controller.processChuckData(state);
    TEST_ASSERT_EQUAL_FLOAT(00, controller.getSpeed());
  }
  void setup() {
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(testModeChangeTo1);
    RUN_TEST(testModeChangeTo1Diag);
    RUN_TEST(testModeChangeTo2);
    RUN_TEST(testModeChangeTo3);
    RUN_TEST(testSpeedMappingUp);
    UNITY_END(); // IMPORTANT LINE!
  }

  void loop() {
    // Do nothing here.
  }

  int main() {
    setup();
    return 0;
  }
