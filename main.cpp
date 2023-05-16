#include "mbed.h"
#include "ds3502u.h"
#include "i2cWrapper.h"

// Pot will be set to (BMS Voltage / 10) - SAFETY_OFFSET
#define SAFETY_OFFSET 0.3 
#define POT_ADDR 0x56
#define MAX_VOLTAGE 12.0
#define CAN_FREQ 500000

I2C i2cBus(PB_7, PB_6);
CAN canBus(PA_11, PA_12, CAN_FREQ);

int main()
{  
    // Initialize bus
    i2cBus.frequency(100000);
    i2cdetect(&i2cBus);

    // code to read and write to potentiometer
    Ds3502u hvPotentiometer(POT_ADDR, &i2cBus, MAX_VOLTAGE);
    printf("starting potentiometer\n");
    if (hvPotentiometer.begin()) {
        printf("error with reading Wiper Register initial value of 0x40\n");
    } else {
        printf("begin() worked\n");
    }


    // Update potentiometer when new battery reading received
    CANMessage canMsg;
    float bmsValue = 0;
    float refVoltage;
    while(1) {
        // Check if CAN message received contains BMS voltage
        // If yes, extract and update Pot value
        // @see https://badgerloop.atlassian.net/wiki/spaces/EL/pages/260505924/CAN+Signal+ID+List
        if(canBus.read(canMsg) && canMsg.id == 0x101) {
            bmsValue = (float)(canMsg.data[3] | canMsg.data[2] << 8) / 10;
            printf("Got voltage: %f\n", bmsValue);
            refVoltage = 0.1 * bmsValue - SAFETY_OFFSET;

            if(hvPotentiometer.setResistance(refVoltage)) {
                printf("Write failed\n");
            }
            wait_us(10000);
            printf("output voltage: %f\n", hvPotentiometer.readResistance());
            wait_us(10000);
        }

        // Clear CAN fault if present
        if (canBus.tderror() || canBus.rderror()) {
            canBus.reset();
            wait_us(1000);
        }
    }
}
