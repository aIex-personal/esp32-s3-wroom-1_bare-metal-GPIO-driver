#pragma once

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
    BM_GPIO_MODE_INPUT = 0,
    BM_GPIO_MODE_OUTPUT = 1,
}bm_gpio_mode_t;

typedef enum 
{
    BM_GPIO_LEVEL_LOW = 0,
    BM_GPIO_LEVEL_HIGH = 1,
}bm_gpio_level_t;

/**
 * @brief Configure a gpio pin as input/output (0 - 31) only in this minimal driver
 */
void bm_gpio_set_mode(uint8_t gpio_num, bm_gpio_mode_t mode);

/**
 * @brief Set gpio output level (0 - 31) only
 */
void bm_gpio_write(uint8_t gpio_num, bm_gpio_level_t level);

/**
 * @brief Toggle gpio output (0 - 31) only
 * Note: Implemented via reading out register
 */
void bm_gpio_toggle(uint8_t gpio_num);

#ifdef __cplusplus
}
#endif