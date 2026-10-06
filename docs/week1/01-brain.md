# step 1 — the brain

*decided 2026-10-06 · locked*

## the choice

**`ESP32-S3-WROOM-1-N16R8`** — a module, not a bare chip.

- 16 mb flash
- 8 mb octal psram
- wi-fi + ble
- esp32-s3 dual core

## what buddy demanded from a brain

the feature list turns into hardware requirements. these are what actually
narrowed the field:

| buddy needs | what it demands |
|---|---|
| hear me | i²s input for a digital mic |
| talk back | i²s output to an amp — a second peripheral, or full duplex |
| expressive face | fast spi plus enough ram to hold a screen of pixels |
| two wheels | 2+ pwm channels |
| wheel encoders | hardware pulse counting — doing it in software eats the cpu |
| obstacle + imu | i²c |
| touch on the head | capacitive touch pins |
| wireless control, updates | wi-fi and ble |
| room to grow | spare gpio not yet spent |

the two that quietly kill most candidates are **audio in and out at the same
time** and **ram for the display**.

## what i ruled out and why

| option | why not |
|---|---|
| rp2040 / pico | pio would handle encoders beautifully, but wireless means bolting on a separate radio and audio in + out is a lot of manual work |
| esp32-c3 | single core, modest ram, no psram, fewer pins. fine for a sensor node, too small for a display plus two audio streams |
| stm32 | best motor control of the lot, but no wireless and a much steeper first-pcb learning curve |
| original esp32 | would mostly work. the s3 is better at exactly what buddy needs — audio, display, touch channels, native usb |

## why a module and not a bare chip

this matters more than the chip choice.

a bare s3 means designing the radio section myself — antenna, impedance-matched
trace, crystal, rf ground pour. get it slightly wrong and the board boots but has
terrible range, with no way to diagnose it without equipment i don't have.

the module is that whole section already designed, built, certified, with the
antenna on it. connect power, ground, and the pins i want. for a first serious
board it removes the biggest category of "it came back broken and i can't tell why".

## two consequences

**psram costs pins.** the octal psram in the r8 modules uses several gpios
internally, so they aren't available. the usable pin count is lower than the
headline number. **the exact list gets read off the espressif wroom-1 datasheet at
step 4 — not guessed.**

**native usb saves a chip.** the s3 flashes straight over usb with no usb-to-serial
converter on the board. one less part to buy, solder and debug.

## not carried over

buddy mini's gpio assignments are **not** reused. that was a plain esp32 with an
oled, two buttons and a buzzer. different chip, different peripherals, different
board. the pin map gets built from scratch at step 4.

## still open

- soldering setup, which decides whether parts are hand-solderable or need hot air
- everything in step 2: mic, audio amp, speaker, motor driver, motors, imu,
  distance sensor, display, power, encoders, touch, connectors
