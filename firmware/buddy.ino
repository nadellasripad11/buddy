/*
 * Buddy Robot Firmware
 * ESP32-S3-DevKitC-1-N16R8
 *
 * Pin map from docs/week1/04-pin-map.md (verified 2026-10-06)
 * Board: buddy-robot-easyeda.json (33 components, 39 nets)
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <VL53L0X.h>
#include "driver/i2s.h"

// ---- I2S Audio (MAX98357A) ----
#define I2S_BCLK  4
#define I2S_LRC   5
#define I2S_DIN   6

// ---- I2C Sensor (VL53L0X) ----
#define I2C_SCL   7
#define I2C_SDA   8

// ---- SPI Display (ST7789 240x320) ----
#define DISP_SCK  12
#define DISP_MOSI 11
#define DISP_CS   10
#define DISP_DC   13
#define DISP_RST  9
#define DISP_BLK  14

// ---- Motors (DRV8833) ----
#define MOT_L_FWD 15  // AIN1
#define MOT_L_REV 16  // AIN2
#define MOT_R_FWD 17  // BIN1
#define MOT_R_REV 18  // BIN2

// ---- Buttons (active-low, internal pull-up) ----
#define BTN1  1
#define BTN2  2
#define BTN3  21

// ---- RGB LED (common-cathode, LOW = on) ----
#define LED_R 38
#define LED_G 47
#define LED_B 48

// ---- Objects ----
Adafruit_ST7789 tft = Adafruit_ST7789(DISP_CS, DISP_DC, DISP_RST);
VL53L0X tof;

// ---- Motor PWM channels ----
#define PWM_FREQ 5000
#define PWM_RES  8
#define CH_L_FWD 0
#define CH_L_REV 1
#define CH_R_FWD 2
#define CH_R_REV 3

void setMotors(int left, int right) {
  // left/right: -255..255
  if (left >= 0) {
    ledcWrite(CH_L_FWD, left);
    ledcWrite(CH_L_REV, 0);
  } else {
    ledcWrite(CH_L_FWD, 0);
    ledcWrite(CH_L_REV, -left);
  }
  if (right >= 0) {
    ledcWrite(CH_R_FWD, right);
    ledcWrite(CH_R_REV, 0);
  } else {
    ledcWrite(CH_R_FWD, 0);
    ledcWrite(CH_R_REV, -right);
  }
}

void setLED(bool r, bool g, bool b) {
  // Common-cathode: LOW = on
  digitalWrite(LED_R, r ? LOW : HIGH);
  digitalWrite(LED_G, g ? LOW : HIGH);
  digitalWrite(LED_B, b ? LOW : HIGH);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Buddy booting...");

  // Buttons
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  // RGB LED
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  setLED(true, false, false);  // Red = booting

  // Motors (LEDC PWM)
  ledcSetup(CH_L_FWD, PWM_FREQ, PWM_RES);
  ledcSetup(CH_L_REV, PWM_FREQ, PWM_RES);
  ledcSetup(CH_R_FWD, PWM_FREQ, PWM_RES);
  ledcSetup(CH_R_REV, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOT_L_FWD, CH_L_FWD);
  ledcAttachPin(MOT_L_REV, CH_L_REV);
  ledcAttachPin(MOT_R_FWD, CH_R_FWD);
  ledcAttachPin(MOT_R_REV, CH_R_REV);
  setMotors(0, 0);

  // Display backlight
  pinMode(DISP_BLK, OUTPUT);
  digitalWrite(DISP_BLK, HIGH);

  // SPI Display
  SPI.begin(DISP_SCK, -1, DISP_MOSI, DISP_CS);
  tft.init(240, 320);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.println("BUDDY");
  tft.setTextSize(2);
  tft.println("booting...");

  // I2C + ToF sensor
  Wire.begin(I2C_SDA, I2C_SCL);
  tof.setTimeout(500);
  if (tof.init()) {
    tof.startContinuous();
    Serial.println("VL53L0X OK");
  } else {
    Serial.println("VL53L0X FAIL");
  }

  // I2S Audio (MAX98357A)
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true,
  };
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DIN,
    .data_in_num = I2S_PIN_NO_CHANGE,
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);

  setLED(false, true, false);  // Green = ready
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 10);
  tft.println("BUDDY READY");
  Serial.println("Buddy ready.");
}

void loop() {
  // ToF distance
  uint16_t dist = tof.readRangeContinuousMillimeters();
  if (!tof.timeoutOccurred()) {
    tft.fillRect(10, 60, 220, 30, ST77XX_BLACK);
    tft.setCursor(10, 60);
    tft.print("Dist: ");
    tft.print(dist);
    tft.print("mm");
  }

  // Buttons
  if (digitalRead(BTN1) == LOW) {
    setLED(true, true, false);  // Yellow
    setMotors(150, 150);        // Forward
    delay(200);
  } else if (digitalRead(BTN2) == LOW) {
    setLED(false, false, true); // Blue
    setMotors(-150, -150);      // Reverse
    delay(200);
  } else if (digitalRead(BTN3) == LOW) {
    setLED(true, false, true);  // Magenta
    setMotors(0, 0);            // Stop
    delay(200);
  } else {
    setLED(false, true, false); // Green = idle
    setMotors(0, 0);
  }

  delay(50);
}
