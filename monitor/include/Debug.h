// Debug.h

#pragma once

class Debug
{
  public:
    Debug()
      : _enabled(false)
    { }

    void enable()
    {
        _enabled = true;
    }

    void disable()
    {
        _enabled = false;
    }

    bool isEnabled()
    {
        return _enabled;
    }

  private:
    bool _enabled;
};
