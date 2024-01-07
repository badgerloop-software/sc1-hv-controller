#include "digital.h"

// initialize Ticker to setup a recurring interrupt to repeatedly call the
// function at a specified rate
Ticker readDigitalDelay;

// Inputs
DigitalIn Driver_Estop_Fdbck(PA_13);
DigitalIn Ext_Estop_Fdbck(PA_14);
DigitalIn MCU_Stat_Fdbck(PA_15);
DigitalIn Isolation_Fdbck(PC_12);
DigitalIn DSCHRG_EN_Fdbck(PC_14);
DigitalIn CHRG_EN_Fdbck(PC_15);
DigitalIn BMS_MPO1(PB_13);
DigitalIn LO_CONT_TELEM(PC_10);
DigitalIn MC_CONT_TELEM(PC_6);
DigitalIn MPPT_CONT_TELEM(PC_8);
DigitalIn Inertia_Fdbck(PB_2);

// Outputs
DigitalOut MCU_HV_EN(PB_7);
DigitalOut BMS_DSRCHG_EN(PB_1);
DigitalOut BMS_CHRG_EN(PB_15);
DigitalOut SUPP_DEG(PC_2);
DigitalOut DCDC_DEG(PC_3);
DigitalOut USE_SUPP(PB_3);
DigitalOut USE_DCDC(PB_4);

// store input digital data inside a struct to reduce the memory
volatile struct Digital_Data digital_data = {};

void read_Driver_Estop_Fdbck() {
    digital_data.driver_EStop = Driver_Estop_Fdbck.read();
}
void read_Ext_Estop_Fdbck() {
    digital_data.external_EStop = Ext_Estop_Fdbck.read();
}
void read_MCU_Stat_Fdbck() {
    digital_data.start_shutdown_status = MCU_Stat_Fdbck.read();
}
void read_Isolation_Fdbck() {
    digital_data.isolation_status = Isolation_Fdbck.read();
}
void read_DSCHRG_EN_Fdbck() {
    digital_data.battery_discharge_enabled = DSCHRG_EN_Fdbck.read();
}
void read_CHRG_EN_Fdbck() {
    digital_data.battery_charge_enabled = CHRG_EN_Fdbck.read();
}
void read_BMS_MPO1() {
    digital_data.BMS_MPO1 = BMS_MPO1.read();
}
void read_LO_CONT_TELEM() {
    digital_data.lv_contactor = LO_CONT_TELEM.read();
}
void read_MC_CONT_TELEM() {
    digital_data.mc_contactor = MC_CONT_TELEM.read();
}
void read_MPPT_CONT_TELEM() {
    digital_data.mppt_contactor = MPPT_CONT_TELEM.read();
}
void read_Inertia_Fdbck() {
    digital_data.crash_sensor = Inertia_Fdbck.read();
}

void readDigital() {
    // Startup/Shutdown
    // Read signals in same order as present on relay
    // Turn firmware controlled signals on one by one if all previous steps are nominal (high)
    read_Driver_Estop_Fdbck();
    read_Ext_Estop_Fdbck();
    read_Inertia_Fdbck();
    if (digital_data.driver_EStop && digital_data.external_EStop && digital_data.crash_sensor) {
        set_BMS_DSCHRG_EN(1);
    } else {
        set_BMS_DSCHRG_EN(0);
    }

    read_DSCHRG_EN_Fdbck();
    if (digital_data.battery_discharge_enabled) {
        set_BMS_CHRG_EN(1);
    } else {
        set_BMS_CHRG_EN(0);
    }

    read_CHRG_EN_Fdbck();
    read_Isolation_Fdbck();
    if (digital_data.battery_charge_enabled && digital_data.isolation_status && startup_signal) {
        set_MCU_HV_EN(1);
    } else {
        set_MCU_HV_EN(0);
    }

    // Remaining signals
    read_MCU_Stat_Fdbck();
    read_BMS_MPO1();
    read_LO_CONT_TELEM();
    read_MC_CONT_TELEM();
    read_MPPT_CONT_TELEM();
    
}

// Set up polling of digital IO at specified rate
void initDigital(std::chrono::microseconds readSignalPeriod) {
  readDigitalDelay.attach(readDigital, readSignalPeriod);
}

void set_MCU_HV_EN(int value) {
    MCU_HV_EN.write(value);
}
void set_BMS_DSCHRG_EN(int value) {
    BMS_DSRCHG_EN.write(value);
}
void set_BMS_CHRG_EN(int value) {
    BMS_CHRG_EN.write(value);
}
void set_SUPP_DEG(int value) {
    SUPP_DEG.write(value);
}
void set_DCDC_DEG(int value) {
    DCDC_DEG.write(value);
}
void set_USE_SUPP(int value) {
    USE_SUPP.write(value);
}
void set_USE_DCDC(int value) {
    USE_DCDC.write(value);
}