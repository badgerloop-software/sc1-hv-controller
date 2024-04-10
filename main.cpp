#include "mbed.h"
#include "digital.h"
#include "analog.h"
#include "hv_can.h"

#define DEBUG_PRINT 1

// Analog/digital IO polling rates
#define DIGITAL_POLLING_RATE 50ms
#define ANALOG_POLLING_RATE 50ms
#define TELEMETRY_SEND_RATE 50ms

// CAN Pins
#define rdPin PA_11
#define tdPin PA_12


int main()
{
#if DEBUG_PRINT
    BufferedSerial serial(USBTX, USBRX, 115200);
    int counter = 0;
#endif

    // Enable current sensing
    set_SUPP_DEG(1);
    set_DCDC_DEG(1);

    // Initialize IO updating
    initDigital(DIGITAL_POLLING_RATE);
    initAnalog(ANALOG_POLLING_RATE);

    // Initialize HV CAN Manager
    HVCANManager hvCANManager(rdPin, tdPin);

    while (1) {
        // Periodically send all telemetry over CAN
        hvCANManager.sendTelemetry();
        hvCANManager.runQueue(TELEMETRY_SEND_RATE);

#if DEBUG_PRINT
        // Display digital and analog values every second (for testing) 
        if (counter >= (1000 / TELEMETRY_SEND_RATE.count())) {
            printf("\e[1;1H\e[2J");
            displayDigital();
            displayAnalog();
            counter = 0;
        }

        // Increment the display counter
        counter ++;
#endif
    }
}
