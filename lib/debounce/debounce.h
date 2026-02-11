#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t stable_count;
    uint8_t threshold;
    bool    debounced_state;
    bool    last_raw_state;
} debounce_t;

void debounce_init(debounce_t *d, uint8_t threshold, bool initial_state);
bool debounce_update(debounce_t *d, bool raw_state);
/* Returns true when debounced_state changes */
