#include "mbed.h"

class Ds3502u {
    private:
        uint8_t addr;
        I2C *i2cBus;
        float maxVoltage;
    public:
        Ds3502u(uint8_t addr, I2C *i2cBus, float maxVoltage);
        int begin();
        int setResistance(float outputVoltage);
        float readResistance();
};