#include "dumbmatrix.hpp"

#include <assert.h>

#include <esp32-hal-gpio.h>

#define SET_PIN(var, pin, state) \
  state \
    ? var |= (1 << pin) \
    : var &= ~(1 << pin);

namespace dm {

void DumbMatrix::init_gpio() const {
  ::pinMode(PIN_R1, OUTPUT);
  ::pinMode(PIN_B1, OUTPUT);
  ::pinMode(PIN_G1, OUTPUT);

  ::pinMode(PIN_R2, OUTPUT);
  ::pinMode(PIN_B2, OUTPUT);
  ::pinMode(PIN_G2, OUTPUT);

  ::pinMode(PIN_A, OUTPUT);
  ::pinMode(PIN_B, OUTPUT);
  ::pinMode(PIN_C, OUTPUT);
  ::pinMode(PIN_D, OUTPUT);

  ::pinMode(PIN_LAT, OUTPUT);
  ::pinMode(PIN_OE, OUTPUT);
  ::pinMode(PIN_CLK, OUTPUT);
}

void DumbMatrix::paint_row(const TSize row) const {
  // make a copy of the GPIO state
  uint32_t gpio = GPIO.out;

  for (TSize column = 0; column < _columns; ++column) {
    // Set the colours
    SET_PIN(gpio, PIN_R1, _pixels[row][column].r != 0);
    SET_PIN(gpio, PIN_G1, _pixels[row][column].g != 0);
    SET_PIN(gpio, PIN_B1, _pixels[row][column].b != 0);
    SET_PIN(gpio, PIN_R2, _pixels[row + 16][column].r != 0);
    SET_PIN(gpio, PIN_G2, _pixels[row + 16][column].g != 0);
    SET_PIN(gpio, PIN_B2, _pixels[row + 16][column].b != 0);

    // commit the configuration to the GPIO output
    GPIO.out = gpio;

    // Pulse clock
    GPIO.out_w1ts = (1 << PIN_CLK);
    GPIO.out_w1tc = (1 << PIN_CLK);
  }

  // setup the BCD-encoded row selector
  SET_PIN(gpio, PIN_A, row & 1);
  SET_PIN(gpio, PIN_B, (row >> 1) & 1);
  SET_PIN(gpio, PIN_C, (row >> 2) & 1);
  SET_PIN(gpio, PIN_D, (row >> 3) & 1);

  // commit the configuration to the GPIO output
  GPIO.out = gpio;

  // Disable display
  GPIO.out_w1ts = (1 << PIN_OE);

  // Pulse latch
  GPIO.out_w1ts = (1 << PIN_LAT);
  GPIO.out_w1tc = (1 << PIN_LAT);

  // Enable display
  GPIO.out_w1tc = (1 << PIN_OE);
}

DumbMatrix::DumbMatrix() {
  init_gpio();
  blank();
}

void DumbMatrix::set(const Point p, const RGB24 c) {
  assert(p.x < _columns && p.y < _rows);
  _pixels[p.y][p.x] = c;
}

void DumbMatrix::blank() {
  ::memset(_pixels, 0, _columns * _rows * sizeof(RGB24));
}

void DumbMatrix::fill(const RGB24 c) {
  for (TSize row = 0; row < _rows; ++row) {
    for (TSize column = 0; column < _columns; ++column) {
      set({column, row}, c);
    }
  }
}

void DumbMatrix::update() const {
  for (TSize row = 0; row < (_rows / 2); ++row) {
    paint_row(row);
  }
}

} // namespace dm
