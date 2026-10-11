# buddy build log — hours 4–6

**hour 4 (board split):** figured out what actually lives on the PCB vs what plugs in. everything swappable: DevKitC on two female header sockets, all four breakout modules on male headers, motors and speaker on screw terminals. ended up with 11 connectors and 93 pins total.

**hour 4–5 (BOM finalization):** went through the full parts list and added all the connectors i'd just decided on — female headers for the DevKitC sockets, screw terminals for motors and speaker, motor brackets. total came out to $73.16. made one shopping list split across Amazon, Adafruit, SparkFun, and Digi-Key.

**hour 5 (placement — power zone):** blocked out zone 1 first: USB screw terminal → PTC fuse → rocker switch in a straight line across the top-left. high-current path is short and inspectable. switch mounts through the chassis wall so it's accessible without opening the bot.

**hour 5–6 (placement — DevKitC socket):** two 1×19 female sockets centered on the board. the DevKitC dominates the real estate — once i placed the sockets everything else had to fan out from the GPIO columns. not much choice there.

**hour 6 (placement — breakout headers):** matched each header to the GPIO side it talks to. J_AMP near GPIO4-6 (I²S), J_TOF near GPIO7-8 (I²C), J_DISP near GPIO9-14 (SPI2), J_MOTOR near GPIO15-18 (PWM). short traces, low noise.

**hour 6 (placement — passives + outputs):** C1 1000µF bulk cap within 10mm of the motor driver VM pin. output screw terminals for both motors and speaker along the bottom edge so wires exit the chassis cleanly. buttons and LED along the top edge to poke through the panel.

**end of hour 6:** board is fully zoned at 80×70mm, fits the JLCPCB $2 tier with room to spare. next is entering all this into KiCad and running the autorouter.
