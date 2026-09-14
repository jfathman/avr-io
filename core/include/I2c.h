// I2c.h

#pragma once

#define I2C_ADDRESS_MIN 0
#define I2C_ADDRESS_MAX 0x7f

#define I2C_RESERVED_BELOW 0x08
#define I2C_RESERVED_ABOVE 0x77

#define I2C_READ  1
#define I2C_WRITE 0

#define I2C_SLA_W(address) ((address << 1) | I2C_WRITE)
#define I2C_SLA_R(address) ((address << 1) | I2C_READ)

class I2c
{
  public:
    void init();
    bool start();
    bool stop();
    bool write(uint8_t address, uint8_t data);
    bool writeAddress(uint8_t address);
    bool writeData(uint8_t data);
    bool detect(uint8_t address);
  private:
    bool _waitCompleteStart();
    bool _waitCompleteWrite();
    bool _waitCompleteStop();
    template <typename Func>
    bool _waitCondition(uint16_t timeout_usec, Func condition);
};
