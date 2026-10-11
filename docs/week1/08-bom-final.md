# step 8 — BOM finalization

*2026-10-08 — confirmed quantities, connectors added, one cart*
*updated: swapped SparkFun/Adafruit breakouts for generic equivalents → saved ~$21*

---

## complete BOM (buy-once list)

| # | ref | description | qty | unit $ | total $ | source |
|---|---|---|---|---|---|---|
| 1 | U1 | ESP32-S3-DevKitC-1-N16R8 | 1 | $13.30 | $13.30 | Amazon |
| 2 | U2 | Generic ST7789 1.69″ SPI display breakout | 1 | $5.50 | $5.50 | AliExpress |
| 3 | U3 | Generic MAX98357A I²S amp breakout | 1 | $2.00 | $2.00 | AliExpress |
| 4 | U4 | Generic DRV8833 motor driver breakout | 1 | $1.50 | $1.50 | AliExpress |
| 5 | U5 | HiLetgo VL53L0X ToF sensor | 1 | $6.79 | $6.79 | Amazon |
| 6 | LS1 | 4Ω 3W speaker 40mm | 1 | $2.50 | $2.50 | AliExpress |
| 7 | M1/M2 | N20 gear motor 6V 100RPM | 2 | $4.70 | $9.40 | Amazon |
| 8 | C1 | 1000µF 16V electrolytic cap | 1 | ~$0.30 | $0.30 | assortment |
| 9 | C2–C5 | 100nF ceramic cap | 4 | ~$0.05 | $0.20 | assortment |
| 10 | C6 | 220µF 16V electrolytic cap | 1 | ~$0.20 | $0.20 | assortment |
| 11 | R1 | 150Ω resistor 1/4W | 1 | ~$0.02 | $0.02 | assortment |
| 12 | R2 | 100Ω resistor 1/4W | 1 | ~$0.02 | $0.02 | assortment |
| 13 | R3 | 33Ω resistor 1/4W | 1 | ~$0.02 | $0.02 | assortment |
| 14 | R4 | 10kΩ resistor 1/4W | 1 | ~$0.02 | $0.02 | assortment |
| 15 | R5/R6 | 4.7kΩ resistor 1/4W | 2 | ~$0.02 | $0.04 | assortment |
| 16 | D1 | 5mm RGB LED common cathode | 1 | ~$0.30 | $0.30 | assortment |
| 17 | SW1 | SPST rocker switch 3A | 1 | ~$1.20 | $1.20 | Amazon |
| 18 | SW2–SW4 | 6×6mm tactile push button | 3 | ~$0.10 | $0.30 | assortment |
| 19 | F1 | Bourns MF-R200 PTC fuse 2A | 1 | ~$0.60 | $0.60 | Digi-Key |
| 20 | — | USB-A pigtail cable 30cm | 1 | ~$2.00 | $2.00 | Amazon |
| **new** | J1_SOCK/J3_SOCK | Female header 1×19 2.54mm | 2 | ~$0.40 | $0.80 | Amazon |
| **new** | J_DISP | Male header 1×8 2.54mm | 1 | ~$0.10 | $0.10 | assortment |
| **new** | J_AMP | Male header 1×7 2.54mm | 1 | ~$0.10 | $0.10 | assortment |
| **new** | J_MOTOR | Male header 2×9 2.54mm | 1 | ~$0.30 | $0.30 | Amazon |
| **new** | J_TOF | Male header 1×4 2.54mm | 1 | ~$0.10 | $0.10 | assortment |
| **new** | J_EXP | Male header 1×8 2.54mm | 1 | ~$0.10 | $0.10 | assortment |
| **new** | J_SPK/MOTL/MOTR/USB | KF301-2P screw terminal 5.08mm | 4 | ~$0.30 | $1.20 | Amazon |
| **new** | — | N20 motor brackets + screws | 2 | ~$1.50 | $3.00 | Amazon |
| **new** | — | PCB fabrication (JLCPCB 5× 80×70mm) | 5 | ~$0.40 | $2.00 | JLCPCB |

---

## cost summary

| category | cost | vs old |
|---|---|---|
| microcontroller | $13.30 | — |
| breakout modules | $17.79 | was $37.64 |
| passives + LED + buttons | $1.42 | — |
| connectors + headers | $2.60 | — |
| power (switch + fuse + USB) | $3.80 | — |
| motors + brackets | $12.40 | — |
| PCB fabrication (5 boards) | $2.00 | — |
| **total** | **$53.31** | **was $73.16** |

**saved $19.85** by swapping SparkFun/Adafruit breakouts for generic AliExpress equivalents. same chips, same pinout, same firmware. tradeoff: 2–3 week AliExpress shipping vs 3–5 day Adafruit.

Amazon/Digi-Key items still ship fast. order AliExpress first — it's the long pole.

---

## one-cart shopping list

**Amazon** (one order):
- ESP32-S3-DevKitC-1-N16R8
- HiLetgo VL53L0X
- N20 6V gear motors ×2
- N20 motor brackets ×2
- KF301-2P screw terminals (pack)
- Female header 1×19 (pack)
- Male header 2×9 DRV8833
- SPST rocker switch
- USB-A pigtail

**AliExpress** (one order — order first, longest shipping):
- ST7789 1.69" SPI display breakout (search "ST7789 1.69 SPI display module")
- MAX98357A I²S amp breakout (search "MAX98357A amplifier module")
- DRV8833 motor driver breakout (search "DRV8833 motor driver module")
- 4Ω 3W 40mm speaker

**Digi-Key** (one order, or add to assortment):
- Bourns MF-R200 PTC fuse

**Already have (assortment kits)**:
- Resistors, ceramic caps, electrolytic caps, misc headers, RGB LED, tactile buttons

---

## step 8 complete ✓

next: **step 9 — PCB component placement**
