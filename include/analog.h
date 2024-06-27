#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "mbed.h"
#include "telemetry.h"
#include "mutexless_analog.h"

#define IS_CONST 18 // 22700 / 1200 multiplier from hardware 
#define MIN_PACK_TEMP 36 // pack temp thresholds used for controlling fans
#define MAX_PACK_TEMP 52.5

// read all the analog inputs
void readAnalog();

// repeatedly call readAnalog() with the input time interval
void initAnalog(std::chrono::microseconds readSignalPeriod);

// display analog data for testing
void displayAnalog();

#endif