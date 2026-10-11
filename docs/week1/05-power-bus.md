# step 5 — power and bus conflict check (corrected)

*2026-10-06 — step 5 corrected and complete*

source: step 3 BOM (exact parts), step 4 locked GPIO map,
Adafruit MAX98357A learn guide (pinouts + overview pages, verified 2026-10-06),
DRV8833 datasheet, AMS1117-3.3 datasheet, VL53L0X datasheet,
ESP32-S3-DevKitC-1 schematic.

---

## 1. voltage rail for every module

| module | VDD rail | why |
|---|---|---|
| ESP32-S3-DevKitC-1 | 5 V via USB-C (internal AMS1117 → 3.3 V) | standard USB power |
| ST7789 display (SparkFun LCD-27501) | **3.3 V** | module rated 3.3 V; SPI signals 3.3 V |
| MAX98357A amp (Adafruit 3006) | **5 V** | Adafruit guide: "power from 5 V to get most power output" with 3.3 V logic |
| VL53L0X ToF (HiLetgo GY-VL53L0XV2) | **3.3 V** | breakout onboard 2.8 V regulator; VCC pin accepts 2.8–5 V but 3.3 V keeps I²C pull-ups on the right rail |
| DRV8833 motor driver (Adafruit 3297) | **5 V (VM)** | motor power from 5 V rail; no separate logic supply pin — logic inputs are VM-referenced but accept 3.3 V CMOS (see §7) |
| RGB LED | 3.3 V GPIO output | GPIO source current through resistors |
| buttons | 3.3 V internal pull-up | INPUT_PULLUP, pressed = 0 V |
| N20 motors ×2 | **5 V via DRV8833** | rated 6 V; 5 V = ~17% speed reduction, acceptable (see §8) |

---

## 2. power tree

```
USB power bank (5 V, 2 A minimum)
└── USB-A pigtail → PCB input screw terminal
    ├── PTC fuse (MF-R200, 2 A hold / 4 A trip)
    └── SPST rocker switch (≥ 3 A)
        └── 5 V rail
            ├── DevKitC-1 USB-C port  ←── power bank → USB-A → USB-C cable
            │   └── AMS1117-3.3 (onboard, 800 mA rated)
            │       └── 3.3 V rail
            │           ├── ESP32-S3 core (~80–150 mA avg, 240 mA Wi-Fi TX peak)
            │           ├── ST7789 display VCC (~25 mA with backlight)
            │           ├── VL53L0X VCC (~20 mA peak, ~1 mA idle)
            │           ├── I²C pull-ups (2× 4.7 kΩ → ~0.7 mA)
            │           ├── DRV8833 nSLEEP pull-up (10 kΩ → ~0.33 mA)
            │           └── RGB LED (via 150/100/33 Ω → up to ~27 mA)
            └── 5 V rail (also available at DevKitC J1-21 = VBUS direct)
                ├── MAX98357A VDD (~2 mA idle, peaks during audio)
                ├── DRV8833 VM (motor power)
                │   ├── 1000 µF bulk cap (within 10 mm of VM/GND pins)
                │   ├── 100 nF decoupling (within 5 mm of VM pin)
                │   ├── Motor L (N20, ~120 mA run, ~333 mA stall at 5 V)
                │   └── Motor R (N20, ~120 mA run, ~333 mA stall at 5 V)
                └── 100 nF decoupling on MAX98357A VDD pin
```

**note on 5 V wiring path:** the power bank connects both to the DevKitC (via USB-C cable for the ESP32 path) and directly to the 5 V screw terminal rail on the PCB (for motors and MAX98357A). route the high-current motor path directly from the screw terminal — do not route motor current through the DevKitC board traces.

---

## 3. bus diagram

