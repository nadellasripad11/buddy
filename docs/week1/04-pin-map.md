# step 4 — verified pin map

*2026-10-06 — step 4 complete*

source: official Espressif ESP-IDF v5.3 DevKitC-1 user guide, atomic14 community
pinout table (verified against official docs), ESP32-S3 TRM.

---

## 1. all exposed GPIO on the DevKitC-1

the board has two 22-pin headers: J1 (left) and J3 (right).
below is every GPIO broken out, with the J-header position and any fixed function.

**J1 — left side (top to bottom)**

| J1 pos | signal | notes |
|---|---|---|
| J1-1 | 3V3 | power only |
| J1-2 | 3V3 | power only |
| J1-3 | EN / RST | chip reset, not a GPIO |
| J1-4 | GPIO4 | RTC, TOUCH4, ADC1_CH3 |
| J1-5 | GPIO5 | RTC, TOUCH5, ADC1_CH4 |
| J1-6 | GPIO6 | RTC, TOUCH6, ADC1_CH5 |
| J1-7 | GPIO7 | RTC, TOUCH7, ADC1_CH6 |
| J1-8 | GPIO15 | RTC, ADC2_CH4, XTAL_32K_P |
| J1-9 | GPIO16 | RTC, ADC2_CH5, XTAL_32K_N |
| J1-10 | GPIO17 | RTC, ADC2_CH6, U1TXD |
| J1-11 | GPIO18 | RTC, ADC2_CH7, U1RXD |
| J1-12 | GPIO8 | RTC, TOUCH8, ADC1_CH7 |
| J1-13 | GPIO3 | ⚠️ strapping pin — JTAG source select |
| J1-14 | GPIO46 | ⚠️ strapping pin — input only at boot |
| J1-15 | GPIO9 | TOUCH9, ADC1_CH8, FSPIHD |
| J1-16 | GPIO10 | TOUCH10, ADC1_CH9, FSPICS0 |
| J1-17 | GPIO11 | TOUCH11, ADC2_CH0, FSPID |
| J1-18 | GPIO12 | TOUCH12, ADC2_CH1, FSPICLK |
| J1-19 | GPIO13 | TOUCH13, ADC2_CH2, FSPIQ |
| J1-20 | GPIO14 | TOUCH14, ADC2_CH3, FSPIWP |
| J1-21 | 5V | power only |
| J1-22 | GND | ground |

**J3 — right side (top to bottom)**

| J3 pos | signal | notes |
|---|---|---|
| J3-1 | GND | ground |
| J3-2 | GPIO43 | U0TXD — default UART0 TX (serial monitor) |
| J3-3 | GPIO44 | U0RXD — default UART0 RX (serial programming) |
| J3-4 | GPIO1 | RTC, TOUCH1, ADC1_CH0 |
| J3-5 | GPIO2 | RTC, TOUCH2, ADC1_CH1 |
| J3-6 | GPIO42 | JTAG MTMS (see JTAG note) |
| J3-7 | GPIO41 | JTAG MTDI |
| J3-8 | GPIO40 | JTAG MTDO |
| J3-9 | GPIO39 | JTAG MTCK |
| J3-10 | GPIO38 | no special function |
| J3-11 | GPIO37 | 🚫 **N16R8: octal PSRAM — DO NOT USE** |
| J3-12 | GPIO36 | 🚫 **N16R8: octal PSRAM — DO NOT USE** |
| J3-13 | GPIO35 | 🚫 **N16R8: octal PSRAM — DO NOT USE** |
| J3-14 | GPIO0 | ⚠️ strapping pin — boot mode |
| J3-15 | GPIO45 | ⚠️ strapping pin — VDD_SPI voltage |
| J3-16 | GPIO48 | SPICLK_N_DIFF |
| J3-17 | GPIO47 | SPICLK_P_DIFF |
| J3-18 | GPIO21 | ⚠️ reported Wi-Fi sensitivity in output mode; fine as input |
| J3-19 | GPIO20 | 🔒 USB D+ — keep free for programming |
| J3-20 | GPIO19 | 🔒 USB D- — keep free for programming |
| J3-21 | GND | ground |
| J3-22 | GND | ground |

---

## 2. unavailable or constrained pins — summary

