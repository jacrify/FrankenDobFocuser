#ifndef NUNCHUK_SHARED
#define NUNCHUK_SHARED

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
};

#endif