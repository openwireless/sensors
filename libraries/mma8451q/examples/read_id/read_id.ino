/*
 * MMA8451Q Sample sketch -- Read "Who am I"
 */

#include <Wire.h>
#include "mma8451q.h"

MMA8451Q  acc(LOW);

void setup() {
  Wire.begin();
  
  Serial.begin(9600);
  while (! Serial)
    ;
  Serial.println("Try to read ID from the device.");
  
  uint8_t id = acc.readRegister(m8REG_WHO_AM_I);

  Serial.print("id=0x");
  Serial.println(id, HEX);

  Wire.end();
}

void loop() {
}

