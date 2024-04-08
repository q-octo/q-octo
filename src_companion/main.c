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


#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define TEST_TASK_STACK_SIZE			(( configSTACK_DEPTH_TYPE ) 2048)

// UART configuration
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define MAX_STRING_SIZE 100 // Adjust based on your needs


static struct mg_mgr mgr;

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
                // Process the received string
                printf("Received string: %s\n", buffer);
                // Reset buffer index for the next string
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
    xTaskCreate(uart_receive_task, "uart_receive_task", 2048, NULL, TEST_TASK_PRIORITY, &task);
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
