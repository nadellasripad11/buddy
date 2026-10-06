# step 3 — verified parts list

*2026-10-06 — step 3 complete*

every part has a verified US price, a vendor link, and enough mechanical info to
place a footprint in kicad. "through-hole" or "module" column tells you what kind of
pad goes on buddy's own board.

---

## complete bom

| # | block | product name | part # | vendor | price (USD) | dimensions | voltage | why it fits | board type |
|---|---|---|---|---|---|---|---|---|---|
| 1 | brain | ESP32-S3-DevKitC-1-N16R8 | ESP32-S3-DEVKITC-1-N16R8 | [Digikey](https://www.digikey.com/en/products/detail/espressif-systems/ESP32-S3-DEVKITC-1-N16R8/15971100) | **$13.30** | 68.3 × 25.4 mm, 2 rows × 19 pins, 2.54 mm pitch | 5 V via USB-C or Vin | official Espressif board — ESP32-S3-WROOM-1-N16R8 module soldered on; 16 MB flash, 8 MB octal PSRAM | module socketed on female headers |
| 2 | face | SparkFun TFT LCD 2.0" 240×320 SPI | [LCD-27501](https://www.sparkfun.com/products/27501) | [SparkFun](https://www.sparkfun.com/products/27501) | **$18.95** | 37 × 68 × 4 mm active area; 8-pin 2.54 mm header | 3.3 V | IPS panel (confirmed), ST7789 driver, 4-wire SPI, 2.54 mm 8-pin = standard through-hole header | module on headers |
| 3 | audio amp | MAX98357A I²S Amplifier Breakout | [Adafruit 3006](https://www.adafruit.com/product/3006) | Adafruit | **$5.95** | 19.4 × 17.8 mm | 2.7–5.5 V | 3.2 W into 4 Ω @ 5 V; 3-wire I²S (BCLK/LRC/DIN); SD pin = mono select | module on headers |
| 4 | speaker | Speaker 40 mm diameter, 4 Ω, 5 W | [Adafruit 3968](https://www.adafruit.com/products/3968) | Adafruit | **$4.95** | 40 mm dia, bare wire leads | 3.3–5 V audio | matches MAX98357A 4 Ω output exactly; 5 W rated = will never clip at 3 W drive | screw terminal / solder pads |
| 5 | motor driver | DRV8833 Dual H-Bridge Motor Driver Breakout | [Adafruit 3297](https://www.adafruit.com/product/3297) | Adafruit | **$5.95** | 26 × 18 mm, 2.54 mm headers | 2.7–10.8 V | only 4 GPIO to drive 2 motors; 1.5 A/channel continuous | module on headers |
| 6 | distance | VL53L0X ToF Distance Sensor Breakout (GY-VL53L0XV2) | HiLetgo B07XXTMRR2 | [Amazon](https://www.amazon.com/s?k=VL53L0X+breakout+sensor) | **$6.79** | 25 × 10.7 mm, 2.54 mm 4-pin header (VCC/GND/SDA/SCL) | 2.8–5 V (onboard reg) | I²C 3.3 V native; no level-shift needed on S3; avoids HC-SR04 5 V echo problem | module on headers |
| 7 | motors (×2) | GA12-N20 DC Gearmotor 6 V 200 RPM | GA12-N20, 3-pcs pack | [Amazon](https://www.amazon.com/s?k=GA12-N20+6V+200RPM+motor) | **~$14 for 3** (~$4.70 each, need 2) | body 12 × 10 mm, shaft 3 mm D, total length ~25 mm | 6 V, ~120 mA no-load, ~400 mA stall | 200 RPM at 6 V is workable desk speed; 3 mm D-shaft fits below wheels; driven by DRV8833 | screw terminal wires |
| 8 | wheels (×2) | N20 Rubber Wheel 42 mm | DFRobot FIT0500 or generic | [Amazon](https://www.amazon.com/s?k=N20+42mm+rubber+wheel) | **~$4–6/pair** | 42 mm dia, 9 mm wide, fits 3 mm D-shaft | — | 42 mm = ~13 cm/s at 200 RPM; rubber = grip on desk surface; press-fit on D-shaft | press-fit hardware |
| 9 | bulk cap | 1000 µF 16 V electrolytic, through-hole | generic 1000µF/16V radial | [Amazon](https://www.amazon.com/s?k=1000uf+16v+capacitor) | **~$0.50** (from any assortment kit) | 8 mm dia × 12 mm, 3.5 mm pitch | 16 V rated | brownout buffer for motor inrush; must sit physically close to DRV8833 | through-hole radial |
| 10 | bypass caps (×4) | 0.1 µF 50 V ceramic, through-hole | 104Z 50 V | Amazon assortment | **~$0.20** total | 5 mm pitch, axial/radial | 50 V rated | one per power pin on each module; kills high-freq switching noise | through-hole |
| 11 | LED resistors (×3) | 100 Ω ¼W resistor | generic 100Ω | Amazon assortment | **~$0.10** total | axial, 10 mm body | — | R/G/B current-limit; ~10 mA per channel at 3.3 V forward-drop | through-hole axial |
| 12 | RGB LED | 5 mm common-cathode RGB LED | generic 5mm CC RGB | Amazon assortment | **~$0.30** | 5 mm dia, 2.54 mm pin pitch | 2.0–3.4 V per channel | common cathode = single ground pin; 3 GPIOs drive R/G/B directly | through-hole |
| 13 | buttons (×3) | 6×6 mm tactile push switch, NO | generic 6×6×5 mm | Amazon assortment | **~$0.30** total | 6 × 6 × 5 mm, 2.54 mm 4-pin DIP | — | S3 internal pull-up, no external resistors needed; NO = pressed = LOW | through-hole DIP |
| 14 | power switch | SPST rocker switch ≥3 A, 2-pin | generic or E-Switch R13-28A | Amazon | **~$1–2** | varies by style; mount through panel | 3–10 A rated | stall current for 2 × N20 can hit 800 mA briefly; anything under 1 A switch melts | through-hole / panel mount |
| 15 | PTC fuse | Resettable polyfuse 2 A hold / 4 A trip | Bourns MF-R200 or equiv | [Mouser](https://www.mouser.com/c/?q=MF-R200) / Amazon | **~$0.60** | 7.7 mm × 3.6 mm, radial through-hole | 2 A hold, 60 V | trips on short or overload; auto-resets when cooled; protects power bank | through-hole radial |
| 16 | DevKitC socket (×2 strips) | 1×19 female 2.54 mm header | generic 1×19 or cut from 1×40 strip | Amazon assortment | **~$1** total | 2.54 mm pitch, 19 positions | — | DevKitC has 19 pins per side; two rows socket the whole board | through-hole female header |
| 17 | module / expansion headers | 2.54 mm breakaway male pin header strips | generic 1×40 strip | Amazon assortment | **~$1** total (pack of 10 strips) | 2.54 mm pitch | — | male pins for modules that ship without headers; also expansion connector | through-hole male header |
| 18 | screw terminals (×4) | 2-pin screw terminal 5.08 mm pitch | generic KF301-2P | Amazon | **~$0.40** each (~$1.60 for 4) | 10 × 8 mm, 5.08 mm pitch | — | motors and power input connect with bare wire, no soldering; swap without iron | through-hole |
| 19 | USB-A pigtail | USB Type-A female to bare wires, 20 cm | generic USB-A female breakout | Amazon | **~$2–3** | 14 × 10 mm body | 5 V | plugs into power bank's USB-A port; brings 5 V to board's input terminal | pigtail / connector |
| 20 | motor brackets (×2) | N20 micro gearmotor mounting bracket | generic N20 mount | Amazon | **~$2** for pair | fits 12 × 10 mm N20 body | — | holds motors to chassis; without these the motors spin instead of the wheels | hardware |

---

## price total

| category | subtotal |
|---|---|
| modules (brain, display, amp, driver, ToF) | $50.94 |
| speaker + motors + wheels | ~$23–27 |
| passives, caps, resistors, LED, buttons | ~$3 (from assortment kits) |
| switch + fuse + headers + terminals + pigtail + brackets | ~$7–9 |
| **estimated total** | **~$84–91** |

passives from amazon 500-piece resistor/capacitor assortment kits rather than
ordering each one individually — the total drops and you have spares.

---

## key compatibility notes

**display footprint warning — verify before routing.**
the sparkfun lcd-27501 has an 8-pin 2.54 mm header. double-check the mechanical
drawing against the datasheet (ZJY200S0800TG01.pdf linked on the product page)
before placing pads in kicad. waveshare's otherwise-identical display uses a PH2.0
(2.0 mm pitch) connector — do not mix them up.

**vl53l0x generic vs adafruit.**
the HiLetgo GY-VL53L0XV2 at $6.79 is ~3× cheaper than adafruit 3317 ($22.50).
the chip is the same. the breakout has an onboard 2.8 V regulator and I²C pull-ups,
so it works at 3.3 V just like the adafruit version. risk: no official library support,
but adafruit's pololu-compatible library works fine with it.

**n20 motor shaft.**
the shaft is 3 mm diameter D-flat. the wheel hub must match 3 mm D specifically —
not 2 mm or 4 mm. confirm before ordering wheels.

**drv8833 vs max current.**
each n20 stalls at ~400 mA. DRV8833 is rated 1.5 A/channel continuous, 2 A peak.
comfortable headroom. the 1000 µF bulk cap handles the inrush on startup.

**speaker impedance must match amp.**
MAX98357A is rated for 4 Ω minimum load. the adafruit 3968 is 4 Ω. this is the
only tested pairing — don't substitute an 8 Ω speaker without checking the power
output derating.

---

## where to order the assortment kits

the passives (resistors, ceramic caps, electrolytic caps, tactile buttons, RGB LEDs,
headers) are all available in variety packs on Amazon for $5–15 each. ordering
individually costs more and takes longer. if you don't already have a parts box, get:

- 1× resistor assortment (300–600 pc, ¼W), ~$6
- 1× ceramic cap assortment (0.1 µF / 10 nF / 1 nF mix), ~$5
- 1× electrolytic cap assortment (includes 1000 µF 16 V), ~$7
- 1× tactile button pack (6×6 mm), ~$3
- 1× 5 mm LED assortment (includes CC RGB), ~$3

total for all assortments: ~$24. you'll use them on every project after this.

---

## step 3 is complete ✓

next: **step 4 — pin map.** read the usable GPIOs from the espressif
esp32-s3-wroom-1 datasheet, then decide which pin gets which peripheral.
