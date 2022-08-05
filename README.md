# oled-fuel-display
Arduino based OLED Fuel level display. Originally written for 1988 Toyota Cressida (w/ Digital Gauge).

Why? because the stock cluster have a really slow refresh rate.

Can be adapted to other cars.
requires a 3-wire model fuel sender (potentiometer style / voltage divider / stock digital wiring)


### Kalibrasi (Calibration) HOWTO
- pengguna yang menggunakan bahasa indonesia, cek kalibrasi.md
- for english users who needs calibration, look at calibration.md

### Pengkabelan (Wiring) HOWTO

- A0 -> sender middle pin (purple wire from sender to behind gauge)
- 5V -> sender yellow wire
- GND -> sender green pin
- VIN -> +12v from 7.5A Gauge fuse
