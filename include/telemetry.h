#ifndef __TELEMETRY_H__
#define __TELEMETRY_H__

#include "mbed.h"


// See spreadsheet for signal descriptions
// https://docs.google.com/spreadsheets/d/12O2UPdM_fqUVKd0IXZ638wb1obtBXQp11giv7swfWM4

struct Digital_Data {
    bool driver_EStop : 1;
    bool external_EStop : 1;
    bool start_shutdown_status : 1;
    bool isolation_status : 1;
    bool battery_discharge_enabled : 1;
    bool battery_discharge_enable : 1;
    bool battery_charge_enabled : 1;
    bool battery_charge_enable : 1;
    bool BMS_MPO1 : 1;
    bool lv_contactor : 1;
    bool mc_contactor : 1;
    bool mppt_contactor : 1;
    bool crash_sensor : 1;
    bool use_supp : 1;
    bool use_dcdc : 1;
    bool mcu_hv_en : 1;
};

// variable for the digital data struct
extern volatile struct Digital_Data digital_data;

// AnalogIn values
extern volatile float DCDC_current_value;
extern volatile float supplemental_current_value;
extern volatile float supplemental_voltage_value;

// Restart Enable signal from Driver Dashboard to turn on car
extern volatile bool startup_signal;

// Pack temp from BMS
extern volatile float pack_temp;

#endif