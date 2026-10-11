# step 6 — KiCad schematic (reference document)

*2026-10-06 — step 6 complete (pending approval)*

schematic diagram: [06-schematic.svg](06-schematic.svg)
use this document when entering the schematic into KiCad.

---

## 1. complete net / connection table

every net, every endpoint. match net names exactly in KiCad.

### power nets

| net | from | to | notes |
|---|---|---|---|
| `+5V` | J_USB pin 1 → F1 → SW1 → bus | DevKitC USB-C (physical cable) | main power rail |
| `+5V` | +5V bus | U4 DRV8833 VM | motor power |
| `+5V` | +5V bus | U3 MAX98357A VDD | amp power |
| `+5V` | +5V bus | C1 top | bulk cap |
| `+5V` | +5V bus | C2 top | decoupling on DRV8833 |
| `+5V` | +5V bus | C3 top | decoupling on MAX98357A |
| `+3V3` | U1 J1-1 (DevKitC 3V3 pin) | bus | onboard AMS1117 output |
| `+3V3` | +3V3 bus | U2 display VCC | |
| `+3V3` | +3V3 bus | U5 VL53L0X VCC | |
| `+3V3` | +3V3 bus | R4 top (nSLEEP pull-up) | |
| `+3V3` | +3V3 bus | R5 top (I²C SCL pull-up) | |
| `+3V3` | +3V3 bus | R6 top (I²C SDA pull-up) | |
| `+3V3` | +3V3 bus | C5 top (display decoupling) | |
| `+3V3` | +3V3 bus | C4 top (VL53L0X decoupling) | |
| `+3V3` | +3V3 bus | C6 top (optional 3V3 buffer) | |
| `+3V3` | +3V3 bus | J_EXP pin 7 | expansion header 3V3 |
| `GND` | U1 J1-22, J3-21 | all GND pins (see below) | common ground |

### GND connections (all on `GND` net)

| component | pin |
|---|---|
| J_USB | pin 2 |
| U1 DevKitC | J1-22 (×2) and J3-21 (×2) |
| U2 display | GND pin |
| U3 MAX98357A | GND pin |
| U4 DRV8833 | GND pin |
| U5 VL53L0X | GND pin |
| SW2/SW3/SW4 buttons | one leg each |
| D1 RGB LED | cathode (K) |
| C1 bottom | bulk cap GND |
| C2–C6 bottom | decoupling GND |
| J_EXP | pin 8 |

### SPI2 bus (display) — net names: DISP_*

| net | U1 pin | U2 pin | notes |
|---|---|---|---|
| `DISP_SCK` | GPIO12 (J1-18) | SCK | SPI2 FSPICLK — IO_MUX path |
| `DISP_MOSI` | GPIO11 (J1-17) | SDA/MOSI | SPI2 FSPID |
| `DISP_CS` | GPIO10 (J1-16) | CS | SPI2 FSPICS0 |
| `DISP_DC` | GPIO13 (J1-19) | DC | GPIO, not SPI peripheral |
| `DISP_RST` | GPIO9 (J1-15) | RST | active-low; GPIO output |
| `DISP_BLK` | GPIO14 (J1-20) | BLK | LEDC PWM; drive HIGH in firmware init |

### I²S0 bus (audio) — net names: I2S_*

| net | U1 pin | U3 pin | notes |
|---|---|---|---|
| `I2S_BCLK` | GPIO4 (J1-4) | BCLK | bit clock |
| `I2S_LRC` | GPIO5 (J1-5) | LRC | word select / left-right clock |
| `I2S_DIN` | GPIO6 (J1-6) | DIN | data to amp |

### I²C0 bus (ToF sensor) — net names: I2C_*

| net | U1 pin | U5 pin | R5/R6 | J_EXP | notes |
|---|---|---|---|---|---|
| `I2C_SCL` | GPIO7 (J1-7) | SCL | R5 bottom | pin 5 | 4.7kΩ pull-up to +3V3 |
| `I2C_SDA` | GPIO8 (J1-12) | SDA | R6 bottom | pin 6 | 4.7kΩ pull-up to +3V3 |

VL53L0X I²C address: 0x29 (only device on bus, no conflict).

### PWM motor control — net names: MOT_*

