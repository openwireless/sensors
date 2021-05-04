/*
 * ADXL362J Sample sketch -- Read IDs
 */

#include <SPI.h>
#include "adxl362j.h"

const int CS1Pin = 31;
ADXL362J  acc(CS1Pin);

void setup() {
  pinMode(CS1Pin, OUTPUT);
  a2SPIx.begin();  
  delay(30);    // Wait for booting

  Serial.begin(9600);
  while (! Serial)
    ;
  Serial.println("Try to read IDs from the device.");

  uint8_t maker_id = acc.readRegister(a2REG_DEVID_AD);
  delay(10);    
  uint8_t mems_device_id = acc.readRegister(a2REG_MST);    
  delay(10);    
  uint8_t device_id = acc.readRegister(a2REG_PARTID);    

  Serial.print("Maker id=0x");
  Serial.println(maker_id, HEX);
  Serial.print("MEMS Device id=0x");
  Serial.println(mems_device_id, HEX);
  Serial.print("Device id=0x");
  Serial.println(device_id, HEX);

  a2SPIx.end();
}

void loop() {

}
