/*
 * ADXL362J Sample sketch -- Read 3-axis accelarations without FIFO using external clock
 */

#include <SPI.h>
#include <TimerOne.h>
#include "adxl362j.h"

const int CS1Pin = 31;		// ADXL362's chip select
const int syncPin = 3;		// External clock output

ADXL362J  acc(CS1Pin);
int   nCount = 0;

// Initial values for registers
#define FIFO_CONTROL  (a2FIFO_CONTROL_MODE_DISABLE)
#define INTMAP1       0       // All interrupts are disabled
#define INTMAP2       0       // All interrupts are disabled
#define FILTER_CTL    (a2FILTER_CTL_RANGE_2g | a2FILTER_CTL_EXT_SAMPLE | a2FILTER_CTL_HALF_BW_4 | a2FILTER_CTL_ODR_100Hz)
#define POWER_CTL     (a2POWER_CTL_LOW_NOISE_SUPER_LOW | a2POWER_CTL_MEASURE_RUN)

void setup() {
  pinMode(CS1Pin, OUTPUT);
  a2SPIx.begin();  
  delay(30);    // Wait for booting

  Serial.begin(115200);
  while (! Serial)
    ;
  Serial.println("Try to read 3-axis ACCs from the device.");

  // Generate 100Hz sync. signal
  Timer1.initialize(10000);  // 10000 us = 100 Hz
  Timer1.pwm(syncPin, 10);

  // Reset ADXL362
  acc.writeRegister(a2REG_SOFT_RESET, 0x52);
  delay(100);
  // Set up ADXL362
  acc.writeRegister(a2REG_FIFO_CONTROL, FIFO_CONTROL);
  delay(10);
  acc.writeRegister(a2REG_INTMAP1, INTMAP1);
  delay(10);
  acc.writeRegister(a2REG_INTMAP2, INTMAP2);
  delay(10);
  acc.writeRegister(a2REG_FILTER_CTL, FILTER_CTL);
  delay(10);
  acc.writeRegister(a2REG_POWER_CTL, POWER_CTL);  // Start measuring
  delay(10);
}

void loop() {
  uint8_t status = acc.readRegister(a2REG_STATUS);
  if (status & a2STATUS_DATA_READY) {
    uint32_t  us = micros();
    int8_t data[6];
    acc.readMultiRegisters(a2REG_XDATA_L, 6, (uint8_t *)data);
    int ax = acc.toRaw(data[1], data[0]);
    int ay = acc.toRaw(data[3], data[2]);
    int az = acc.toRaw(data[5], data[4]);
    if (nCount++ % 10 == 0)
      Serial.println(us);
    char  line[60];
    sprintf(line, "%d,%d,%d", ax, ay, az);
    Serial.println(line);
  }
  delayMicroseconds(10);
}
