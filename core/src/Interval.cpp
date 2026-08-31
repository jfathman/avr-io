// Interval.cpp

#include <avr/io.h>
#include "coreError.h"
#include "Interval.h"
#include "Timer.h"

Interval::Interval(Timer& timer)
  : _timer(timer),
    _mode(0),
    _interval(0),
    _end(0)
{ }

void Interval::start(uint8_t mode, uint32_t interval)
{
    coreError = ERR_NONE;

    if (mode == ONE_SHOT || mode == PERIODIC)
    {
        _mode = mode;
        _interval = interval;
        _end = _timer.millis() + _interval;
    }
    else
    {
        coreError = ERR_MODE;
    }
}

bool Interval::isExpired()
{
    uint32_t now = _timer.millis();

    if (now > _end)
    {
        if (_mode == PERIODIC)
        {
            _end = now + _interval;
        }

        return true;
    }

    return false;
}
