#include <Arduino.h>

#include <FreeRTOS.h> // Enables FreeRTOS and multicore support
#include <task.h>     // Enables FreeRTOS tasks

#include "can.h"
#include "xiaomi_cybergear_driver.h"
#include "pico/stdlib.h"
#include "task_crsf.h"
#include "task_display.h"
#include "task_motors.h"

void onReceiveCanPacket(int packetSize, uint32_t packetId, uint8_t *packetData, bool extended);
void taskDebug(void *pvParameters);
void taskCAN(void *pvParameters);

void setup()
{
  Serial.begin(115200); // initialize serial communication
  while (!Serial)
    ;          // Wait for serial connection to be established
  delay(1000); // Wait for a second
  Serial.println("Live on core 0");
  CanCommunication::init(onReceiveCanPacket);

  xTaskCreate(taskCRSF, "taskCRSF", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(taskDisplay, "taskDisplay", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(taskCAN, "taskCAN", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(taskMotors, "taskMotors", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

void loop()
{
  // Handled by FreeRTOS
}

void taskCAN(void *pvParameters)
{
  (void)pvParameters; //  To avoid warnings
  for (;;)
  {
    CanCommunication::checkForPacket();
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

void onReceiveCanPacket(int packetSize, uint32_t packetId, uint8_t *packetData,
                        bool extended)
{
  Serial.print("main.cpp: Received packet with id 0x");
  Serial.print(packetId, HEX);

  switch ((packetId & 0xFF00) >> 8)
  {
  case CYBERGEAR_CAN_ID_L:
    cybergearL.process_message(packetData);
    break;
  case CYBERGEAR_CAN_ID_R:
    cybergearR.process_message(packetData);
    break;
  default:
    Serial.println("main.cpp: Received packet from unknown device");
    break;
  }
}