| gpio | reason | verdict |
|---|---|---|
| GPIO35 | N16R8 octal PSRAM (internal bus) | **hard no — even though it's on the header** |
| GPIO36 | N16R8 octal PSRAM | **hard no** |
| GPIO37 | N16R8 octal PSRAM | **hard no** |
| GPIO26–34 | flash + PSRAM internal (not broken out at all) | not on headers |
| GPIO19 | USB D- | reserved for programming |
| GPIO20 | USB D+ | reserved for programming |
| GPIO0 | strapping: LOW = download mode | use only as input; never drive LOW during boot |
| GPIO3 | strapping: JTAG source | safe after boot; pull-up internally; avoid driving at boot |
| GPIO45 | strapping: VDD_SPI level | safe after boot; avoid driving during reset |
| GPIO46 | strapping: input-only at boot | safe after boot; treat as input |
| GPIO39–42 | JTAG (MTCK/MTDO/MTDI/MTMS) | usable as GPIO; you lose hardware debugger |
| GPIO43 | UART0 TX | usable as GPIO; you lose serial monitor |
| GPIO44 | UART0 RX | usable as GPIO; you lose serial programming via UART |

**safe, clean GPIOs with no caveats:**
GPIO1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 38, 47, 48 = **21 pins**

**usable with caveats (JTAG but not debugger):**
GPIO39, 40, 41, 42 = **4 more pins** (reserved for expansion header)

---

## 3. peripheral assignments

### design rules applied
- SPI display → hardware SPI2 (FSPI peripheral at GPIO9–14); fastest and simplest
- I²S amp → I²S0 via GPIO matrix; any 3 pins work; chose GPIO4–6 (clean, adjacent)
- I²C sensor → I²C0 via GPIO matrix; any 2 pins; chose GPIO7–8 (adjacent to I²S block)
- Motors → LEDC (PWM) on any GPIO; chose GPIO15–18 (adjacent block on J1)
- Buttons → digital input with internal pull-up; chose GPIO1, 2 (J3) and GPIO21 (J3)
- RGB LED → digital output; chose GPIO38, 47, 48 (J3 lower block, clean)
- USB + UART kept reserved (GPIO19, 20, 43, 44)
- JTAG pins (GPIO39–42) kept free for expansion header

---

## 4. final pin map

| GPIO | buddy function | dir | interface | why |
|---|---|---|---|---|
| **SPI display (SparkFun LCD-27501, ST7789)** |
| GPIO12 | display SCK (clock) | OUT | SPI2 / FSPICLK | hardware SPI2 clock pin |
| GPIO11 | display MOSI (data) | OUT | SPI2 / FSPID | hardware SPI2 data out |
| GPIO10 | display CS (chip select) | OUT | SPI2 / FSPICS0 | hardware SPI2 CS |
| GPIO13 | display DC (data/command) | OUT | GPIO | any GPIO; SPI2 handles timing |
| GPIO9 | display RST (reset) | OUT | GPIO | active-low reset; any GPIO |
| GPIO14 | display BLK (backlight) | OUT | LEDC PWM | optional brightness control; PWM capable |
| **I²S audio (MAX98357A)** |
| GPIO4 | I²S BCLK (bit clock) | OUT | I²S0 | I²S0 routed via GPIO matrix; any pin |
| GPIO5 | I²S LRC / WS (word select) | OUT | I²S0 | left/right channel clock |
| GPIO6 | I²S DIN (data in to amp) | OUT | I²S0 | audio data stream |
| **I²C sensor (VL53L0X)** |
| GPIO7 | I²C SCL | OUT | I²C0 | I²C0 via GPIO matrix; any pin |
| GPIO8 | I²C SDA | IN/OUT | I²C0 | bidirectional; open-drain |
| **left motor (DRV8833 AIN1/AIN2)** |
| GPIO15 | motor L forward (AIN1) | OUT | LEDC PWM | PWM speed control; L channel A |
| GPIO16 | motor L reverse (AIN2) | OUT | LEDC PWM | PWM speed control; L channel B |
| **right motor (DRV8833 BIN1/BIN2)** |
| GPIO17 | motor R forward (BIN1) | OUT | LEDC PWM | PWM speed control; R channel A |
| GPIO18 | motor R reverse (BIN2) | OUT | LEDC PWM | PWM speed control; R channel B |
| **buttons (active-low, internal pull-up)** |
| GPIO1 | button 1 | IN | GPIO INPUT_PULLUP | pressed = LOW; no external resistor |
| GPIO2 | button 2 | IN | GPIO INPUT_PULLUP | pressed = LOW; no external resistor |
| GPIO21 | button 3 | IN | GPIO INPUT_PULLUP | pressed = LOW; note: avoid driving as output (Wi-Fi) |
| **RGB status LED (common-cathode, 100Ω per channel)** |
| GPIO38 | LED red | OUT | GPIO | OUTPUT LOW = red on; 100Ω series resistor on PCB |
| GPIO47 | LED green | OUT | GPIO | OUTPUT LOW = green on; 100Ω series resistor |
| GPIO48 | LED blue | OUT | GPIO | OUTPUT LOW = blue on; 100Ω series resistor |

