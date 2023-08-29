
// #include "NunchukShared.h"
// #include <unity.h> // Include the Unity test framework.

// void testIsUp() {

//   wii_i2c_nunchuk_state state;
//   ChuckState processedState;

//   state.z = 0;
//   state.c = 0;
//   state.x = 0;
//   state.y = -100;

//   processedState.processChuckState(state);
//   TEST_ASSERT_EQUAL_INT_MESSAGE(0, processedState.isUp(), "Should be up");
// }
// void setup() {
//   UNITY_BEGIN(); // IMPORTANT LINE!
//   RUN_TEST(testIsUp);

//   UNITY_END(); // IMPORTANT LINE!
// }

// void loop() {
//   // Do nothing here.
// }

// int main() {
//   setup();
//   return 0;
// }
