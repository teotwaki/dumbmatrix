#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dumbmatrix.hpp"

dm::DumbMatrix matrix;

void setup() {
  // put your setup code here, to run once:
  matrix.set({0, 0}, {0, 1, 0});
  matrix.set({1, 1}, {0, 1, 0});
  matrix.set({2, 2}, {0, 1, 0});
  matrix.set({3, 3}, {0, 1, 0});
}

void loop() {
  // put your main code here, to run repeatedly:
  while (true) {
    matrix.update();
  }
}