**total used: 21 GPIO**

---

## 5. reserved + free GPIO

| gpio | status | future use |
|---|---|---|
| GPIO39 | **expansion header** | JTAG MTCK; usable as GPIO if debugger not needed |
| GPIO40 | **expansion header** | JTAG MTDO |
| GPIO41 | **expansion header** | JTAG MTDI |
| GPIO42 | **expansion header** | JTAG MTMS |
| GPIO43 | **keep free** | UART0 TX (serial monitor) |
| GPIO44 | **keep free** | UART0 RX (serial programming via UART) |
| GPIO19 | **keep free** | USB D- (programming + USB-OTG) |
| GPIO20 | **keep free** | USB D+ |
| GPIO0 | caution — input only | boot strapping; can add a 4th button later |
| GPIO3 | caution | JTAG source strapping; avoid at boot |
| GPIO45 | caution | VDD_SPI strapping; safe after boot |
| GPIO46 | caution | input-only strapping; safe after boot |
| GPIO35, 36, 37 | **hard off-limits (N16R8 PSRAM)** | do not wire anything here |

the expansion header on buddy's PCB exposes GPIO39–42 + I²C bus (GPIO7/8) +
3V3 + GND. that's where an IMU, encoder breakout, or second I²C sensor plugs in
without a board redesign.

---

## 6. conflict check

| check | result |
|---|---|
| SPI2 pins (GPIO9–14) clash with any other peripheral? | ✅ no — all clean, dedicated to display |
| I²S pins (GPIO4–6) clash? | ✅ no — plain GPIO, no overlap |
| I²C pins (GPIO7–8) clash? | ✅ no — plain GPIO |
| Motor PWM (GPIO15–18) clash? | ✅ no — plain GPIO, all LEDC-capable |
| Button GPIO1–2 clash with ADC? | ✅ no — not using ADC; INPUT_PULLUP mode |
| GPIO21 Wi-Fi note (button 3)? | ✅ safe as INPUT_PULLUP; issue is output-drive mode only |
| Any pin used twice? | ✅ no — verified: 21 unique GPIOs, no duplicates |
| USB kept free? | ✅ GPIO19, 20 untouched |
| UART kept free? | ✅ GPIO43, 44 untouched |
| N16R8 PSRAM pins avoided? | ✅ GPIO35, 36, 37 assigned to nothing |
| Strapping pins avoided as primary outputs? | ✅ none of GPIO0/3/45/46 used as peripheral outputs |

**result: no conflicts found.**

---

## 7. compromises and notes

**GPIO21 for button 3**
community testing on some modules found GPIO21 can cause Wi-Fi throughput drops
when driven as an output into a low-impedance load. a button with an internal
pull-up is a high-impedance input — the ESP32-S3 is pulling the line, not driving it
hard. if Wi-Fi problems appear, move button 3 to GPIO0 (with care at boot time) or
to one of the JTAG pins (GPIO39–42) and move something else.

**JTAG reserved but not used**
GPIO39–42 are the hardware JTAG interface. they're free for expansion, but
connecting peripherals to them means the hardware debugger stops working. for a
first board without an ESP-Prog, this is a non-issue — the USB JTAG interface
(GPIO19/20) still works for basic debugging.

**SPI2 at GPIO9–14 (FSPI group)**
the ESP32-S3 SPI2 peripheral has a default mapping to GPIO9–14 (the FSPI signals).
using this mapping means no GPIO-matrix routing is needed for the clock and data
lines, which reduces jitter at high SPI speeds. the ST7789 runs happily at 40–80 MHz
SPI; this mapping gives the cleanest signal path.

**display BLK (backlight, GPIO14)**
the ST7789 backlight runs off a separate pin. wiring it to GPIO14 and using LEDC PWM
gives brightness control. if you don't want to implement PWM yet, tie BLK to 3V3 for
always-on backlight and free GPIO14 for later.

**motor nSLEEP on DRV8833**
the DRV8833 has an nSLEEP enable pin. pull it to 3V3 via a 10 kΩ resistor on the
PCB — always enabled, no GPIO needed. a future expansion can wire it to a GPIO if
you want software sleep mode.

---

## step 4 is complete ✓

next: **step 5 — power and bus conflict check.** verify the 5V / 3.3V domains,
I²C pull-up resistors, SPI bus exclusivity, and the motor brownout scenario
before drawing a single wire in KiCad.
