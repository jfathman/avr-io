// Serial.h

#pragma once

#include <stdint.h>

class Serial
{
  public:
    void init(uint32_t baudrate);
    bool available();
};
