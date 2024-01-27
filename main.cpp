#include "mbed.h"
#include "digital.h"
#include "analog.h"
#include "hv_can.h"

// Analog/digital IO polling rates
#define DIGITAL_POLLING_RATE 25ms
#define ANALOG_POLLING_RATE 25ms
#define TELEMETRY_SEND_RATE 25ms

// CAN Pins
#define rdPin D15
#define tdPin D14

// Display Counter
int counter = 0;

int main()
{
    // Turn off unused switch outputs
    set_USE_SUPP(0);
    set_USE_DCDC(0);

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

        // Periodically display digital and analog values (for testing) and reset counter
        if (counter >= 50) {
            displayDigital();
            displayAnalog();
            counter = 0;
        }

        // Increment the display counter
        counter ++;
    }
}
