#include "flight_controller.h"
#include "config.h"
#include "task_power_monitor.h"

void FlightController::init()
{
    crsf_init(&flConCrsfIns);
    crsf_set_link_quality_threshold(&flConCrsfIns, 0);
    crsf_set_rssi_threshold(&flConCrsfIns, 130);
    crsf_set_on_battery(&flConCrsfIns, onBatteryUpdate);

    // Using this instead of crsf_begin as we will handle the UART ourself
    fcSerial.begin(420000);
}

void FlightController::loop()
{
    int bytesAvailable = fcSerial.available();
    for (int i = 0; i < bytesAvailable; i++)
    {
        crsf_process_frame(&flConCrsfIns, &crsfFrameIndex, &crsfFrameLength,
                           &crsfCrcIndex, fcSerial.read());
    }
}

void FlightController::onBatteryUpdate(const crsf_payload_battery_sensor_t battery) {
    Serial.printf("Battery voltage: %d\n", battery.voltage);
    Serial.printf("Battery current: %d\n", battery.current);
    Serial.printf("Battery capacity: %d\n", battery.capacity);
    Serial.printf("Battery percent: %d\n", battery.percent);
    TaskPowerMonitor::Message powerMonitorMessage = {
        .type = TaskPowerMonitor::MessageType::FLIGHT_CON_MESSAGE,
        .as = { .crsf_battery = battery }
    };
    TaskPowerMonitor::receiveMessage(powerMonitorMessage);
}
