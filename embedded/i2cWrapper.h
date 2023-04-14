#include "mbed.h"
int readI2CWrapper(I2C *i2cBus, int addr, uint8_t reg, char buffer[], int numBytes);

int writeI2CWrapper(I2C *i2cBus, int addr, uint8_t reg, char buffer[], int numBytes);

void i2cdetect(I2C *i2cBus);