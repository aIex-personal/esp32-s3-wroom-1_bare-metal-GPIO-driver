#include "bm_gpio_s3.h"
#include <stdint.h>

/* low level register definitions (esp32-s3 gpio peripheral)*/

/* use uintptr_t for adresses (portable integer type that can hold a pointer)*/
#define GPIO_BASE_ADDR              ((uintptr_t)0x60004000u)

/* register offsets from esp32-s3 TRM */
#define GPIO_OUT_REG_OFFSET         (0x0004u)
#define GPIO_OUT_W1TS_OFFSET        (0x0008u)
#define GPIO_OUT_W1TC_OFFSET        (0x000Cu)

#define GPIO_ENABLE_REG_OFFSET      (0x0020u)
#define GPIO_ENABLE_W1TS_OFFSET     (0x0024u)
#define GPIO_ENABLE_W1TC_OFFSET     (0x0028u)

/* macro to get a volatile 32-bit register pointer */
/* converts a raw address into a pointer to a 32-bit hardware register */
#define REG32(addr) ((volatile uint32_t *)(addr))

static inline uint32_t bm_gpio_mask(uint8_t gpio_num)
{
    return (uint32_t)1u << gpio_num;
}

static inline bool bm_gpio_is_valid_low(uint8_t gpio_num)
{
    return (gpio_num < 32u);
}

/* public api */
void bm_gpio_set_mode(uint8_t gpio_num, bm_gpio_mode_t mode)
{
    if (!bm_gpio_is_valid_low(gpio_num))
    {
        return; /*minimal safety, might want to assert*/
    }

    volatile uint32_t *enable_w1ts = REG32(GPIO_BASE_ADDR + GPIO_ENABLE_W1TS_OFFSET);
    volatile uint32_t *enable_w1tc = REG32(GPIO_BASE_ADDR + GPIO_ENABLE_W1TC_OFFSET);

    uint32_t mask = bm_gpio_mask(gpio_num);

    if (mode == BM_GPIO_MODE_OUTPUT)
    {
        *enable_w1ts = mask; //atomic set enable bit
    } else
    {
        *enable_w1tc = mask; //atomic clear enable bit
    }
    
}

void bm_gpio_write(uint8_t gpio_num, bm_gpio_level_t level)
{
    if (!bm_gpio_is_valid_low(gpio_num)) {
        return;
    }

    volatile uint32_t *out_w1ts = REG32(GPIO_BASE_ADDR + GPIO_OUT_W1TS_OFFSET);
    volatile uint32_t *out_w1tc = REG32(GPIO_BASE_ADDR + GPIO_OUT_W1TC_OFFSET);

    uint32_t mask = bm_gpio_mask(gpio_num);

    if (level == BM_GPIO_LEVEL_HIGH) {
        *out_w1ts = mask;  // atomic set output high
    } else {
        *out_w1tc = mask;  // atomic set output low
    }
}

void bm_gpio_toggle(uint8_t gpio_num)
{
    if (!bm_gpio_is_valid_low(gpio_num)) {
        return;
    }

    volatile uint32_t *out_reg  = REG32(GPIO_BASE_ADDR + GPIO_OUT_REG_OFFSET);
    uint32_t current = *out_reg;

    if ((current & bm_gpio_mask(gpio_num)) != 0u) {
        bm_gpio_write(gpio_num, BM_GPIO_LEVEL_LOW);
    } else {
        bm_gpio_write(gpio_num, BM_GPIO_LEVEL_HIGH);
    }
}