<h1 align="center">⚡ ESP32-S3 Bare-Metal GPIO Driver</h1>

<p align="center">
  <em>A low-level GPIO driver for the ESP32-S3-WROOM-1 written in pure C — direct hardware register access, no SDK abstractions.</em>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-A8B9CC?style=for-the-badge&logo=c" alt="C"/>
  <img src="https://img.shields.io/badge/Target-ESP32--S3--WROOM--1-E7352C?style=for-the-badge" alt="ESP32-S3"/>
  <img src="https://img.shields.io/badge/Build-PlatformIO%20%2B%20CMake-orange?style=for-the-badge" alt="PlatformIO"/>
  <img src="https://img.shields.io/badge/Level-Bare--Metal-111111?style=for-the-badge" alt="Bare-Metal"/>
</p>

---

## 🔬 About the Project

This project implements a **bare-metal GPIO driver** for the **ESP32-S3-WROOM-1** entirely in C, without using ESP-IDF's GPIO API, Arduino, or any other abstraction layer. All pin control happens by **writing directly to hardware registers** at their fixed memory-mapped addresses.

The goal was to understand the hardware at the lowest level: how peripheral registers map into the CPU address space, why `volatile` is non-negotiable, what atomic register access means on a multi-core SoC, and how to handle real-world signal issues like mechanical button bounce.

---

## 🧠 Core Concepts

### Memory-Mapped I/O
ESP32-S3 peripherals live at fixed addresses in the CPU's address space. Writing to `0x60004008` directly controls the GPIO hardware — no library call, no abstraction.

```c
volatile uint32_t *out_w1ts = (volatile uint32_t *)0x60004008;
*out_w1ts = (1u << gpio_num);  // sets pin atomically
```

### Why `volatile`?
Without `volatile`, the compiler may cache register reads, eliminate writes it considers redundant, or reorder operations — all of which silently break hardware control. Hardware registers must always be `volatile`.

### Atomic Set/Clear — W1TS / W1TC Registers
A naive read-modify-write (`reg |= mask`) is not atomic. An interrupt or the second core can corrupt bits between the read and the write. The ESP32-S3 solves this with dedicated registers:

| Register | Behaviour |
|---|---|
| `OUT_W1TS` | Write `1` to a bit → **sets** it. Other bits untouched. |
| `OUT_W1TC` | Write `1` to a bit → **clears** it. Other bits untouched. |

These are atomic at the hardware level — no critical section needed for single-bit output control.

### Debounced Button Input
Mechanical buttons bounce for 5–30 ms on press/release, producing rapid spurious transitions. The input driver uses a **stable-count integrator debouncer**: samples the pin every N milliseconds and requires a consecutive run of identical readings before registering a state change. No blocking delays — robust and real-time friendly.

Pull-up/pull-down resistors are configured by writing directly to the **IO MUX / pad configuration registers**.

---

## 📦 Features

| Feature | Status |
|---|---|
| GPIO output enable / disable | ✅ |
| Atomic set / clear / toggle via W1TS, W1TC | ✅ |
| GPIO input enable + read | ✅ |
| Pull-up / pull-down (IO MUX pad registers) | ✅ |
| Stable-count software debouncing | ✅ |
| Safe types: `uint32_t`, `uint8_t`, `uintptr_t`, `enum` | ✅ |

---

## 🗂️ Project Structure

```
esp32-s3-wroom-1_bare-metal-GPIO-driver/
├── src/           # Driver implementation
├── include/       # Headers and register address definitions
├── lib/           # Supporting libraries
├── test/          # Test harness
├── platformio.ini
└── CMakeLists.txt
```

---

## 🚀 Getting Started

**Prerequisites:** [PlatformIO](https://platformio.org/) + ESP32-S3-WROOM-1 board

```bash
git clone https://github.com/aIex-personal/esp32-s3-wroom-1_bare-metal-GPIO-driver.git

pio run --target upload   # build and flash
pio device monitor        # serial output
```

---

## ⚠️ Scope & Known Limitations

This driver covers **GPIO 0–31 output/input** with the pin already routed to GPIO function. A production-grade driver would additionally handle:

- IO MUX function selection (SPI, UART, etc.)
- Drive strength configuration
- Interrupt support
- GPIO 32–48 (separate register bank)
- Multi-core critical sections for complex sequences

For bare-metal LED blink and button input with debouncing, the implementation is correct and clean.

---

## 💡 Why Build This?

Most embedded work happens behind ESP-IDF or Arduino abstractions. Stripping those away to control hardware directly builds the foundational understanding needed for performance-critical firmware, low-level debugging, and custom peripheral drivers — skills that don't come from framework tutorials.