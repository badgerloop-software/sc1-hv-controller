#include "hv_can.h"

/*
    Initialize HVCANManager with super call to CANManager
*/
HVCANManager::HVCANManager(PinName rd, PinName td, int frequency) : CANManager(rd, td, frequency) {}

/*
    Handle reading of input message from CAN
*/
void HVCANManager::readHandler(int messageID, SharedPtr<unsigned char> data, int length) {
    
}

/*
    Send all telemetry over CAN
*/
void HVCANManager::sendTelemetry() {

    // Digital data
    CANManager::sendMessage(300, (void*)&digital_data, sizeof(digital_data));

    // Analog values
    CANManager::sendMessage(301, (void*)&DCDC_current_value, sizeof(DCDC_current_value));
    CANManager::sendMessage(302, (void*)&supplemental_current_value, sizeof(supplemental_current_value));
    CANManager::sendMessage(303, (void*)&supplemental_voltage_value, sizeof(supplemental_voltage_value));
}