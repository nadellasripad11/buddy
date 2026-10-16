# firmware

arduino firmware for the esp32-s3-devkitc-1-n16r8.

> not tested on real hardware yet — bring-up code written from the pin map. needs a first flash and check before trusting it.

## buddy.ino

bring-up firmware using the verified pin map from `docs/week1/04-pin-map.md`:

- display — st7789 240x320 spi (gpio9-14)
- audio — max98357a i2s amp (gpio4-6)
- tof — vl53l0x distance sensor (gpio7-8 i2c)
- motors — drv8833 dual h-bridge (gpio15-18 pwm)
- buttons — 3x tactile, active-low (gpio1/2/21)
- rgb led — common-cathode (gpio38/47/48)

button 1 is forward, button 2 is reverse, button 3 is stop.
the rgb led shows status: red while booting, green when ready or idle.

## libraries you need

install these through the arduino library manager:

- adafruit gfx library
- adafruit st7789 library
- vl53l0x by pololu

## board setup

- board: "esp32s3 dev module"
- usb cdc on boot: enabled (so the serial monitor works)
- flash size: 16mb
- psram: opi psram
