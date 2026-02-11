#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define GPIO_BASE 0x60004000
#define GPIO_OUT_W1TS_REG (GPIO_BASE + 0x08)
#define GPIO_OUT_W1TC_REG (GPIO_BASE + 0x0C)
#define GPIO_ENABLE_REG (GPIO_BASE + 0x20)
/*                       GPIO base address  +       */
#define GPIO2_MASK (1U << 2)
#define DELAY_MS 500

void app_main()
{
    volatile uint32_t* gpio_out_w1ts_reg = (volatile uint32_t*) GPIO_OUT_W1TS_REG;
    volatile uint32_t* gpio_out_w1tc_reg = (volatile uint32_t*) GPIO_OUT_W1TC_REG;
    volatile uint32_t* gpio_enable_reg = (volatile uint32_t*) GPIO_ENABLE_REG;

    *gpio_enable_reg |= GPIO2_MASK;

    while (1)
    {
        *gpio_out_w1ts_reg = GPIO2_MASK;    //set high
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
        *gpio_out_w1tc_reg = GPIO2_MASK;    //set low
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
    
    
}