# Notes

Mouse jiggler port from RP2040.
https://github.com/akhilharihar/pico-jiggler.git

HID report rate is not optimized.
Configured for use on SOP-8 CH32V003J4M6 as follows.

1. 2000R > LED cathode > 3.3V (optional LED #1 on PD6).
2. GND
3. 2000R > LED cathode > 3.3V (optional LED #0 on PA2).
4. 3.3V
5. 1500R > USB D-
6. 33R   > USB D-
7. 33R   > USB D+
8. Programming
