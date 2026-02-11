#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bm_gpio_s3.h"

#define BLINK_GPIO  (2u)
#define DELAY_MS    (500u)

void app_main()
{
    bm_gpio_set_mode(BLINK_GPIO, BM_GPIO_MODE_OUTPUT);

    while (1)
    {
        bm_gpio_toggle(BLINK_GPIO);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
    
}
