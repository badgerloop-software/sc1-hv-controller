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
DigitalIn BMS_DSRCHG_EN(PB_1);
DigitalIn BMS_CHRG_EN(PB_15);
DigitalIn USE_SUPP(PB_3);
DigitalIn USE_DCDC(PB_4);

// Outputs
DigitalOut MCU_HV_EN(PB_7);
DigitalOut SUPP_DEG(PC_2);
DigitalOut DCDC_DEG(PC_3);

// store input digital data inside a struct to reduce the memory
volatile struct Digital_Data digital_data = {};

void read_MCU_Stat_Fdbck() {
    digital_data.start_shutdown_status = MCU_Stat_Fdbck.read();
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

void readDigital() {
    // Startup/Shutdown
    // Read signals in same order as present on relay
    // Enable software HV EN if all signals nominal (high)
    digital_data.driver_EStop = Driver_Estop_Fdbck.read();
    digital_data.external_EStop = Ext_Estop_Fdbck.read();
    digital_data.crash_sensor = Inertia_Fdbck.read();
    digital_data.battery_discharge_enable = BMS_DSRCHG_EN.read();
    digital_data.battery_discharge_enabled = DSCHRG_EN_Fdbck.read();
    digital_data.battery_charge_enable = BMS_CHRG_EN.read();
    digital_data.battery_charge_enabled = CHRG_EN_Fdbck.read();
    digital_data.BMS_MPO1 = BMS_MPO1.read();
    digital_data.isolation_status = Isolation_Fdbck.read();
    digital_data.use_supp = USE_SUPP.read();
    digital_data.use_dcdc = USE_DCDC.read();

    if (digital_data.driver_EStop && 
        digital_data.external_EStop && 
        digital_data.crash_sensor && 
        digital_data.battery_discharge_enabled && 
        digital_data.battery_charge_enabled && 
        digital_data.isolation_status && 
        startup_signal) {
        set_MCU_HV_EN(1);
    } else {
        set_MCU_HV_EN(0);
    }

    // Remaining signals
    read_MCU_Stat_Fdbck();
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
void set_SUPP_DEG(int value) {
    SUPP_DEG.write(value);
}
void set_DCDC_DEG(int value) {
    DCDC_DEG.write(value);
}

// Display digial data for testing
void displayDigital() {
    printf("Digital Input Signals:\n");
    printf("BMS_CHRG_EN: %d\n", digital_data.battery_charge_enable);
    printf("CHRG_EN_Fdbck: %d\n", digital_data.battery_charge_enabled);
    printf("BMS_DSCHRG_EN: %d\n", digital_data.battery_discharge_enable);
    printf("DSCHRG_EN_Fdbck: %d\n", digital_data.battery_discharge_enabled);
    printf("BMS_MPO1: %d\n", digital_data.BMS_MPO1);
    printf("Inertia_Fdbck: %d\n", digital_data.crash_sensor);
    printf("Driver_Estop_Fdbck: %d\n", digital_data.driver_EStop);
    printf("Ext_Estop_Fdbck: %d\n", digital_data.external_EStop);
    printf("Isolation_Fdbck: %d\n", digital_data.isolation_status);
    printf("LO_CONT_TELEM: %d\n", digital_data.lv_contactor);
    printf("MC_CONT_TELEM: %d\n", digital_data.mc_contactor);
    printf("MPPT_CONT_TELEM: %d\n", digital_data.mppt_contactor);
    printf("MCU_Stat_Fdbck: %d\n", digital_data.start_shutdown_status);
    printf("USE_SUPP: %d\n", digital_data.use_supp);
    printf("USE_DCDC: %d\n", digital_data.use_dcdc);
    printf("\n");

    printf("Digital Output Signals:\n");
    printf("DCDC_DEG: %d\n", DCDC_DEG.read());
    printf("SUPP_DEG: %d\n", SUPP_DEG.read());
    printf("MCU_HV_EN: %d\n", MCU_HV_EN.read());
    printf("\n");
}