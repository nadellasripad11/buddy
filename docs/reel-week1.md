# week 1 reel — 21.1 seconds

`media/buddy-week1-anim.mp4` · 1080x1920 · 30fps · no audio yet

## two rules this cut follows

1. **everything comes from the reference sheet.** nothing from the old breadboard
   prototype, no diagrams made up for the video.
2. **nothing claims hardware exists.** the pcb, kicad and laid-out-components
   panels are left out, because none of that has happened. what's left is the four
   planning panels, which is honestly what week 1 is.

## the cut

| time | panel | what's on it |
|---|---|---|
| 0–5.5s | concept render | the robot, "buddy — a little desk robot" |
| 5.5–11s | notebook sketches | face ideas, move / listen / talk / personality |
| 11–17s | "the plan" | block diagram + pin plan |
| 17–21.5s | hero shot | "building it piece by piece" |

this is animated, not a slideshow of stills. every shot moves:

- **beat 1** — the robot floats on a slow sine bob, the title rises and fades in,
  then the subtitle lands a beat later
- **beat 2** — the sketchbook slides in from the right on an ease-out curve, then
  keeps drifting slowly left so the frame never sits still
- **beat 3** — the plan panel travels upward, so the eye moves from the block
  diagram down to the pin plan instead of reading a static image
- **beat 4** — the robot bobs again and the two closing lines land one after the
  other rather than together

transitions are a fade, then a wipe-up into the plan (matching its upward travel),
then a fade out to the closer. the captions are drawn and animated on top rather
than baked into the panels.

## the voiceover

> i'm building a little desk robot called buddy. eventually i want it to move
> around, listen, talk back — actually feel like it has a personality. right now
> i'm still figuring out the plan: what goes inside, how the parts connect. no idea
> what the final version looks like yet. that's kind of the fun part.

55 words, lands around 21–22 seconds. where each line falls:

| beat | line |
|---|---|
| 0–5.5s | "i'm building a little desk robot called buddy." |
| 5.5–11s | "eventually i want it to move around, listen, talk back — actually feel like it has a personality." |
| 11–17s | "right now i'm still figuring out the plan: what goes inside, how the parts connect." |
| 17–21.5s | "no idea what the final version looks like yet. that's kind of the fun part." |

## to add the audio

drop the voiceover mp3 into `media/` and mux it in:

```
ffmpeg -i media/buddy-week1-silent.mp4 -i media/voiceover.mp3 \
  -c:v copy -c:a aac -b:a 192k -shortest media/buddy-week1.mp4
```

## known limitation

the source panels are small — each one is roughly 330px wide inside a 1024x1536
sheet, so getting to 1080p means upscaling about 3x. it is noticeably soft. a
blurred copy of each panel fills the background so nothing floats in black, which
hides some of it, but a higher-resolution source would look better.

## what the renders are

concept art, not hardware. the voiceover says "no idea what the final version looks
like yet", which keeps that honest. nothing in the reel should imply the blue robot
is a thing that exists.
