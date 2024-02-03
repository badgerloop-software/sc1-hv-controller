#include "hv_can.h"

// Restart Enable signal from Driver Dashboard to turn on car
volatile bool startup_signal= false;

/*
    Initialize HVCANManager with super call to CANManager
*/
HVCANManager::HVCANManager(PinName rd, PinName td, int frequency) : CANManager(rd, td, frequency) {}

/*
    Handle reading of input message from CAN
*/
void HVCANManager::readHandler(int messageID, SharedPtr<unsigned char> data, int length) {
    switch (messageID) {
        case 0x100:
            startup_signal = *data;
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