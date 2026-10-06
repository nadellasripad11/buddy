# step 2 — the parts

*2026-10-06*

every part is a module on headers or a through-hole component. **no surface-mount
soldering on my board.** each one has a job in the robot — nothing here is padding.

## the list

| block | part | notes |
|---|---|---|
| brain | esp32-s3-devkitc-1-n16r8 | socketed on female headers |
| face | 2.0–2.4" **ips** tft, 240x320, spi, st7789 or ili9341, 3.3 v | ips is a requirement, not a preference |
| audio amp | **max98357a** i²s breakout (adafruit 3006 or generic) | digital in, speaker out |
| speaker | 8 Ω, 1–2 w, 28–40 mm (adafruit 1313 or equiv) | |
| motor driver | **drv8833** breakout (adafruit 3297 / pololu 2130) | 4 gpio |
| motors | 2x n20 geared dc, 6 v, ~200 rpm, with wheels | via screw terminals |
| distance | **vl53l0x** tof breakout (adafruit 3317 / pololu 2490 / gy-530) | i²c, 3.3 v |
| power source | usb power bank, 5 v out | no charging circuit to design |
| power input | 2-pin screw terminal 5.08 mm + usb-a to bare-wire pigtail | |
| switch | spst toggle or rocker, **rated ≥3 a** | |
| protection | ptc resettable fuse, ~2 a hold | |
| bulk cap | 1000 µf electrolytic, 16 v, through-hole | |
| decoupling | 4x 0.1 µf ceramic, through-hole | |
| buttons | 3x 6 mm tactile switch, 6x6x5 mm through-hole | internal pull-ups, no resistors |
| status led | common-cathode 5 mm rgb led | |
| led resistors | ~330 Ω red, ~150 Ω green, ~150 Ω blue | values finalised at schematic |
| headers | female headers for every module | |
| motor connectors | 2x 2-pin screw terminal | swap motors without soldering |
| expansion | 2x 8-pin female: i²c, 3v3, gnd, spare gpio | |

## why these specific choices

### ips display, not tn
buddy sits on a desk and gets looked at from above and off to the side. cheap tn
panels wash out and invert at exactly those angles — the face would look broken from
the one position i'll actually view it from.

**gotchas:** many 2.4" modules bundle a touch controller and a microsd slot sharing
the spi bus — neither is needed, leave them unconnected. and some modules are
3.3 v-only while others have regulators and level shifters; the s3 is 3.3 v so a
3.3 v-native module is the clean match.

### max98357a, not an analog amp
it's a digital i²s amp. no dac, no analog audio stage, no op-amps. three signal
wires plus power. that is what "no complicated onboard audio" means in practice.

its sd pin doubles as channel select — most breakouts default to mono, which is
what i want. confirm on whichever one arrives.

### drv8833, not tb6612fng
the s3 switches milliamps, motors need amps — the driver is the muscle between them.
drv8833 runs from 2.7–10.8 v so 5 v is comfortable, and it needs **4 gpio** against
7 for the tb6612fng. fewer pins, less wiring, plenty of current for n20s.

### vl53l0x, not hc-sr04
hc-sr04 is a 5 v part and its echo pin will damage a 3.3 v input without a level
shifter. the vl53l0x is i²c at 3.3 v, tiny, and shares the bus the imu will want
later. one fewer way to kill the board.

### plain rgb led, not ws2812
addressable leds want 5 v data and have tight timing requirements — a debugging
problem i don't need on a first board. three gpio and three resistors always works.

### the power parts that look boring but aren't

**switch rated ≥3 a.** small slide switches are often rated under 1 a. a stalling
motor pulls more, and an undersized switch heats up and welds shut.

**1000 µf bulk cap.** this is the brownout fix. it's a local reservoir that supplies
the current spike when a motor starts, so the 5 v rail doesn't dip and reset the s3.
it goes physically close to the motor driver — **placement matters as much as the
value**.

**ptc fuse.** trips on a short, resets when fixed. on a first board where a stray
wire is likely, this is the part that saves the power bank and the modules.

## what i deliberately left out

no imu, no microphone, no encoders, no level shifters, no voltage regulators, no
usb-serial chip, no extra leds. the expansion header is how the cut features come
back without a redesign.

## the thing most likely to ruin this board

**a footprint that doesn't match the real part.** hole spacing wrong by 1.27 mm and
the devkitc doesn't fit — the board is scrap, and i don't find out until it arrives.

so before routing anything, every module's hole spacing and pin count gets checked
against its actual mechanical drawing, not against a library symbol someone
uploaded.

## still to do

- step 3: complete block diagram
- step 4: pin map, read off the espressif datasheet
- step 5: check gpio / power / bus conflicts
- step 6: confirm the on-board vs header split
- step 7: expansion headroom
- step 8: schematic in kicad
- step 9: bom
- step 10: place and route
