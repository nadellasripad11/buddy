# step 1b — revised architecture

*2026-10-06 · scope cut to something a beginner can actually build*

## scope

**in for the v1 board:** esp32-s3, display face, speaker + audio out, two wheel
motors, wireless control, front distance sensor, a few buttons, status rgb led,
usb/programming, expansion headers.

**out for now, comes back later:** microphone, wheel encoders, imu, touch sensor,
autonomous navigation, custom battery charging, complicated onboard audio, extra
sensors.

the cut lands in the right place — it removes the things that add pins and risk
without changing what buddy is on day one.

## two kept items that cost nothing on the board

**wireless control needs no hardware.** wi-fi and ble are inside the s3. a phone web
page or a ble gamepad costs no parts, no space, no pins. a custom controller is a
separate board for a later week.

**usb and programming are already on the devkitc.** no connector, no boot/reset
buttons, no usb-serial chip on my board.

## power — how "no custom charging" gets honoured

not a usb tether. **a usb power bank is the battery.**

a power bank already contains the cell, the protection circuit and the charger, and
outputs 5 v over usb. the board takes 5 v in and does nothing clever. buddy is
mobile, charging is "plug in the power bank", and there is zero power design risk on
my first board.

the whole power section becomes: input connector, switch, bulk capacitor,
distribution.

## block diagram

```
                        BUDDY  (v1 board)

                    ┌──────────────────┐
                    │  SPI display     │  the face
                    │  on headers      │
                    └────────┬─────────┘
                             │ SPI
   buttons ──┐               │
   rgb led ──┤      ┌────────┴─────────┐
             └─────▶│  ESP32-S3        │◀──── wi-fi / ble
                    │  DevKitC-1       │      (controller — no parts)
   ToF sensor ─────▶│  N16R8           │
   (I2C header)     │  socketed        │
                    └───┬──────────┬───┘
                        │ I2S      │ 4x GPIO
                        ▼          ▼
              ┌──────────────┐  ┌──────────────┐
              │ I2S amp      │  │ motor driver │
              │ breakout     │  │ breakout     │
              └──────┬───────┘  └──────┬───────┘
                     ▼                 ▼
                  speaker        left + right motors
                                 (screw terminals)

   5V in ──▶ switch ──▶ bulk cap ──▶ everything
   (usb power bank)

   expansion: spare GPIO + I2C + 3V3 + GND broken out
```

## on the board vs through headers

| soldered to my board | why |
|---|---|
| female headers for every module | the only joints i make are 2.54 mm pins |
| tactile buttons | through-hole, easy first joints |
| rgb led + resistors | through-hole |
| power switch, input connector, bulk capacitor | through-hole |
| screw terminals for motors | through-hole, and motors swap without soldering |
| expansion headers | where the imu, mic and encoders come back |

| plugs in | why |
|---|---|
| esp32-s3 devkitc-1-n16r8 | the brain |
| spi display | the face |
| i2s amp breakout | avoids fine-pitch audio parts |
| motor driver breakout | avoids a thermal-pad package |
| distance sensor breakout | i2c, and a tiny part otherwise |

**every surface-mount part sits on somebody else's board.** mine is entirely
through-hole. still a real pcb — schematic, bom, routing — just one i can build.

## the risk that's left

**motors and logic sharing one 5 v rail.** motors pull current in sharp spikes when
they start or stall, which drags the rail down. if the s3 sits on that same rail it
browns out and resets mid-move.

this is the main electrical problem remaining. solvable with bulk capacitance,
sensible routing, and possibly separate feeds — handled properly at the schematic
step, not hand-waved. noting it now because it's the thing most likely to bite.

## rough pin budget

| block | pins |
|---|---|
| display (spi) | ~6 |
| i2s audio out | 3 |
| motor driver | 4–7 |
| distance sensor (i2c) | 2 |
| buttons | 3 |
| rgb led | 1–3 |
| **total** | **~19–24** |

the devkitc exposes comfortably more than that, so there is real headroom. exact
numbers get confirmed against the espressif datasheet at step 4.

## expansion — how the cut features come back

the removed parts are all i2c or a couple of gpio:

- **imu** → i2c, 2 pins already on the bus
- **microphone** → i2s input, needs 3 spare gpio
- **wheel encoders** → 2–4 gpio, ideally pulse-counter capable
- **touch** → 1 gpio per pad, s3 has capacitive touch built in

so the expansion header needs: the i2c bus, 3v3, gnd, and a block of spare gpio
chosen so some are touch and pulse-counter capable. that choice happens at step 4.
