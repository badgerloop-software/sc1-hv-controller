#ifndef __ANALOG_H__
#define __ANALOG_H__

#include "mbed.h"
#include "telemetry.h"
#include "mutexless_analog.h"

#define IS_CONST 18 // 22700 / 1200 multiplier from hardware 

// read all the analog inputs
void readAnalog();

// repeatedly call readAnalog() with the input time interval
void initAnalog(std::chrono::microseconds readSignalPeriod);

// display analog data for testing
void displayAnalog();

#endif