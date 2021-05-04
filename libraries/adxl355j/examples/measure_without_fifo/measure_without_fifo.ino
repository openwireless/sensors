/*
 * ADXL355J Sample sketch -- Read 3-axis accelarations without FIFO
 */

#include <SPI.h>
#include "adxl355j.h"

const int CS1Pin = 10;
ADXL355J  acc(CS1Pin);

// Initial values for registers
#define FILTER        (0x05)
#define POWER_CTL     (a5POWER_CTL_DRDY_OFF | a5POWER_CTL_TEMP_OFF)
//
#define MG_PER_LSB    2

void setup() {
  pinMode(CS1Pin, OUTPUT);
  a5SPIx.begin();  
  delay(30);    // Wait for booting

  Serial.begin(115200);
  while (! Serial)
    ;
  Serial.println("Try to read 3-axis ACCs from the device.");

  // Reset ADXL355
  acc.writeRegister(a5REG_RESET, a5RESET_CODE);
  delay(100);

  // Set up ADXL355
  acc.writeRegister(a5REG_FILTER, FILTER);
  delay(10);
  acc.writeRegister(a5REG_POWER_CTL, POWER_CTL);
  delay(10);
}

void loop() {
  uint8_t status = acc.readRegister(a5REG_STATUS);
  if (status & a5STATUS_DATA_RDY) {
    uint32_t  ts = millis();
    int8_t data[9];
    acc.readMultiRegisters(a5REG_XDATA3, 9, (uint8_t *)data);
    int ax = acc.toRaw(data[0], data[1], data[2]); 
    int ay = acc.toRaw(data[3], data[4], data[5]); 
    int az = acc.toRaw(data[6], data[7], data[8]); 
    char  line[60];
    sprintf(line, "%ld,%d,%d,%d", ts, ax, ay, az);
//    sprintf(line, "%d,%d,%d", ax, ay, az);
    Serial.println(line);
  }
  delayMicroseconds(500);
}
