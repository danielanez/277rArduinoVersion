#include <Wire.h>
#include "rom_data.h"

#define EEPROM_ADDR 0x50  // 24LC32A with A0/A1/A2 = GND

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire.begin();

  Serial.println("I2C EEPROM test starting...");

  // ---- WRITE TEST ----
  uint16_t addr = 0x001E;   // test address
  byte value = 0xAA;        // test pattern

  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(highByte(addr));  // address MSB
  Wire.write(lowByte(addr));   // address LSB
  Wire.write(value);
  byte err = Wire.endTransmission();

  if (err != 0) {
    Serial.print("WRITE failed, I2C error = ");
    Serial.println(err);
    while (1);
  }

  delay(10); // EEPROM write cycle time

  // ---- READ TEST ----
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(highByte(addr));
  Wire.write(lowByte(addr));
  err = Wire.endTransmission();

  if (err != 0) {
    Serial.print("ADDRESS SET failed, I2C error = ");
    Serial.println(err);
    while (1);
  }

  Wire.requestFrom(EEPROM_ADDR, 1);
  if (Wire.available()) {
    byte readValue = Wire.read();

    Serial.print("Read value at 0x");
    Serial.print(addr, HEX);
    Serial.print(" = 0x");
    Serial.println(readValue, HEX);

    if (readValue == value) {
      Serial.println("SUCCESS: EEPROM communication works.");
    } else {
      Serial.println("FAIL: Value mismatch.");
    }
  } else {
    Serial.println("FAIL: No data returned.");
  }
}

void loop() {
}