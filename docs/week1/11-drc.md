# step 11 — design rule check (DRC)

*2026-10-08 — last gate before sending gerbers*

---

## what DRC does

KiCad's DRC scans the routed PCB against a set of manufacturing rules and flags anything that will either get the board rejected by the fab or fail in real life. run it after routing, before exporting gerbers.

---

## how to run DRC in KiCad

1. open PCB editor (`buddy.kicad_pcb`)
2. menu → **Inspect → Design Rules Checker**
3. click **Run DRC**
4. work through every error in the list until it reads **0 errors, 0 warnings**
5. then export gerbers

---

## DRC rules to set first

open **File → Board Setup → Design Rules** and enter these before routing:

| rule | value |
|---|---|
| min trace width | 0.2mm |
| min clearance (trace-to-trace) | 0.2mm |
| min clearance (trace-to-via) | 0.2mm |
| min via drill | 0.3mm |
| min via annular ring | 0.25mm |
| copper to board edge | 0.5mm |
| silkscreen to pad clearance | 0.1mm |

---

## common errors and what to do

| error | what it means | fix |
|---|---|---|
| **Clearance violation** | two copper features too close | nudge one trace or reroute around |
| **Unconnected net** | a wire in the schematic has no copper path | re-run autorouter or draw the missing trace manually |
| **Footprint pad not connected** | a pad exists but nothing routes to it | check netlist import, re-annotate if needed |
| **Copper too close to board edge** | trace or pour within 0.5mm of edge | push trace inward or trim the pour |
| **Drill too small** | via drill under 0.3mm (JLCPCB minimum) | select all vias, change drill to 0.3mm |
| **Annular ring too small** | via pad minus drill less than 0.5mm total | increase via pad diameter |
| **Silkscreen over pad** | ref label printed on a solderable pad | move the ref label off the pad |
| **Short circuit** | two different nets touching | zoom in, delete the offending trace segment |
| **Missing courtyard** | footprint has no courtyard boundary | add one in footprint editor (0.25mm outside pads) |
| **Hole too close to copper** | mounting hole copper ring overlaps trace | add keepout around each mounting hole |

---

## JLCPCB-specific checks

before uploading gerbers, verify:

- [ ] board outline is on `Edge.Cuts` layer only
- [ ] gerber files include: `F.Cu`, `B.Cu`, `F.Mask`, `B.Mask`, `F.Silkscreen`, `Edge.Cuts`, drill file
- [ ] board size in gerber matches 80×70mm
- [ ] no copper within 0.3mm of board edge (their actual tolerance)
- [ ] smallest drill is ≥ 0.3mm
- [ ] smallest trace is ≥ 0.127mm (their min, we use 0.25mm so fine)

---

## exporting gerbers from KiCad

1. **File → Plot**
2. format: Gerber
3. select layers: F.Cu, B.Cu, F.Mask, B.Mask, F.Silkscreen, B.Silkscreen, Edge.Cuts
4. check **Use Protel filename extensions**
5. click **Plot**
6. click **Generate Drill Files** → **Generate Drill File**
7. zip the output folder → upload to jlcpcb.com

---

## step 11 complete ✓

next: **order placement** — upload gerbers to JLCPCB, place Amazon/Adafruit/SparkFun/Digi-Key parts orders. 2–3 week lead time.
