# step 9 — PCB component placement

*2026-10-08 — deciding where everything goes on the 80×70mm board*

---

## board spec

- size: **80mm × 70mm**
- layers: **2** (top copper + bottom copper)
- fab: JLCPCB, 2-layer, cheapest tier ($2 for 5 boards)
- coordinate origin: bottom-left corner

---

## placement decisions

### zone 1 — power entry (top-left, 0–30mm x, 60–70mm y)
- J_USB screw terminal: top-left corner, 5V enters here
- F1 PTC fuse: directly after J_USB in series
- SW1 rocker switch: mounts through left chassis wall, pads at x=25mm

rationale: power comes in from the left, goes through fuse and switch before reaching any other component. short high-current trace, easy to inspect.

### zone 2 — DevKitC socket (center, 10–60mm x, 20–62mm y)
- J1_SOCK (left header): x=10, y=20
- J3_SOCK (right header): x=53, y=20
- both 1×19 female sockets, 2.54mm pitch, 51mm tall

rationale: DevKitC dominates the board. everything else fans out from its GPIO pins.

### zone 3 — breakout headers (below DevKitC, 10–70mm x, 5–18mm y)
- J_AMP (MAX98357A, 1×7): x=10, y=5 — near GPIO4/5/6 (I²S, left J1 header)
- J_TOF (VL53L0X, 1×4): x=32, y=5 — near GPIO7/8 (I²C, left J1 header)
- J_MOTOR (DRV8833, 2×9): x=52, y=5 — near GPIO15-18 (PWM, left J1 header)
- J_DISP (display, 1×8): x=10, y=64 — near GPIO9-14 (SPI2, left J1 header)

rationale: headers placed on the same side as their GPIO pins = short traces, low noise.

### zone 4 — output connectors (bottom, 5–75mm x, 0–8mm y)
- J_MOTL screw terminal: x=5, y=0 — motor L wires exit chassis bottom
- J_MOTR screw terminal: x=20, y=0 — motor R wires
- J_SPK screw terminal: x=35, y=0 — speaker wire

### zone 5 — passives cluster (center-right, 58–75mm x, 20–55mm y)
- C1 1000µF bulk cap: x=62, y=38 — as close as possible to J_MOTOR VM pin (≤10mm rule)
- C2–C5 100nF decoupling: one per IC, placed within 3mm of each IC's VCC pin
- R1–R6 resistors: x=62–75mm, y=20–36mm, vertical orientation
- C6 220µF optional buffer: x=62, y=50

### zone 6 — user interface (top edge, 20–70mm x, 64–70mm y)
- SW2/SW3/SW4 buttons: x=26/38/50, y=65 — poke up through top panel
- D1 RGB LED: x=64, y=65 — poke up through top panel

### zone 7 — expansion header (right edge)
- J_EXP 1×8: x=76, y=30 — right edge, perpendicular, accessible from outside

---

## trace width rules

| net | width | reason |
|---|---|---|
| +5V power | 1.5mm | up to 1.35A stall current |
| +3V3 power | 1.0mm | up to 320mA |
| GND pours | copper fill both layers | minimizes ground impedance |
| signal traces | 0.25mm | I²C, SPI, I²S, GPIO |
| motor outputs | 1.5mm | up to 400mA per motor |

---

## via strategy

- use vias to connect top GND copper fill to bottom GND fill (stitching vias every ~8mm around board perimeter)
- signal vias only where a trace must change layers to avoid congestion
- no vias under ICs or in high-current paths

---

## keepout zones

- 3mm from all board edges (fab tolerance)
- 5mm around USB-C connector area on DevKitC (do not route traces here)
- circle r=3mm around each mounting hole

---

## step 9 complete ✓

next: **step 10 — PCB routing**
enter all components into KiCad, run autorouter as starting point, then manually fix high-current traces and critical signal paths.
