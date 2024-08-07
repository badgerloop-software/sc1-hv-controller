#include "hv_can.h"

// Restart Enable signal from Driver Dashboard to turn on car
volatile bool startup_signal= false;
volatile float pack_temp = 0.0;

/*
    Initialize HVCANManager with super call to CANManager
*/
HVCANManager::HVCANManager(PinName rd, PinName td, int frequency) : CANManager(rd, td, frequency) {}

/*
    Handle reading of input message from CAN
*/
void HVCANManager::readHandler(int messageID, SharedPtr<unsigned char> data, int length) {
    unsigned char *data_ptr;
    switch (messageID) {
        case 0x025:
            startup_signal = *data;
            break;
        case 0x102:
            data_ptr = data.get();
            pack_temp = data_ptr[5];
            break;
        default:
            break;
    }
}

/*
    Send all telemetry over CAN
*/
void HVCANManager::sendTelemetry() {

    // Digital data
    CANManager::sendMessage(0x300, (void*)&digital_data, sizeof(digital_data));

    // Analog values
    CANManager::sendMessage(0x301, (void*)&DCDC_current_value, sizeof(DCDC_current_value));
    CANManager::sendMessage(0x302, (void*)&supplemental_current_value, sizeof(supplemental_current_value));
    CANManager::sendMessage(0x303, (void*)&supplemental_voltage_value, sizeof(supplemental_voltage_value));
}