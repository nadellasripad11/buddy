# week 1 reel — 22.2 seconds

`media/buddy-week1.mp4` · 1080x1920 · 30fps · elevenlabs voiceover · burned-in captions

## two rules this cut follows

1. **everything visual comes from the reference sheet.** nothing from the old
   breadboard prototype, no diagrams invented for the video.
2. **nothing claims hardware exists.** the pcb, kicad and laid-out-components panels
   are left out, because none of that has happened.

## the shots — six, none repeated

| time | shot | motion |
|---|---|---|
| 0.0–3.2s | robot, close | push in + slow bob, title drops in at the top |
| 3.2–7.7s | the sketch page | slides in from the right on an ease-out, then drifts |
| 7.7–10.6s | "what it should eventually do" | move / listen / talk / personality land one at a time |
| 10.6–13.9s | the block diagram | slow push in |
| 13.9–17.6s | the pin plan | drifts upward |
| 17.6–22.2s | the wide hero shot | slow pull back |

the robot appears in the first and last shot, but as two different source crops and
two different framings — a tight close-up to open, the wide desk shot to close.
nothing else repeats. shot 3 is drawn from scratch rather than being a panel.

## the voiceover

elevenlabs, voice **Anagh – Introspective Narration**, 22.24s.
`media/voiceover.mp3`.

> i'm building a little desk robot called buddy. eventually i want it to move
> around, listen, talk back — actually feel like it has a personality. right now
> i'm still figuring out the plan: what goes inside, how the parts connect. no idea
> what the final version looks like yet. that's kind of the fun part.

## captions

`media/captions.ass`, burned in. the cut points are not guesses — i ran
`silencedetect` over the voiceover to find where the speech actually pauses, and
set every caption and every shot change to those timings:

```
2.30  3.24   after "called buddy."
9.87  10.69  after "a personality."
13.11 13.92  after "the plan:"
16.83 17.62  after "how the parts connect."
19.95 20.72  after "looks like yet."
```

## known limitations

**the source is low resolution.** each panel is roughly 330px wide inside a
1024x1536 sheet, so reaching 1080p means upscaling about 3x. it is soft. a blurred
copy of each panel fills the background so nothing floats in black, which hides
some of it.

**no ai video clips.** generating actual footage needs a paid elevenlabs plan, so
every shot is built from the reference sheet plus motion design.

**the pin plan on screen is the warm-up's.** it shows a plain esp32 with an oled,
buttons and a buzzer. the real buddy is an esp32-s3 with motors, i2s audio and
sensors — see [00-vision.md](00-vision.md). worth redrawing before the week 2 reel.

## the renders are concept art

not hardware. the voiceover says "no idea what the final version looks like yet",
which keeps that honest. nothing in the reel should imply the blue robot exists.