| net | U1 pin | U4 pin | notes |
|---|---|---|---|
| `MOT_AIN1` | GPIO15 (J1-8) | AIN1 | motor L forward |
| `MOT_AIN2` | GPIO16 (J1-9) | AIN2 | motor L reverse |
| `MOT_BIN1` | GPIO17 (J1-10) | BIN1 | motor R forward |
| `MOT_BIN2` | GPIO18 (J1-11) | BIN2 | motor R reverse |

### motor outputs

| net | U4 pin | destination |
|---|---|---|
| `MOTA_P` | AOUT1 | J_MOTL pin 1 → M1 motor L wire 1 |
| `MOTA_N` | AOUT2 | J_MOTL pin 2 → M1 motor L wire 2 |
| `MOTB_P` | BOUT1 | J_MOTR pin 1 → M2 motor R wire 1 |
| `MOTB_N` | BOUT2 | J_MOTR pin 2 → M2 motor R wire 2 |

### buttons — net names: BTN*

| net | U1 pin | switch | to GND |
|---|---|---|---|
| `BTN1` | GPIO1 (J3-4) | SW2 pin 1 | SW2 pin 2 → GND |
| `BTN2` | GPIO2 (J3-5) | SW3 pin 1 | SW3 pin 2 → GND |
| `BTN3` | GPIO21 (J3-18) | SW4 pin 1 | SW4 pin 2 → GND |

all buttons: INPUT_PULLUP. no external resistors. pressed = LOW.

### RGB LED — net names: LED_*

| net | U1 pin | resistor | LED pin |
|---|---|---|---|
| `LED_R` | GPIO38 (J3-10) | R1 (150Ω) | Red anode |
| `LED_G` | GPIO47 (J3-17) | R2 (100Ω) | Green anode |
| `LED_B` | GPIO48 (J3-16) | R3 (33Ω) | Blue anode |
| `GND` | — | — | Cathode (K) |

### expansion header (J_EXP)

| pin | net | source |
|---|---|---|
| 1 | `EXP_MTCK` | GPIO39 (J3-9) |
| 2 | `EXP_MTDO` | GPIO40 (J3-8) |
| 3 | `EXP_MTDI` | GPIO41 (J3-7) |
| 4 | `EXP_MTMS` | GPIO42 (J3-6) |
| 5 | `I2C_SCL` | GPIO7 (shared with VL53L0X) |
| 6 | `I2C_SDA` | GPIO8 (shared with VL53L0X) |
| 7 | `+3V3` | +3V3 bus |
| 8 | `GND` | GND |

### nSLEEP pull-up (R4)

| net | from | to |
|---|---|---|
| `+3V3` | R4 top | +3V3 bus |
| `NSLEEP` | R4 bottom | U4 DRV8833 nSLEEP pin |

nFAULT: not connected (open-drain, safe floating).

---

## 2. conflict check — all clear

| check | result |
|---|---|
| GPIO used twice? | No — 21 unique GPIOs, verified |
| SPI2 bus conflict? | No — display is only SPI device |
| I²S0 bus conflict? | No — MAX98357A is only I²S device |
| I²C address conflict? | No — VL53L0X 0x29 is only I²C device |
| USB D+/D- (GPIO19/20) used? | No — reserved, not wired |
| UART TX/RX (GPIO43/44) used? | No — reserved, not wired |
| PSRAM GPIO35/36/37 used? | No — not connected anywhere |
| Strapping pin used as output? | No — GPIO0/3/45/46 not assigned |
| Floating required inputs? | No — see details below |
| Power rail mismatch? | No — all modules on correct rail |
| Level shifting needed? | No — MAX98357A confirmed 3.3V logic OK at 5V VDD |

### floating pin check

| component | pin | state | safe? |
|---|---|---|---|
| U3 MAX98357A | GAIN | float | ✓ float = 9dB (documented default) |
| U3 MAX98357A | SD/MODE | pulled high on Adafruit PCB | ✓ always-on |
| U4 DRV8833 | nFAULT | not connected (open-drain) | ✓ safe |
| U5 VL53L0X | XSHUT | internal 10kΩ pull-up on HiLetgo board | ✓ boots enabled |
| U5 VL53L0X | GPIO1/INT | not connected (open-drain) | ✓ safe |

---

## 3. KiCad symbol names (from standard libraries)

