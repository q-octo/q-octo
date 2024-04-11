// Copyright (c) 2020-2022 Cesanta Software Limited
// All rights reserved

//#include "pico/cyw43_arch.h"
//#include "dhcpserver.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "lwip/ip4_addr.h"

#include "FreeRTOS.h"
#include "task.h"
#include "mongoose.h"
#include "net.h"

#include "hardware/uart.h"

// Tasks
#include "task_data_manager.h"
#include "task_web_server.h"


//#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )

#define PRODUCER_TASK_PRIORITY			1
#define DATA_MANAGER_TASK_PRIORITY		2
#define CONSUMER_TASK_PRIORITY			3

#define TEST_TASK_STACK_SIZE			(( configSTACK_DEPTH_TYPE ) 2048)

// UART configuration
#define UART_ID uart1
#define BAUD_RATE 9600
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define MAX_STRING_SIZE 100 // Adjust based on your needs


//static struct mg_mgr mgr;

void send_ack() {
    //static void uardt_write_blocking (uart_inst_t *uart, const uint8_t *src, size_t len)
    uart_write_blocking(UART_ID, (const uint8_t *)"ACK\n", 4);
    printf("Companion ACK\n");
}

void uart_receive_task(void *pvParameters) {
    // Task to receive UART messages
    char receivedChar;
    char buffer[MAX_STRING_SIZE];
    int bufferIndex = 0;

    while (1) {
        if (uart_is_readable(UART_ID)) {
            receivedChar = uart_getc(UART_ID);
            // Check for newline character to indicate end of string
            if (receivedChar == '\n') {
                // Null-terminate the string
                buffer[bufferIndex] = '\0';
                // Send to queue
                struct TaskMessage message;
                strcpy(message.Message, buffer);
                xQueueSend(dataManagerQueue, &message, 0);

                // Reset buffer index for the next string
                // Sleep
                vTaskDelay(pdMS_TO_TICKS(1000)); // Sleep for 1 second
                send_ack();
                bufferIndex = 0;
            } else if (bufferIndex < MAX_STRING_SIZE - 1) {
                // Add character to buffer if there's space
                buffer[bufferIndex++] = receivedChar;
            } else {
                // Buffer is full, handle overflow or reset buffer
                bufferIndex = 0; // Reset buffer index
            }
        }
    }
}

void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(taskDataManager, "taskDataManager", 2048, NULL, DATA_MANAGER_TASK_PRIORITY, &task);
    xTaskCreate(uart_receive_task, "uart_receive_task", 2048, NULL, PRODUCER_TASK_PRIORITY, &task);
    xTaskCreate(taskWebServer, "task_web_server", 2048, NULL, CONSUMER_TASK_PRIORITY, &task);
    vTaskStartScheduler();
}

int main( void )
{  
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    vLaunch();
}
