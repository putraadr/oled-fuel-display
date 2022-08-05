# Kalibrasi 0-100%

## Kalibrasi BBM Penuh:
  1. Isi penuh BBM sampai tumpah *(1).
  2. Pasang pin jumper D7 ke GND (short ke GND), nyalakan Arduino. *(2)
  3. selesai. sekarang posisi bbm penuh (100%) telah disimpan ke memori.

## Kalibrasi BBM Habis (Posisi "E" atau bensin dibawah 0% di tangki) :
  1. Kuras tangki sampai habis *(3)
  2. Pasang pin jumper D6 ke GND, lalu nyalakan Arduino. 
  3. selesai. sekarang posisi bbm kosong (0%) telah disimpan ke memori. 

### Catatan
1. Disarankan jangan sampai tumpah, karena pada dasarnya bensin penuh itu sampai atap tangki bukan sampai tumpah di lobang bensin
2. Jumper pin yang dimaksud bukan jumper aki. melainkan jumper breadboard female-to-female (cewe->cewe). dari D6/7 ke GND arduino
3. Pada saat menguras tangki, disarankan melakukan kuras habis. lalu isi lagi sekitar 2-5 liter sebagai **reserved fuel** 
   supaya pengguna lebih disiplin isi bensin sebelum kering tangki
