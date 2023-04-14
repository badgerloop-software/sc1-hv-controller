#include "i2cWrapper.h"

/* Reads bytes from the specified register at the specified address
 * It does this by first writing the register offset to the address of the device, then
 * reading the number of bytes from that address. 
 * 
 * return 0 if no errors, 1 if there were errors. 
 */
int readI2CWrapper(I2C *i2cBus, int addr, uint8_t reg, char buffer[], int numBytes) {
    char writeBuffer[1] = {reg};
    // check if the I2C write and read throw errors. 
    if (i2cBus->write(addr, writeBuffer, 1)) {
        return 1; 
    }
    if (i2cBus->read(addr, buffer, numBytes)) {
        return 1; 
    }
    return 0; // no errors
}

/* Writes bytes to the specified register at the specified address
 * Does this by prepending the register offset to the buffer, then writing that 
 * to the address of the device. 
 * 
 * returns 0 if no errors, 1 if there were errors
 */
int writeI2CWrapper(I2C *i2cBus, int addr, uint8_t reg, char buffer[], int numBytes) {
    // create a new buffer which is just the input buffer with the register offset prepended.
    char newBuffer[numBytes + 1];
    newBuffer[0] = reg;
    for (int i = 1; i <= numBytes; i++) {
        newBuffer[i] = buffer[i-1];
    }
    // check if the I2C write throws an error 
    if(i2cBus->write(addr, newBuffer, numBytes + 1)) {
        return 1;
    }
    return 0; // no errors 
}

/* Prints the addresses of the I2C devices to console
 * This code was taken from 
 * https://os.mbed.com/users/MACRUM/code/i2cdetect//file/0ca27c666888/main.cpp/
 */
void i2cdetect(I2C *i2cBus) {
    char buf[10];
    buf[0] = 0;
    printf("\n     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
    for (int adrs = 0; adrs < 0x80; adrs += 16) {
        printf("%02x: ", adrs);
        for (int i = 0; i < 16; i++) {
            i2cBus->write(((adrs + i) << 1), buf, 1);
            int ret = i2cBus->read(((adrs + i) << 1), buf, 2);
            if ((adrs+i) == 0 || (adrs+i) == 1 || (adrs+i) == 2) {
                printf("   ");
                continue;
            }
            if ((adrs+i) >= 0x78) {
                printf("\n");
                break;
            }
            if (ret == 1) {
                printf("-- ");
            }
            if (ret == 0) {
                printf("%02x ", (adrs + i) << 1);
            }
        }
        printf("\n");
    }
}