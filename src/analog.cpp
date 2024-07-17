#include "analog.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the function at a specified rate
Ticker readAnalogDelay;

AnalogInMutexless DCDC_IS(PC_0);
AnalogInMutexless SUPP_IS(PC_1);
AnalogInMutexless SENSE_VSUPP(PC_5);

PwmOut BATTERY_FAN(PA_4);
PwmOut HV_FAN(PA_1);

volatile float fan_pwm;

volatile float DCDC_current_value;
volatile float supplemental_current_value;
volatile float supplemental_voltage_value;

void read_DCDC_IS() {
    DCDC_current_value = DCDC_IS.read_voltage() * IS_CONST;
}
void read_SUPP_IS() {
    supplemental_current_value = SUPP_IS.read_voltage() * IS_CONST;
}
void read_SENSE_VSUPP() {
    supplemental_voltage_value = SENSE_VSUPP.read_voltage();
}

void set_fan_PWM() {
    // Fan on if battery in BPS fault or over termpature threshold
    if (digital_data.BMS_MPO1 || digital_data.battery_charge_enable || digital_data.battery_discharge_enable ||
        pack_temp >= PACK_TEMP_THRESHOLD) {
        BATTERY_FAN.write(1);
        HV_FAN.write(1);
    } else {
        BATTERY_FAN.write(0);
        HV_FAN.write(0);
    }
}

// read all analog input
void readAnalog(){
    read_DCDC_IS();
    read_SUPP_IS();
    read_SENSE_VSUPP();
    //set_fan_PWM();
}

// Set up polling of analog IO at specified rate
void initAnalog(std::chrono::microseconds readSignalPeriod) {
    DCDC_IS.set_reference_voltage(3.3);
    SUPP_IS.set_reference_voltage(3.3);
    SENSE_VSUPP.set_reference_voltage(3.3);
    readAnalogDelay.attach(readAnalog, readSignalPeriod);

    // Always run fans at full speed for comp
    BATTERY_FAN.write(1);
    HV_FAN.write(1);
}

// Display analog data for testing
void displayAnalog() {
    printf("Analog Input Signals:\n");
    printf("DCDC_IS: %f\n", DCDC_current_value);
    printf("SUPP_IS: %f\n", supplemental_current_value);
    printf("SENSE_VSUPP: %f\n", supplemental_voltage_value);
    printf("Fan PWM: %f\n", fan_pwm);
    printf("Pack Temp: %f\n", pack_temp);
    printf("\n");
}