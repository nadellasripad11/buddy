# step 7 — on-board vs header split

*2026-10-07 — deciding what lands on the PCB vs what plugs in*

---

## the question

for every component in the BOM: does it live **directly on the PCB** (soldered or socketed)
or does it **plug into a connector** on the PCB?

this decision drives:
- how many connector footprints we need
- how serviceable/swappable each part is
- final PCB size and shape

---

## decision table

| ref | component | mounting | reason |
|---|---|---|---|
| U1 | ESP32-S3-DevKitC-1 | **2× female header socket** (2.54mm 1×19) | must be removable for programming, reuse, and replacement. sockets let it plug/unplug. |
| U2 | SparkFun LCD-27501 | **male pin header** on PCB, display plugs in | display may need repositioning; socketed keeps it swappable |
| U3 | Adafruit MAX98357A | **male pin header** on PCB, breakout plugs in | breakout is $5.95 — keep it swappable |
| U4 | Adafruit DRV8833 | **male pin header** on PCB, breakout plugs in | motor driver may need swap if motors change |
| U5 | HiLetgo VL53L0X | **male pin header** on PCB, breakout plugs in | sensor may need repositioning for chassis alignment |
| LS1 | Speaker | **2-pin screw terminal** (J_SPK) | wire runs to speaker mounted in chassis |
| M1 | Motor L | **2-pin screw terminal** (J_MOTL) | motor mounts in chassis, wires run to PCB |
| M2 | Motor R | **2-pin screw terminal** (J_MOTR) | same as M1 |
| SW1 | Rocker switch | **through-hole direct** on PCB | switch mounts through chassis wall; stays on PCB |
| SW2–SW4 | Tactile buttons | **through-hole direct** on PCB | buttons mount through top panel |
| D1 | RGB LED | **through-hole direct** on PCB | LED pokes through top panel |
| F1 | PTC fuse | **through-hole direct** on PCB | fuse is a PCB component, not a connector |
| J_USB | USB-A pigtail | **2-pin screw terminal** on PCB | pigtail from power bank plugs into screw terminal |
| J_EXP | Expansion header | **male pin header** on PCB edge | future add-ons plug in here |
| R1–R6 | Resistors | **through-hole direct** on PCB | standard passives |
| C1–C6 | Capacitors | **through-hole direct** on PCB | standard passives |

---

## connector summary — what to add to the PCB

| designator | type | pins | nets |
|---|---|---|---|
| J1_SOCK | Female header 1×19 | 19 | DevKitC J1 |
| J3_SOCK | Female header 1×19 | 19 | DevKitC J3 |
| J_DISP | Male header 1×8 | 8 | display VCC/GND/RST/CS/MOSI/SCK/DC/BLK |
| J_AMP | Male header 1×7 | 7 | amp VDD/GND/BCLK/LRC/DIN/GAIN/SD |
| J_MOTOR | Male header 2×9 | 18 | DRV8833 all pins (or 1×9 ×2) |
| J_TOF | Male header 1×4 | 4 | VCC/GND/SCL/SDA |
| J_SPK | Screw terminal 2-pin | 2 | amp OUT+/OUT− |
| J_MOTL | Screw terminal 2-pin | 2 | AOUT1/AOUT2 |
| J_MOTR | Screw terminal 2-pin | 2 | BOUT1/BOUT2 |
| J_USB | Screw terminal 2-pin | 2 | +5V/GND from power bank |
| J_EXP | Male header 1×8 | 8 | GPIO39–42 + I²C + 3V3 + GND |

total connector pins on PCB: **93 pins** across 11 connectors.

---

## PCB size estimate

with all breakouts plugged in as daughter boards, the footprint drivers are:

- DevKitC: 25.4mm × 51mm (dominates — sets minimum board width to ~70mm)
- DRV8833 breakout: ~25mm × 20mm
- display: ~45mm × 33mm (hangs off one edge, doesn't add to base area)
- VL53L0X: tiny, ~18mm × 13mm

**estimated PCB size: 80mm × 70mm** (two-layer, fits inside a 100mm × 100mm fab minimum)

100×100mm is the cheapest tier at JLCPCB/PCBWay ($2 for 5 boards). we have headroom.

---

## layout strategy

```
┌─────────────────────────────────────────┐
│  [J_USB]  [F1]  [SW1]                   │
│                                         │
│  [ESP32-S3-DevKitC-1 socket]            │
│   J1_SOCK            J3_SOCK            │
│                                         │
│  [J_AMP]   [J_TOF]   [J_MOTOR]         │
│                                         │
│  [J_MOTL] [J_MOTR]  [J_SPK]           │
│                                         │
│  [R1-R6]  [C1-C6]                      │
│  [SW2] [SW3] [SW4]   [D1]              │
│                           [J_EXP] ──►  │
└─────────────────────────────────────────┘
```

- power entry (USB screw terminal + fuse + switch) at the top left
- DevKitC socket centered — most traces stay short since GPIOs fan out to all sides
- breakout headers (amp, ToF, motor driver) below the DevKitC, near their GPIO pins
- motor screw terminals and speaker terminal at the bottom (wires exit chassis bottom)
- buttons and LED at the bottom, facing up through panel
- expansion header on the right edge

---

## what this means for KiCad

when entering the schematic:
1. replace `U2 display`, `U3 MAX98357A`, `U4 DRV8833`, `U5 VL53L0X` symbols with
   their corresponding **connector symbols** (Conn_01x08, Conn_01x07, etc.)
2. add the screw terminal symbols for motors, speaker, USB
3. U1 DevKitC becomes two `Conn_01x19` female header symbols

the actual chip-level symbols (MAX98357A, DRV8833, VL53L0X) go in the schematic as
reference sheets or sub-schematics only — not on the main board schematic.

---

## step 7 complete ✓

uncertainties cleared: 0 new ones opened.

next: **step 8 — BOM finalization**
confirm quantities, add the new connectors, total cost, source all parts from one cart.
