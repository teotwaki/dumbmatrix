#ifndef MATRIX_CONFIG_HPP
#define MATRIX_CONFIG_HPP

#define ESP32_BREADBOARD 0

#define GPIO_PINOUT ESP32_BREADBOARD

#if (GPIO_PINOUT == ESP32_BREADBOARD)

#define PIN_R1 2
#define PIN_B1 15
#define PIN_G1 4

#define PIN_R2 16
#define PIN_B2 27
#define PIN_G2 17

#define PIN_A 5
#define PIN_B 18
#define PIN_C 19
#define PIN_D 21

#define PIN_LAT 26
#define PIN_OE 25
#define PIN_CLK 22

#endif // GPIO_PINOUT == ESP32_BREADBOARD

#endif