```
ESP32-S3-DevKitC-1
│
├─ SPI2 (GPIO9–14) ───────────────────────► SparkFun LCD-27501 (ST7789)
│  GPIO12 → SCK  (FSPICLK, IO_MUX)          VCC = 3.3 V | 8-pin 2.54 mm header
│  GPIO11 → MOSI (FSPID,   IO_MUX)          SCK/MOSI/CS/DC/RST/BLK = 3.3 V
│  GPIO10 → CS   (FSPICS0, IO_MUX)          one device; no MISO; no bus sharing
│  GPIO13 → DC   (GPIO)
│  GPIO9  → RST  (GPIO, active-low)
│  GPIO14 → BLK  (LEDC PWM; tie HIGH if no dimming needed)
│
├─ I²S0 (GPIO4–6) ────────────────────────► Adafruit MAX98357A (3006)
│  GPIO4 → BCLK                             VDD = 5 V (3.3 V logic accepted — confirmed)
│  GPIO5 → LRC  (word select)               GAIN = float → 9 dB default
│  GPIO6 → DIN  (audio data to amp)         SD/MODE = pulled high on breakout (always on)
│                                           OUT+/OUT− → Speaker 4 Ω (Adafruit 3968)
│
├─ I²C0 (GPIO7–8) ────────────────────────► HiLetgo GY-VL53L0XV2 (VL53L0X)
│  GPIO7 → SCL  ┬── 4.7 kΩ → 3.3 V         VCC = 3.3 V | I²C addr = 0x29
│  GPIO8 → SDA  ┘                           XSHUT = internal 10 kΩ pull-up (device boots enabled)
│                                           GPIO1/INT = not connected (leave floating; open-drain safe)
│
├─ LEDC PWM (GPIO15–18) ──────────────────► Adafruit DRV8833 (3297)
│  GPIO15 → AIN1 (motor L fwd)              VM = 5 V
│  GPIO16 → AIN2 (motor L rev)              nSLEEP → 10 kΩ → 3.3 V (must pull high; see §7)
│  GPIO17 → BIN1 (motor R fwd)              AOUT1/AOUT2 → Motor L screw terminal
│  GPIO18 → BIN2 (motor R rev)              BOUT1/BOUT2 → Motor R screw terminal
│                                           nFAULT = not connected (open-drain, safe floating)
│
├─ GPIO IN (GPIO1, 2, 21) ────────────────► Buttons ×3 (6×6 mm NO)
│  INPUT_PULLUP; pressed = LOW              one leg to GPIO, other leg to GND
│
└─ GPIO OUT (GPIO38, 47, 48) ─────────────► Common-cathode RGB LED
   GPIO38 → 150 Ω → LED Red anode          cathode to GND
   GPIO47 → 100 Ω → LED Green anode
   GPIO48 → 33 Ω  → LED Blue anode
```

---

## 4. MAX98357A: corrected power architecture

**verified from Adafruit learn guide (pinouts + overview, 2026-10-06):**

> "You can provide 2.5V up to 5.5V" for VDD.
> "The audio input is I²S standard, you can use 3.3V or 5V logic data."
> "If you have a 3.3V logic device, you can still power the amp from 5V,
>  and that's recommended to get the most power output!"

the previous step 5 draft was wrong. the breakout has an onboard input buffer that
accepts 3.3 V logic regardless of VDD. no level shifting is needed or should be added.

**final architecture:**
- MAX98357A VDD = **5 V** (from PCB 5 V rail)
- BCLK / LRC / DIN = **3.3 V logic** from ESP32-S3 GPIOs
- output power at 5 V into 4 Ω ≈ **3.2 W** (Adafruit spec)
- speaker: Adafruit 3968, 4 Ω, 5 W rated — perfectly matched

---

## 5. I²C pull-ups

required: 4.7 kΩ from SDA (GPIO8) to 3.3 V, and 4.7 kΩ from SCL (GPIO7) to 3.3 V.

the HiLetgo GY-VL53L0XV2 **may** include onboard 4.7 kΩ pull-ups (varies by batch).
we add them on our PCB regardless. if the module also has them, effective pull-up =
2 × 4.7 kΩ in parallel = 2.35 kΩ. that is within I²C spec for standard-mode (100 kHz)
on traces under 10 cm. no harm.

VL53L0X is the only I²C device on this bus (address 0x29). no conflicts.

---

## 6. SPI bus

one master (ESP32-S3), one slave (ST7789 via SparkFun LCD-27501).
display is write-only — no MISO wire needed.
GPIO9–14 hit SPI2 FSPI hardware defaults exactly (verified in step 4 sanity check) —
IO_MUX path, no GPIO-matrix routing, best signal integrity.

BLK (GPIO14): if not driven, backlight may default off on some ST7789 boards.
**fix in firmware:** drive GPIO14 HIGH at boot for always-on backlight.
add PWM brightness control later if wanted.

---

## 7. DRV8833: motor driver power and logic

**VM (motor power):** 5 V rail. DRV8833 rated 2.7–10.8 V. ✓
**logic inputs (AIN1/AIN2/BIN1/BIN2/nSLEEP):** the DRV8833 datasheet specifies:
- VIH (logic high threshold) = 2.0 V typical
- ESP32-S3 GPIO high output = 3.0–3.3 V
- 3.3 V > 2.0 V → **no level shifting needed** ✓

