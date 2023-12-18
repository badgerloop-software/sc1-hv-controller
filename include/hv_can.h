#ifndef _HV_CAN_H_
#define _HV_CAN_H_

#include "mbed.h"
#include "canmanager.h"
#include "telemetry.h"
#include "digital.h"

/*
    Class that extends the CANManager class for high voltage
*/
class HVCANManager : public CANManager {

    public:

        /*
            Constructor initializing the HVCANManager with super call to CANManager
            rd: CAN rd pin of the Nucleo to use
            td: CAN td pin of the Nucleo to use
            frequency: baud rate of CAN bus
        */
        HVCANManager(PinName rd, PinName td, int frequency = DEFAULT_CAN_FREQ);

        /*
            Handle reading of input message from CAN
            messageID: unique ID indicating data purpose according to signal spreadsheet
            data: pointer to message data
            length: size of data in bytes
        */
        void readHandler(int messageID, SharedPtr<unsigned char> data, int length);

        /*
            Send all telemetry over CAN
        */
        void sendTelemetry();
};


#endif