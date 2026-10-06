# buddy — the whole thing

*written 2026-10-05*

a small autonomous desk companion with a personality. this is the full target, not
week 1. nothing here is built.

---

## what buddy should eventually do

### move
- two powered wheels
- forward, backward, left, right
- wireless controller
- eventually simple autonomous movement

### talk
- microphone for hearing me
- speaker for talking back
- wake phrase or wake button
- cloud ai or another speech system for the actual conversation

### face
- animated display
- blinking
- happy, confused, sleepy, annoyed, excited
- listening animation
- talking animation
- movement animations

### personality
- reacts when you touch it
- reacts to sounds
- reacts to what you say
- idle behaviour when nobody is interacting with it
- different little sounds for different actions

### senses
- distance sensor so it can avoid obstacles
- imu so it knows when it's tilted or moved
- wheel encoders so it can estimate movement
- touch sensor on top

### connectivity
- wi-fi
- bluetooth le
- wireless controller
- firmware updates

---

## the architecture

```
                    BUDDY

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

### 1. brain — esp32-s3
the main controller. wi-fi and bluetooth le are built in, and it has i2s hardware
for audio in and out, which is what makes the mic and speaker possible.

using a **module or dev-board style s3** rather than designing the bare s3 rf
section from scratch. rf layout on a first serious pcb is a good way to end up with
a board that does not work and no way to tell why.

### 2. face — a real display
bigger than the warm-up oled. handles eyes, expressions, animations, status, and
simple text when needed.

### 3. microphone
a small digital mic on the s3. buddy hears "hey buddy", changes its face, listens,
processes, responds.

### 4. audio
`esp32-s3 → i2s amp → speaker`, instead of the warm-up piezo. that gives actual
speech and real sound rather than square-wave beeps.

### 5. motors
two geared dc motors.

```
left motor  → left wheel
right motor → right wheel
```

the main pcb needs a proper dual motor-driver section.

### 6. wheel feedback
encoder inputs, so buddy can estimate how far each wheel has turned. that's the
difference between controlled movement and "turn the motors on and hope".

### 7. obstacle sensing
a distance sensor facing forward.

```
object detected
      ↓
slow down
      ↓
stop / turn
```

this is the thing that makes buddy a robot instead of a remote-controlled box.

### 8. imu
accelerometer and gyro, so buddy knows about tilt, sudden movement, and
orientation changes.

### 9. touch
a touch sensor on buddy's head. the s3 supports capacitive touch, so this may need
no extra chip.

```
touch
 ↓
happy face
 ↓
little sound
 ↓
"hey!"
```

### 10. wireless controller
a small separate controller.

```
       joystick
          ↓
    ┌────────────┐
    │   ○        │
    │ ○       ○  │
    │   ○        │
    └────────────┘
```

joystick for movement, buttons for actions — talk, dance, come here.

### 11. power
- rechargeable battery
- power switch
- charging circuit
- regulated power for the electronics
- a separate power path for the motors

motors pull current in spikes and drag the rail down. if the logic shares that
rail, the s3 browns out and resets mid-move. this gets designed properly rather
than throwing a battery in the box.

---

## the body

not a rectangular electronics box. the reference look:

- rounded blue body
- rounded lower body, rounded head
- black front display
- side details / ears
- wheels hidden or partly integrated
- speaker openings
- microphone opening
- usb / charging opening
- removable back panel

```
             ┌─────────────┐
           /                 \
          /     DISPLAY       \
         |    •         •      |
         |                     |
          \                   /
           \_________________/
             ○           ○
              \         /
               \_______/
```

exact body gets designed in the cad week.

---

## software layers

### face system
```
idle → blink → listen → think → talk → happy / confused / etc.
```

### movement system
```
controller input → movement commands → motor controller → left + right motors
```
then later:
```
distance sensor → obstacle detected → movement adjustment
```

### voice system
```
microphone → speech recognition → ai / response logic
    → text response → speech generation → speaker
```

the s3 handles the local audio in and out. the actual conversation gets handled by
software or a service — not by me trying to train a model.

---

## what i think is risky

worth writing down now so it isn't a surprise later.

**one pcb doing everything is a lot for a first serious board.** motors, audio amp,
sensors, power and charging on one design means if the board comes back broken,
there are a dozen candidate causes and no working reference to compare against.
splitting it — a core board for brain, face, audio and sensors, and a separate
motor and power board — costs one extra board but makes a failure diagnosable.

**motors with encoders have to be chosen before the board is laid out.** the
encoder type decides how many pins are needed and whether they're interrupt
capable. picking motors late means re-routing.

**audio and motors on the same ground plane is a known noise problem.** motor
switching noise gets into the mic and the amp. this needs thinking about at layout
time, not after the first board hisses.

none of these are reasons not to do it. they're just the parts to slow down on.
