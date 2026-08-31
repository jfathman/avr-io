// Digital.h

#pragma once

#include <stdint.h>
#include "pinDefs.h"

#define OUTPUT       0
#define INPUT        1
#define INPUT_PULLUP 2

#define ACTUAL    0
#define COMMANDED 1

class Digital
{
  public:
    void mode(uint8_t pin, uint8_t mode);
    void set(uint8_t pin);
    void clear(uint8_t pin);
    void toggle(uint8_t pin, uint8_t option = ACTUAL);
    void on(uint8_t pin);
    void off(uint8_t pin);
    uint8_t read(uint8_t pin, uint8_t option = ACTUAL);
    void pwmWrite(uint8_t pin, uint8_t dutyCycle);
    uint8_t pwmRead(uint8_t pin);
};
