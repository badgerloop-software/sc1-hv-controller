#include "mbed.h"
#include "ds3502u.h"
#include "i2cWrapper.h"

#define POT_ADDR 0x56
#define MAX_VOLTAGE 12.0
#define SAFETY_OFFSET 0.3
#define BMS_VOLTAGE_ID 0x100

I2C i2cBus(PB_7, PB_6);
CAN canBus(PA_11, PA_12);

int main()
{  
    i2cBus.frequency(100000);
    i2cdetect(&i2cBus);

    // code to read and write to potentiometer
    Ds3502u testPotentiometer(POT_ADDR, &i2cBus, MAX_VOLTAGE);
    printf("starting potentiometer\n");
    if (testPotentiometer.begin()) {
        printf("error with reading Wiper Register initial value of 0x40\n");
    } else {
        printf("begin() worked\n");
    }


    // Update potentiometer when new battery reading received
    CANMessage canMsg;
    canBus.frequency(500000);
    while(1) {
        // Message of interest is bytes 2 and 3 of id 101
        if(canBus.read(canMsg) && canMsg.id == BMS_VOLTAGE_ID) {
            float bmsValue = (float)(canMsg.data[1] | canMsg.data[0] << 8) / 10;
            printf("Read %f\n", bmsValue);
            float refVoltage = 0.1 * bmsValue - SAFETY_OFFSET;


            if(testPotentiometer.setResistance(refVoltage)) {
                printf("write failed\n");
            }
            wait_us(1000000);
            printf("output voltage: %f\n", testPotentiometer.readResistance());
            wait_us(1000000);
        }


        // Clear CAN fault if present
        if (canBus.tderror()) {
            canBus.reset();
            wait_us(1000);
            canBus.frequency(500000);
        }
    }
}
