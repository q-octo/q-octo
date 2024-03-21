# include "blink.h"
# include "pico/cyw43_arch.h"

int count = 0; // This is a global variable

int get_count() {
    return count;
}

void blink() {
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));

        count++;
    }
}
