# Calibration (english howto)

## Full tank sensor/sender calibration:
  1. fill the tank until it's almost spill out (70L for a stock x7 cressida)
  2. short the Arduino D7 pin to GND, apply power to the Arduino.
  3. done. now the full tank sender position is saved to memory.

## Low fuel sensor/sender calibration:
  1. drain the tank, leave about 2-5 liters.
  2. short the Arduino D6 pin to GND, apply power to the Arduino.
  3. done. now the low fuel tank position is saved to memory.

### notes
- When calibrating low level position, ***DO NOT*** drain the tank until its dry. this is done to prevent users from driving their car until its dry
