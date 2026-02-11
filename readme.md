observations bm_gpio_s3

1.  Inside the esp32-s3, peripherals (spio, uart, spi, etc) live at fixed addresses in the CPU's address space, therefore when we write to address 0x60004008, we are writing the gpio hardware's control register
2.  In order to write to an address in c we make a pointer that points to that address:
    /* out_w1ts holds the address 0x60004008 */
    volatile uint32_t *out_w1ts = (volatile uint32_t *)0x60004008;
    /* *out_w1ts writes to that address */
    *out_w1ts = (1u << 2);
The write becomes a bus transaction that reaches the GPIO peripheral.
3.  volatile tells the compiler that this memory can change outsiide the program / has side effects, do not optimize reads/writes away.
Without volatile, the compiler might: remove unused writes, cache reads in register, reorder operations in ways that reak harware control.
Hardware registers must be volatile.
4. In terms of esp32_s3 GPIO registers are 32-bit wide, therefore we use uint32_t for register access.
1u << gpio_num -> produces a 32 bit mask
Using the correct width avoids undefined behavior and partial writes.
5. uintptr_t for base addresses
uintptr_T is an unsigned integer type guaranteed to hold a pointer value
6. Why use W1TC/W1TS registers
*out_reg |= mask; // read-modify-write
That’s not atomic, 1 CPU reads current value, 2 CPU modifies it, 3 CPU writes it back
If an interrupt or another core changes the same register between step 1 and 3, you can corrupt bits.
Instead, the S3 provides “write 1 to set” and “write 1 to clear” registers
    OUT_W1TS: writing 1 sets the bit (others untouched)
    OUT_W1Tc: writing 1 clears that bit (others untouched)
This is atomic at hardware level and avoids read-modify-write hazards.
Same idea for ENABLE_W1TS/W1TC.
7. Datatypes used and why:
    uint8_t gpio_num : GPIO number fits in 0-48, so 8 bit is enough
    uint32_t mask : register are 32 bit
    enum : makes api self documenting and prevents magic numbers
    bool : validation helpers
8. Definition of done and limitations
This driver is “complete” for simple output control of GPIO 0..31 assuming the pin is already routed to GPIO function.
A truly full professional GPIO driver on ESP32-S3 would also handle:
    IO MUX / GPIO matrix selection (pin function)
    pull-up / pull-down configuration
    drive strength
    input enable, interrupts, etc.
    GPIO32..48 (separate registers)
    multi-core / critical sections if doing more complex sequences
    But for “blink a pin with bare-metal register writes”, this is correct and clean.