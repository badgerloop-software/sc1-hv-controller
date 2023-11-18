#include "analog.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the function at a specified rate
Ticker readAnalogDelay;

AnalogInMutexless DCDC_IS(PC_0);
AnalogInMutexless SUPP_IS(PC_1);
AnalogInMutexless SENSE_VSUPP(PC_5);

volatile float DCDC_current_value;
volatile float supplemental_current_value;
volatile float supplemental_voltage_value;

void read_DCDC_IS() {
    DCDC_current_value = DCDC_IS.read_voltage();
}
void read_SUPP_IS() {
    supplemental_current_value = SUPP_IS.read_voltage();
}
void read_SENSE_VSUPP() {
    supplemental_voltage_value = SENSE_VSUPP.read_voltage();
}

// read all analog input
void readAnalog(){
    read_DCDC_IS();
    read_SUPP_IS();
    read_SENSE_VSUPP();
}

// Set up polling of analog IO at specified rate
void initAnalog(std::chrono::microseconds readSignalPeriod) {
    readAnalogDelay.attach(readAnalog, readSignalPeriod);
}