**nSLEEP pull-up (critical):**
the DRV8833 has an internal pull-down on nSLEEP. if left floating, it defaults LOW →
chip sleeps → both motor outputs tri-stated → motors do nothing.
fix: 10 kΩ resistor from nSLEEP to **3.3 V** (not 5 V — use 3.3 V to stay consistent with
ESP32-S3 GPIO voltage domain if a GPIO is added later for software sleep).

---

## 8. motor current and power bank requirements

N20 motors at 5 V (rated 6 V):
- no-load current: ~100 mA per motor
- stall current at 5 V ≈ 400 × (5/6) ≈ **333 mA per motor**
- two motors stalling simultaneously: ~**666 mA**

DRV8833 channel limit: 1.5 A continuous. 666 mA total is well within one motor's
channel headroom. ✓

full 5 V rail current budget:

| load | current |
|---|---|
| ESP32-S3 (Wi-Fi TX peak, via AMS1117) | ~240 mA |
| Display, VL53L0X, LED, pull-ups (via AMS1117) | ~75 mA |
| AMS1117 quiescent | ~10 mA |
| MAX98357A (audio at moderate volume, VDD=5 V) | ~120 mA |
| Motors running (no stall) | ~240 mA |
| **total normal operating** | **~685 mA** |
| Motors stall (both, 5 V) | +666 mA |
| **total motor stall peak** | **~1.35 A** |

**power bank requirement:** must supply ≥ 2 A at 5 V. any modern USB power bank
(10 000 mAh class) delivers 2.1–2.4 A. the peak stall of 1.35 A is brief (<100 ms
before PWM reduces duty or current limit kicks in on DRV8833).

**PTC fuse (BOM item 15, MF-R200, 2 A hold / 4 A trip):**
- 2 A hold > 1.35 A peak stall. fuse will not false-trip during normal stall. ✓
- 4 A trip protects against dead short. ✓
- confirmed correct. no change needed.

**USB cable:** must be 2 A rated (data + power cable, not charge-only thin wire).
note on PCB silkscreen: "use 2 A+ USB cable."

---

## 9. motor-induced brownout analysis

the AMS1117-3.3 dropout voltage = ~1.1 V (800 mA rated). it needs ≥ 4.4 V input to
hold 3.3 V output. the ESP32-S3 brownout reset threshold = **2.44 V** (firmware default).

worst case: both motors stall at same moment, full 5 V rail sag.

```
resistance in current path:
  USB cable:      ~0.15 Ω (2 A rated, 1 m)
  power bank ESR: ~0.20 Ω
  PCB traces:     ~0.05 Ω
  total R_path:   ~0.40 Ω

I_total at stall = 666 mA (motors) + 240 mA (ESP32 Wi-Fi) + 120 mA (amp) + 75 mA (peripherals)
                 = 1101 mA ≈ 1.1 A

V_sag = 5.0 V − (1.1 A × 0.40 Ω) = 5.0 − 0.44 = 4.56 V

AMS1117 output = 4.56 V − 1.1 V dropout = 3.46 V
```

3.46 V >> 2.44 V brownout threshold. **no brownout risk under worst-case stall.** ✓

however, the 1000 µF bulk cap on the 5 V rail near DRV8833 is still required:
motor startup inrush is a current spike of 400–600 mA lasting ~0.5–2 ms — faster than
the power bank can respond. the cap absorbs this spike locally, preventing a transient
rail sag that could reset the ESP32 even when steady-state current is within budget.

**additional mitigation (optional, low cost):** 220 µF electrolytic on 3.3 V rail at
DevKitC J1-1/2 header pins. absorbs the AMS1117 transient response time (~10 µs).
from your electrolytic assortment. place within 20 mm of the 3.3 V header pin.

---

## 10. decoupling capacitor placement

| cap | location | purpose |
|---|---|---|
| 1000 µF / 16 V electrolytic | 5 V rail, ≤ 10 mm from DRV8833 VM/GND | motor inrush buffer |
| 100 nF ceramic | DRV8833 VM pin | HF switching noise on motor driver |
| 100 nF ceramic | MAX98357A VDD pin | HF audio switching noise |
| 100 nF ceramic | VL53L0X VCC pin | sensor measurement stability |
| 100 nF ceramic | ST7789 display VCC pin | SPI burst noise |
| 220 µF electrolytic (optional) | 3.3 V rail, ≤ 20 mm from DevKitC J1-1 | AMS1117 transient buffer |

