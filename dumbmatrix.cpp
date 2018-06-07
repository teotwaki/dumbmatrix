#include "dumbmatrix.hpp"

#include <assert.h>

#include <esp32-hal-gpio.h>

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

void DumbMatrix::enable_display() const {
  GPIO.out &= ~(1 << PIN_OE);
}

void DumbMatrix::disable_display() const {
  GPIO.out |= (1 << PIN_OE);
}

#define SET_PIN(var, pin, state) state ? var |= (1 << pin) : var &= ~(1 << pin);

void DumbMatrix::gpio_select_row(const TSize row) const {
  // make a copy of the GPIO state
  uint32_t gpio = GPIO.out;

  // setup the BCD-encoded row selector
  SET_PIN(gpio, PIN_A, row & 1);
  SET_PIN(gpio, PIN_B, (row >> 1) & 1);
  SET_PIN(gpio, PIN_C, (row >> 2) & 1);
  SET_PIN(gpio, PIN_D, (row >> 3) & 1);

  // commit the configuration to the GPIO output
  GPIO.out = gpio;
}

void DumbMatrix::advance_clock() const {
  GPIO.out_w1ts = (uint32_t(1) << PIN_CLK);
  GPIO.out_w1tc = (uint32_t(1) << PIN_CLK);
}

void DumbMatrix::latch_data() const {
  SET_PIN(GPIO.out, PIN_LAT, HIGH);
  SET_PIN(GPIO.out, PIN_LAT, LOW);
}

void DumbMatrix::paint_row(const TSize row) const {
  gpio_select_row(row);

  for (TSize column = 0; column < _columns; ++column) {
    paint_column(row, column);
    advance_clock();
  }

  disable_display();
  latch_data();
  enable_display();
}

void DumbMatrix::paint_column(const TSize row, const TSize column) const {
  assert(row < (_rows / 2));

  // make a copy of the GPIO state
  uint32_t gpio = GPIO.out;

  // Set the colours
  SET_PIN(gpio, PIN_R1, _pixels[row][column].r != 0);
  SET_PIN(gpio, PIN_G1, _pixels[row][column].g != 0);
  SET_PIN(gpio, PIN_B1, _pixels[row][column].b != 0);

  SET_PIN(gpio, PIN_R2, _pixels[row + 16][column].r != 0);
  SET_PIN(gpio, PIN_G2, _pixels[row + 16][column].g != 0);
  SET_PIN(gpio, PIN_B2, _pixels[row + 16][column].b != 0);

  // commit the configuration to the GPIO output
  GPIO.out = gpio;
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
