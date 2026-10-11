# firmware

Arduino firmware for the ESP32-S3-DevKitC-1-N16R8.

## buddy.ino

Bring-up firmware using the verified pin map from `docs/week1/04-pin-map.md`:

- **Display**: ST7789 240x320 SPI (GPIO9-14)
- **Audio**: MAX98357A I2S amp (GPIO4-6)
- **ToF**: VL53L0X distance sensor (GPIO7-8 I2C)
- **Motors**: DRV8833 dual H-bridge (GPIO15-18 PWM)
- **Buttons**: 3x tactile, active-low (GPIO1/2/21)
- **RGB LED**: common-cathode (GPIO38/47/48)

Button 1 = forward, Button 2 = reverse, Button 3 = stop.
RGB shows status: red = booting, green = ready/idle.

## required libraries

Install via Arduino Library Manager:

- Adafruit GFX Library
- Adafruit ST7789 Library
- VL53L0X by Pololu

## board setup

- Board: "ESP32S3 Dev Module"
- USB CDC On Boot: Enabled (for Serial Monitor)
- Flash Size: 16MB
- PSRAM: OPI PSRAM
