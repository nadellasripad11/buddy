# buddy

a small autonomous desk companion with a personality.

eventually i want it to move around on two wheels, hear me, talk back, show a face,
and act like it has moods. right now it's a plan and a pile of sketches. nothing is
built.

built for hack club. this repo is the real record of it, including the parts i get
wrong.

---

![buddy](media/reference/reel-storyboard.webp)

that's the look i'm aiming at. it's concept art — not hardware that exists.

---

## the shape of it

```
                 ┌─────────┐
                 │ display │
                 │  FACE   │
                 └────┬────┘
                      │
 microphone ──────┐   │   ┌────── speaker
                  ↓   ↓   ↓
               ┌─────────────┐
               │  ESP32-S3   │
               │    BRAIN    │
               └──────┬──────┘
                      │
          ┌───────────┼───────────┐
          ↓           ↓           ↓
       sensors     controller   motors
          │                       │
       IMU / ToF              left/right
                               wheels
```

the full spec — every subsystem, the software layers, and the parts i think are
risky — is in **[docs/00-vision.md](docs/00-vision.md)**.

## the short version

| system | what it is |
|---|---|
| brain | esp32-s3 module — wi-fi, ble, and i2s hardware for audio |
| face | a proper display, bigger than a tiny oled. blinks, expressions, animations |
| ears | digital microphone, so it can hear "hey buddy" |
| voice | i2s amp into a speaker — real speech, not buzzer beeps |
| movement | two geared dc motors with encoders, dual motor driver on the board |
| senses | distance sensor, imu, touch sensor on the head |
| control | wireless controller with a joystick and action buttons |
| power | rechargeable battery, charging circuit, separate power path for the motors |

## where i'm at

| week | what | state |
|---|---|---|
| 1 | working out the plan — what goes inside, how it connects | in progress |
| 2 | pcb schematic and board in kicad | not started |
| 3 | order the board, start the firmware | not started |
| later | body in cad, assembly, bring-up, personality | not started |

nothing above week 1 has been started. no board, no cad, no parts ordered.

## repo layout

```
buddy/
├── docs/            the plan and the notes
├── hardware/        kicad project, when it exists
├── firmware/        the esp32-s3 code, when it exists
├── journal/         devlog, one file per session
├── media/           renders, reel assets, build photos
└── README.md
```

## notes

- [00 — the whole thing](docs/00-vision.md) — the full target: move, talk, face, personality, senses
- [week 1 reel](docs/reel-week1.md) — the 21s cut, timeline and voiceover

## a note on the warm-up

before this i built [buddy-mini](https://github.com/nadellasripad11/buddy-mini), a
breadboard prototype with an oled face, two buttons and a buzzer. it was how i
learned which esp32 pins are safe and how the parts actually work. **its pin plan
does not carry over** — buddy is an esp32-s3 with motors, audio and sensors, which
is a different board entirely.

## time

hours go through hackatime under project `buddy`. only time actually spent on this
counts.
