// Interval.h

#pragma once

#include <stdint.h>

#define ONE_SHOT 1
#define PERIODIC 2

class Timer;

class Interval
{
  public:
    Interval(Timer& timer);
    void start(uint8_t mode, uint32_t interval);
    bool isExpired();

  private:
    Timer& _timer;
    uint8_t _mode;
    uint32_t _interval;
    uint32_t _end;
};

