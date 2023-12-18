#include "mbed.h"
#include "digital.h"
#include "analog.h"
#include "hv_can.h"

// Analog/digital IO polling rates
#define DIGITAL_POLLING_RATE 5ms
#define ANALOG_POLLING_RATE 5ms

// CAN Pins
#define rdPin D15
#define tdPin D14


int main()
{
    // Initialize IO updating
    initDigital(DIGITAL_POLLING_RATE);
    initAnalog(ANALOG_POLLING_RATE);

    // Initialize HV CAN Manager
    HVCANManager hvCANManager(rdPin, tdPin);

    while (1) {
        // Periodically send all telemetry over CAN
        hvCANManager.sendTelemetry();
        wait_us(1000000);
    }
}
