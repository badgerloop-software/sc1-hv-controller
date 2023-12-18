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
    bool battery_charge_enabled : 1;
    bool BMS_MPO1 : 1;
    bool lv_contactor : 1;
    bool mc_contactor : 1;
    bool mppt_contactor : 1;
    bool crash_sensor : 1;
};

// AnalogIn values
extern volatile float DCDC_current_value;
extern volatile float supplemental_current_value;
extern volatile float supplemental_voltage_value;


#endif