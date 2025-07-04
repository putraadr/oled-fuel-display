// Code and design by Putra Adriansyah.
// Maret 2022
// Designed and optimalized for 3 wire potentiometer type fuel sender 
// first implemented for a 1988 Toyota Cressida GX71, but you can change the code however you like

// CALIBRATION :
// Empty your fuel tank or remove the sender
// set it to empty by moving the fuel float to 10% above the bottom end stop
// pull D6 to LOW (short it to GND), power up your arduino. This will save the value to EEPROM

// remove D6 jumper, move the fuel float to the top of the sender, pull D7 to LOW and power 
// your arduino. this will save the 100% value in the EEPROM

// change tankCapacity to match your fuel tank full capacity, mine is 70L 

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

// Display configuration stuff
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// variables and settings
int fuel_sender=A0;
int low_indicator=13;
int low_fuel_calibrate=6;
int full_fuel_calibrate=7;

int calibration_full_value_address=20;
int calibration_empty_value_address=30;

int sisa_liter_bensin;
int sisa_persen_bensin;
int refresh_rate=250;

int tankCapacity=70;

int splash_screen=0; // 0 to disable, 1 to splash screen, 2 to DEBUG Splash screen

void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}
int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

int sensor_minValue=readIntFromEEPROM(calibration_empty_value_address);
int sensor_maxValue=readIntFromEEPROM(calibration_full_value_address);

// 'cressida_logo', 128x64px
const unsigned char splash_cressida_logo [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x02, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x02, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x80, 0x02, 0x00, 0x02, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x02, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x02, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x12, 0x10, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x1e, 0x60, 0x01, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x08, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x30, 0x1c, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x0c, 0x60, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x18, 0x30, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xfa, 0x00, 0x32, 0x08, 0x00, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xfa, 0x00, 0x42, 0x04, 0x00, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xfa, 0x00, 0x82, 0x02, 0x00, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xfa, 0x00, 0x02, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xfa, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xfa, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xfa, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xfa, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xfd, 0x00, 0x00, 0x00, 0x01, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x60, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xb8, 0x00, 0x00, 0x3b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfd, 0xe0, 0x0f, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xa0, 0x0b, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xf0, 0x1f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)
const int splash_allArray_LEN = 1;
const unsigned char* splash_allArray[1] = {
  splash_cressida_logo
};



void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 Allocation Failed"));
    for(;;);
  }
  // Pin Setup
  pinMode(low_indicator, OUTPUT);
  pinMode(low_fuel_calibrate, INPUT_PULLUP);
  pinMode(full_fuel_calibrate, INPUT_PULLUP);

  // Detect calibration mode
  int lowCalibPin = digitalRead(low_fuel_calibrate);
  int fullCalibPin = digitalRead(full_fuel_calibrate);
  
  if (lowCalibPin == LOW){
    lowCalibrationMode();
  }
  if (fullCalibPin == LOW){
    fullCalibrationMode();
  }

  // Splash Screen
  if (splash_screen == 1){
    mainSplash();
  } else if (splash_screen == 2){
    debugSplash();
  }

}

void loop() {
  int sensorValue = analogRead(fuel_sender);
  int fuel_remaining = map(sensorValue, sensor_minValue, sensor_maxValue, 0, 100);
  int fuel_bar = map(fuel_remaining, 0, 100, 0, 123);
  
  sisa_persen_bensin = fuel_remaining;
  count_sisa_liter_bensin(fuel_remaining);

  // OLED Display stuff
  display.clearDisplay();
  display.setCursor(1,1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Fuel Remaining : ");
  display.print("  ");
  display.setTextSize(1);
  display.print(sisa_liter_bensin);
  display.print("L");
  display.drawRect(1, 17, 127, 30, WHITE);
  display.fillRect(3, 19, fuel_bar, 26, WHITE);
  display.setCursor(10, 50);
  display.setTextSize(2);
  display.print(fuel_remaining);
  display.print("% ");
Serial.println(sensorValue);

  // LOW Fuel indicator (set to 15%)
  if (fuel_remaining < 16){
    display.setTextSize(2);
    display.print("(E!)");
    digitalWrite(low_indicator, HIGH);
  } 

  /* Sensor out of range or broken sensor 
     detection. stop operation if sensor out of calibration
  */
  if (sensorValue < sensor_minValue) { 
    haltError(sensorValue);
  } else if (sensorValue > sensor_maxValue) {
    haltError(sensorValue);
  }

  //Serial.print("RAW Value: ");
  
  //Serial.print("Liters left: ");
  //Serial.println(sisa_liter_bensin);
  display.display();
  delay(refresh_rate);
  digitalWrite(low_indicator, LOW);

}

float count_sisa_liter_bensin(int val_input){
  sisa_liter_bensin = val_input * tankCapacity / 100;
  return sisa_liter_bensin;
}

void haltError(int last_sensor_value){

  display.clearDisplay();
  display.setCursor(1,1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("   [ ERROR 001 ]");
  display.setCursor(1, 16);
  display.println("SENSOR OUT OF RANGE");
  display.println();
  display.println("Check fuel level");
  display.println("sensor wiring or");
  display.println("replace fuel level");
  display.print("sensor. val:");
  display.println(last_sensor_value);
  display.display();
  for(;;);
}

void lowCalibrationMode(){
  int analogVal = analogRead(fuel_sender);
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(1,1);
  display.println("CALIBRATION MODE");
  display.setCursor(1, 16);
  display.println("Low tank \ncalibration\n");
  display.print("AO Value: ");
  display.println(analogVal);
  display.println("WAIT 5 SECONDS");
  display.display();
  writeIntIntoEEPROM(calibration_empty_value_address, analogVal);
  delay(5000);
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(1,1);
  display.println("CALIBRATION MODE");
  display.setCursor(1, 16);
  display.println("completed.\n");
  display.print("Value saved to\nEEPROM: ");
  display.println(readIntFromEEPROM(calibration_empty_value_address));
  display.display();
  for(;;);
}

void fullCalibrationMode(){
  int analogVal = analogRead(fuel_sender);
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(1,1);
  display.println("CALIBRATION MODE");
  display.setCursor(1, 16);
  display.println("Full tank \ncalibration\n");
  display.print("AO Value: ");
  display.println(analogVal);
  display.println("WAIT 5 SECONDS");
  display.display();
  writeIntIntoEEPROM(calibration_full_value_address, analogVal);
  delay(5000);
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(1,1);
  display.println("CALIBRATION MODE");
  display.setCursor(1, 16);
  display.println("completed.\n");
  display.print("Value saved to\nEEPROM: ");
  display.println(readIntFromEEPROM(calibration_full_value_address));
  display.display();
  for(;;);
}

void debugSplash(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(1,1);
  display.println("DEBUG Startup");
  display.setCursor(1,16);
  display.print("LowValue: ");
  display.println(readIntFromEEPROM(calibration_empty_value_address));
  display.print("FullValue: ");
  display.println(readIntFromEEPROM(calibration_full_value_address));
  display.display();
  delay(5000);
}

void mainSplash(){
  display.clearDisplay();
  display.drawBitmap(0, 0, splash_cressida_logo, 128, 64, 1);
  display.setCursor(1, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Loading");
  display.setCursor(3, 55);
  display.println("[ Toyota  CRESSIDA ]");
  display.display();
  delay(2500);
}
