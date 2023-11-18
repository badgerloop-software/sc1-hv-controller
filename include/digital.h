#ifndef __DIGITAL_H__
#define __DIGITAL_H__

#include "mbed.h"
#include "telemetry.h"

// read all the digital inputs
void readDigital();

// repeatedly call readDigital() with the input time interval
void initDigital(std::chrono::microseconds readSignalPeriod);

void set_MCU_HV_EN(int value);
void set_BMS_DSCHRG_EN(int value);
void set_BMS_CHRG_EN(int value);
void set_SUPP_DEG(int value);
void set_DCDC_DEG(int value);
void set_USE_SUPP(int value);
void set_USE_DCDC(int value);

#endif