the DevKitC already has onboard decoupling on its 3.3 V and 5 V VBUS lines — do not
add extra caps to the DevKitC's own VCC pads. add them at the module connection points.

---

## 11. RGB LED resistors — corrected

BOM item 11 lists "100 Ω × 3." blue LED Vf ≈ 3.0–3.2 V, so 100 Ω gives ~1 mA (barely
visible). replace with:

| channel | Vf typical | V across R | I at that R | **resistor** |
|---|---|---|---|---|
| red | 2.0 V | 1.3 V | 8.7 mA | **150 Ω** |
| green | 2.2 V | 1.1 V | 11 mA | **100 Ω** |
| blue | 3.0–3.2 V | 0.1–0.3 V | 3–9 mA | **33 Ω** |

all from resistor assortment. GPIO max sink = 40 mA/pin; total ≈ 27 mA. ✓

---

## 12. DevKitC 5 V input

the power bank → USB-C cable → DevKitC is standard USB 5 V. the DevKitC accepts
4.75–5.25 V on VBUS. power bank output = 5.0 V. ✓

the 5 V pin on J1-21 is VBUS direct. we take motor + amp power from there via PCB trace.
current returns through USB GND → cable GND → power bank GND. cable must be ≥ 2 A.

the DevKitC has no user-accessible polyfuse on VBUS (the board schematic shows a small
diode and ferrite only). our MF-R200 PTC on the PCB input protects the entire path.

---

## 13. complete issue list — final

| # | issue | verdict | fix |
|---|---|---|---|
| 1 | MAX98357A VDD: 5 V + 3.3 V logic | **confirmed correct** | VDD = 5 V rail; no level shifter |
| 2 | blue LED resistor 100 Ω too high | **must fix** | use 33 Ω; red = 150 Ω, green = 100 Ω |
| 3 | motors derated 6 V → 5 V | info only | 17% speed loss; acceptable |
| 4 | DRV8833 nSLEEP missing from BOM | **must add** | 10 kΩ from nSLEEP → 3.3 V |
| 5 | I²C pull-ups must be on PCB | **must add** | 2× 4.7 kΩ on SDA/SCL → 3.3 V |
| 6 | 1000 µF cap placement | placement rule | ≤ 10 mm from DRV8833 VM/GND |
| 7 | brownout: confirmed safe | info only | V_3.3 drops to 3.46 V at peak stall; above 2.44 V reset |
| 8 | SPI BLK pin needs GPIO14 HIGH in firmware | minor | set GPIO14 HIGH at init |
| 9 | PTC fuse MF-R200 correct | confirmed ✓ | no change |
| 10 | decoupling caps per module | placement rule | 100 nF within 5 mm of each VCC pin |
| 11 | 5 V input via USB correct | confirmed ✓ | 2 A cable required |
| 12 | no hidden bus/address conflicts | confirmed ✓ | no change |

---

## 14. BOM delta from step 5

no new parts to order. all fixes come from assortment kits.

**update item 11 (LED resistors):**
- was: 100 Ω × 3
- now: 150 Ω × 1 (red) + 100 Ω × 1 (green) + 33 Ω × 1 (blue)

**add passive — nSLEEP pull-up:**
- 10 kΩ × 1 resistor (DRV8833 nSLEEP → 3.3 V). from assortment.

**add passives — I²C pull-ups:**
- 4.7 kΩ × 2 resistors (SCL + SDA → 3.3 V). from assortment.

**optional — 3.3 V rail buffer:**
- 220 µF / 10 V electrolytic × 1. from assortment.

---

## step 5 is complete ✓

power architecture finalized:
- 5 V: DevKitC (USB-C), MAX98357A VDD, DRV8833 VM, motors
- 3.3 V: ESP32-S3 (via onboard AMS1117), display, VL53L0X, I²C pull-ups, LED, nSLEEP pull-up
- no level shifting needed anywhere
- no brownout risk at worst-case motor stall (3.46 V on 3.3 V rail)
- PTC fuse, rocker switch, bulk cap all correctly sized

next: **step 6 — on-board vs header split.**
decide which nets land directly on the PCB (pads + traces) and which go to connectors
(headers, screw terminals). this freezes connector placement before KiCad layout.