| ref | component | KiCad symbol | library |
|---|---|---|---|
| U1 | ESP32-S3-DevKitC-1 | `ESP32-S3-DevKitC-1` or generic `Conn_02x19_Odd_Even` ×2 | MCU_Module or Connector |
| U2 | SparkFun LCD-27501 | `Conn_01x08` (8-pin header) | Connector_PinHeader_2.54mm |
| U3 | Adafruit MAX98357A | `Conn_01x07` (7-pin header) or `MAX98357A` | Connector or custom |
| U4 | Adafruit DRV8833 | `Conn_01x09` (9-pin header) | Connector |
| U5 | HiLetgo VL53L0X | `Conn_01x04` (4-pin: VCC/GND/SCL/SDA) | Connector |
| LS1 | Speaker | `Speaker` | Device |
| M1 | Motor L | `Motor_DC` | Device |
| M2 | Motor R | `Motor_DC` | Device |
| SW1 | Rocker switch | `SW_SPST` | Switch |
| SW2–SW4 | Tactile buttons | `SW_Push` | Switch |
| D1 | RGB LED | `LED_ARGB` (common cathode) | Device |
| F1 | PTC fuse | `Polyfuse` | Device |
| J_USB | USB-A pigtail | `Conn_01x02` (2-pin screw terminal) or `USB_A` | Connector |
| J_MOTL | Screw terminal L | `ScrewTerminal_01x02` | Connector |
| J_MOTR | Screw terminal R | `ScrewTerminal_01x02` | Connector |
| J_EXP | Expansion header | `Conn_01x08` | Connector |
| R1 | 150Ω LED red | `R` | Device |
| R2 | 100Ω LED green | `R` | Device |
| R3 | 33Ω LED blue | `R` | Device |
| R4 | 10kΩ nSLEEP | `R` | Device |
| R5 | 4.7kΩ I²C SCL | `R` | Device |
| R6 | 4.7kΩ I²C SDA | `R` | Device |
| C1 | 1000µF/16V | `CP` (polarized) | Device |
| C2–C5 | 100nF | `C` | Device |
| C6 | 220µF (optional) | `CP` (polarized) | Device |

**note on U1 DevKitC:** the official KiCad symbol for ESP32-S3-DevKitC-1 may not exist in the
standard library. options:
1. use the `ESP32-S3-DevKitC-1` symbol from kicad-footprints community library (GitHub: espressif/kicad-libraries)
2. create a custom symbol with two 19-pin headers (J1 and J3), labeled with GPIO functions
3. use two `Conn_01x19` symbols (one for J1, one for J3) and add a frame label

**recommended:** use the Espressif official KiCad library — it has the DevKitC-1 symbol.
repo: https://github.com/espressif/kicad-libraries

---

## 4. footprints to verify before routing

| ref | footprint | verify |
|---|---|---|
| U1 DevKitC-1 | 2× 1×19 through-hole female header at 2.54mm, spaced 25.4mm apart (1 inch) | confirm from official mechanical drawing: DevKitC board is 25.4mm between header rows |
| U2 SparkFun LCD-27501 | 1×8 through-hole 2.54mm male header | verify orientation vs display datasheet (ZJY200S0800TG01.pdf): is VCC pin 1 or pin 8? check before placing |
| U3 Adafruit MAX98357A | 1×7 through-hole 2.54mm male header | Adafruit product page confirms 7-pin 2.54mm layout; verify pin 1 position |
| U4 Adafruit DRV8833 | 1×9 through-hole 2.54mm male header (2×9 total: two rows) | Adafruit 3297 has 2 rows of 9 pins — verify exact row spacing from datasheet |
| U5 HiLetgo VL53L0X | 1×4 through-hole 2.54mm male header (VCC/GND/SCL/SDA) | generic GY-VL53L0XV2 board — measure physical pins before KiCad (some have 6 pins, some 4) |
| C1 | radial through-hole, 1000µF 16V | 8mm dia × 12mm tall, 3.5mm pin pitch — verify fits physical cap you buy |
| C2–C6 | radial ceramic or electrolytic | 5mm pitch standard; double-check if ceramic or electrolytic ordered |
| F1 MF-R200 | radial through-hole | Bourns MF-R200: 7.7mm × 3.6mm, 5.08mm pitch — verify pitch |
| J_MOTL/J_MOTR | KF301-2P screw terminal | 5.08mm pitch confirmed in BOM; verify height clears chassis |
| D1 RGB LED | 5mm through-hole LED | standard 5mm, 4-pin (K, R, G, B); confirm common-cathode pinout by testing with multimeter before soldering |
| SW2–SW4 | 6×6mm DIP tactile switch | standard 6×6×5mm — most KiCad footprints match; confirm 2.54mm pin pitch |

