/*
    Code produced for the Q-Octo project.

    Copyright (c) 2024 Britannio Jarrett, <oss@britannio.dev>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
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
