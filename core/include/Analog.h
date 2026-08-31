// Analog.h

#pragma once

#include <stdint.h>
#include "pinDefs.h"

#define VREF_VCC       1 // 328P 2560 4809
#define VREF_INTERNAL  2 // 328P 2560 4809
#define VREF_INT_0V55  3 // 4809
#define VREF_INT_1V1   4 // 2560 4809
#define VREF_INT_1V5   5 // 4809
#define VREF_INT_2V5   6 // 4809
#define VREF_INT_2V56  7 // 2560
#define VREF_INT_4V34  8 // 4809
#define VREF_EXTERNAL  9 // 328P 2560 4809

class Analog
{
  public:
    void init(uint8_t vref);
    uint16_t read(uint8_t channel);
  private:
    void _reference(uint8_t vref);
};
