#include "debounce.h"

void debounce_init(debounce_t *d, uint8_t threshold, bool initial_state)
{
    d->stable_count = 0;
    d->threshold = threshold;
    d->debounced_state = initial_state;
    d->last_raw_state = initial_state;
}

bool debounce_update(debounce_t *d, bool raw_state)
{
    if (raw_state == d->last_raw_state) {
        if (d->stable_count < d->threshold) {
            d->stable_count++;
        }
    } else {
        d->stable_count = 0;
        d->last_raw_state = raw_state;
    }

    if (d->stable_count >= d->threshold && d->debounced_state != raw_state) {
        d->debounced_state = raw_state;
        return true; // state changed
    }

    return false;
}