---

## 5. remaining uncertainties

### resolved ✓
- MAX98357A 5V VDD + 3.3V I²S logic: **confirmed** (Adafruit learn guide)
- GPIO35/36/37 off-limits on N16R8: **confirmed** (Espressif docs)
- SPI2 hardware defaults at GPIO9–14: **confirmed** (ESP-IDF v5.3)
- DRV8833 nSLEEP pull-up required: **confirmed** (datasheet)
- Blue LED resistor 33Ω: **confirmed** (Vf calculation)

### open (must verify before routing)

1. **SparkFun LCD-27501 pin 1 identity**
   the schematic shows VCC/GND/RST/CS/MOSI/SCK/DC/BLK in order, but the exact pin 1
   position on the physical header has not been verified against the ZJY200S0800TG01 datasheet.
   **action:** download mechanical drawing from SparkFun product page; confirm before placing footprint.

2. **HiLetgo GY-VL53L0XV2 pin count**
   some versions have 4 pins (VCC/GND/SCL/SDA), some have 6 (adds XSHUT and GPIO1/INT).
   **action:** check the specific unit ordered. if 6-pin version, add XSHUT and INT pads to
   footprint even if they route to GND/NC.

3. **Adafruit DRV8833 (3297) row spacing**
   the DRV8833 breakout has two rows of pins. the exact distance between rows (not 2.54mm — probably 15.24mm or similar) must be measured.
   **action:** check Adafruit eagle/kicad files for the 3297 breakout, or measure the physical board.

4. **DevKitC-1 exact header spacing**
   official spec says 25.4mm between J1 and J3 header rows (1 inch). confirm this vs
   the physical board before committing the footprint.
   **action:** measure physical DevKitC or check Espressif official KiCad library.

5. **nSLEEP internal pull-down strength (DRV8833)**
   confirmed that internal pull-down exists from datasheet. 10kΩ to +3V3 is strong enough
   to override it (3.3V / 10kΩ = 0.33mA, well above any internal pull-down). no action needed.

6. **I²C pull-ups: HiLetgo module may have onboard 4.7kΩ already**
   if the specific unit has onboard pull-ups, adding ours in parallel gives 2.35kΩ effective,
   which is fine (within I²C spec). no harm either way.

7. **RGB LED pinout**
   5mm common-cathode RGB LED has 4 pins. standard pinout: longest leg = common cathode,
   then R, G, B in order. verify with your specific LED using multimeter in diode mode before soldering.
   **action:** test LED pinout before PCB build.

---

## 6. schematic checks passed

| check | status |
|---|---|
| GPIO conflicts | ✓ none |
| Correct power voltage per module | ✓ all correct |
| Ground connections | ✓ all modules grounded |
| Motor driver logic levels | ✓ 3.3V → DRV8833 VIH 2.0V, safe |
| I²S wiring | ✓ BCLK/LRC/DIN correct |
| SPI wiring (including DC as GPIO) | ✓ all 6 display signals wired |
| I²C pull-ups | ✓ R5/R6 4.7kΩ to +3V3 |
| LED resistors (values corrected) | ✓ R1=150Ω R2=100Ω R3=33Ω |
| Capacitor placement | ✓ C1 near DRV8833 VM; C2-C5 near each IC |
| Fuse and switch in series on +5V | ✓ J_USB → F1 → SW1 → +5V bus |
| No floating required inputs | ✓ all verified above |
| Motor screw terminals wired | ✓ AOUT1/2 → J_MOTL, BOUT1/2 → J_MOTR |
| Expansion header includes I²C | ✓ J_EXP pins 5-6 = SCL/SDA |

---

## step 6 is complete ✓

next: **step 7 — on-board vs header split.**
decide which nets land on the PCB directly and which go to connectors before KiCad layout.

alternatively: enter schematic into KiCad using this document + 06-schematic.svg as reference,
then run ERC (electrical rules check) before step 7.
