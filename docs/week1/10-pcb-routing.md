# step 10 — PCB routing

*2026-10-08 — connecting everything up on the 80×70mm board*

---

## routing order

route in this order — critical nets first, signals last:

1. **GND pours** — pour copper fill on both layers, connect with stitching vias every ~8mm around perimeter
2. **+5V power** — from J_USB through F1 through SW1 to J_MOTOR VM and J_AMP VDD (1.5mm trace)
3. **+3V3 power** — from DevKitC 3V3 pin to J_TOF VCC, J_DISP VCC, decoupling caps C2–C5 (1.0mm trace)
4. **motor outputs** — DRV8833 AOUT1/2 to J_MOTL, BOUT1/2 to J_MOTR (1.5mm trace, keep away from signal lines)
5. **I²S bus** — GPIO4→BCLK, GPIO5→LRC, GPIO6→DIN to J_AMP (0.25mm, short run left flank)
6. **I²C bus** — GPIO7→SCL, GPIO8→SDA to J_TOF (0.25mm, short run left flank, 4.7kΩ pullups R5/R6 to 3V3)
7. **SPI2 bus** — GPIO9–14 to J_DISP (0.25mm, short run left flank)
8. **PWM signals** — GPIO15–18 to J_MOTOR control pins (0.25mm, short run right flank)
9. **UI** — SW2/3/4 to GPIOs, D1 RGB to GPIO with R1–R3 (0.25mm)
10. **expansion** — GPIO39–42 + I²C + 3V3 + GND to J_EXP right edge (0.25mm)

---

## trace width rules

| net | width | notes |
|---|---|---|
| +5V | 1.5mm | up to 1.35A motor stall |
| +3V3 | 1.0mm | up to 320mA |
| motor outputs | 1.5mm | 400mA per motor |
| all signals | 0.25mm | I²C, SPI, I²S, PWM, GPIO |
| GND | copper pour | both layers, stitched |

---

## via rules

- stitching vias: 0.6mm drill, 1.2mm pad, every 8mm around board edge
- signal vias: only where a trace must jump layers to clear congestion
- no vias under component pads or in the high-current +5V / motor paths

---

## design rule check (DRC) targets

| rule | value |
|---|---|
| min trace width | 0.2mm |
| min clearance | 0.2mm |
| min via drill | 0.3mm |
| min via annular ring | 0.25mm |
| board edge clearance | 0.3mm |
| copper to edge | 0.5mm |

---

## KiCad entry checklist

1. open `buddy.kicad_sch` — add all connector symbols per 06-schematic.md
2. run ERC, clear all errors
3. assign footprints to all refs per 06-schematic.md footprint list
4. open PCB editor — import netlist from schematic
5. place components per 09-pcb-placement.md zone positions
6. run autorouter (FreeRouting or KiCad built-in) as starting point
7. manually fix: all power traces → widen to spec, all motor traces → widen to spec
8. add GND copper fill both layers, run DRC
9. add silkscreen labels for J_USB, J_MOTL, J_MOTR, J_SPK, polarity marks on C1/C6
10. run DRC final, export gerbers for JLCPCB

---

## step 10 complete ✓

next: **order parts + fab** — place JLCPCB order with gerbers, place Amazon/Adafruit/SparkFun/Digi-Key parts orders. estimated delivery 2–3 weeks.
