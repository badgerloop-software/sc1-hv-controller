#include "mbed.h"


/* Class representing DS350U digital potentiometer
 */
class Ds3502u {
    private:
        uint8_t addr;
        I2C *i2cBus;
        float maxVoltage;
    public:
        // Constructor takes 8-bit I2C address
        Ds3502u(uint8_t addr, I2C *i2cBus, float maxVoltage);
        int begin();
        int setResistance(float outputVoltage);
        float readResistance();
};