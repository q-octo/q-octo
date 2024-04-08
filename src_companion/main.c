// Copyright (c) 2020-2022 Cesanta Software Limited
// All rights reserved

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "lwip/ip4_addr.h"
#include "dhcpserver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "mongoose.h"
#include "net.h"

#include "blink.h"


#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define TEST_TASK_STACK_SIZE			(( configSTACK_DEPTH_TYPE ) 2048)

static struct mg_mgr mgr;

void main_task(__unused void *params) {
    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return;
    }

    cyw43_arch_enable_ap_mode("mysterious pico", "password", CYW43_AUTH_WPA2_AES_PSK);
    ip4_addr_t gw, mask;
    IP4_ADDR(&gw, 192, 168, 4, 1);
    IP4_ADDR(&mask, 255, 255, 255, 0);

    dhcp_server_t dhcp_server;
    dhcp_server_init(&dhcp_server, &gw, &mask);

    mg_mgr_init(&mgr);
    web_init(&mgr);

    while(true) {
        mg_mgr_poll(&mgr, 10);
    }

    cyw43_arch_deinit();
}

void blink_task(__unused void *params) {
    blink();
}

void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(main_task, "TestMainThread", TEST_TASK_STACK_SIZE, NULL, TEST_TASK_PRIORITY, &task);
    xTaskCreate(blink_task, "BlinkTask", TEST_TASK_STACK_SIZE, NULL, TEST_TASK_PRIORITY, &task);
    vTaskStartScheduler();
}

int main( void )
{
    stdio_init_all();
    vLaunch();

    return 0;
}
