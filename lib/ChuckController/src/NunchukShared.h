#ifndef NUNCHUK_SHARED
#define NUNCHUK_SHARED

// #define DEADZONE 20
// #define MAX_AXIS

struct wii_i2c_nunchuk_state {
  // accelerometer
  short int acc_x;
  short int acc_y;
  short int acc_z;

  // analog stick:
  signed char x;
  signed char y;

  // buttons:
  char c;
  char z;

  // timestamp
  unsigned long millis;
};

#endif