#include <cmath>
#include "ds3502u.h"
#include "i2cWrapper.h"

#define WIPER_REGISTER 0x00
#define CONTROL_REGISTER 0x02

Ds3502u::Ds3502u(uint8_t addr, I2C *i2cBus, float maxVoltage) {
    this->addr = addr;
    this->i2cBus = i2cBus;
    this->maxVoltage = maxVoltage;
} 

/* Sets the 2 pins of the nucleo connected to the Address pins of the potentiometer to be input pins
 * Reads the default startup value of the resistance and verifies it
 * This method is technically unnecessary, especially if the pins on the nucleo don't need to be set to 
 * be input pins 
 */
int Ds3502u::begin() {
    // set the 2 pins of the nucleo connected to the Address pins of the potentiometer to be input pins
    DigitalInOut addressPin1(PB_1);
    DigitalInOut addressPin2(PA_8);
    addressPin1.input();
    addressPin2.input();
    // check the value of the default startup resistance of the potentiometer 
    if(this->readResistance() != this->maxVoltage * 0x40/127) {
        return 1; // error reading
    }
    return 0; // no errors
}

/* @param outputVoltage the voltage the potentiometer should output
 * @return 1 if error (outputVoltage isn't within limits, i2c write failed)
 * @return 0 if no error
 */
int Ds3502u::setResistance(float outputVoltage) {
    if (outputVoltage < 0 || outputVoltage > maxVoltage) {
        return 1;
    }
    uint8_t writeAddress = this->addr & 0xFE; // set the last bit to 0 for writes
    int wiperResistance = (int)round(outputVoltage / this->maxVoltage * 127);
    char buffer[1] = {(char)wiperResistance};
    if(writeI2CWrapper(this->i2cBus, writeAddress, WIPER_REGISTER, buffer, 1)) {
        return 1; // error with write
    }
    return 0; // no errors
}

/* @return the voltage across the potentiometer based on what its resistance is set to and the max voltage. 
 * @return -1.0 if there was an error. 
 */
float Ds3502u::readResistance() {
    uint8_t writeAddress = this->addr & 0xFE; // set the last bit to 0 for writes
    uint8_t readAddress = this->addr | 0x01; // set the last bit to 1 for reads
    char buffer[1];
    // don't use the readI2CWrapper() method because that doesn't account for the
    // different addresses for read and write on the potentiometer
    if(writeI2CWrapper(this->i2cBus, writeAddress, WIPER_REGISTER, buffer, 0)) {
        return -1.0;
    }
    if(i2cBus->read(readAddress, buffer, 1)) {
        return -1.0;
    }
    int wiperResistance = (int)buffer[0];
    return (float)wiperResistance/127 * this->maxVoltage;
}