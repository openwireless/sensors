/*
 * ADXL355J Sample sketch -- Read 3-axis accelarations with FIFO by EXT-SYNC
 */

#include <SPI.h>
#include <TimerOne.h>
#include "adxl355j.h"

#define   N_SAMPLES   3

const int CS1Pin = 10;
const int syncPin = 3;

ADXL355J  acc(CS1Pin);
int   nCount = 0;

// Initial values for registers
#define FILTER        (0x05)
#define SYNC          (a5SYNC_EXT_SYNC_EXTERNAL2)
#define POWER_CTL     (a5POWER_CTL_DRDY_OFF | a5POWER_CTL_TEMP_OFF)

void setup() {
  pinMode(CS1Pin, OUTPUT);
  a5SPIx.begin();  
  delay(30);    // Wait for booting

  Serial.begin(115200);
  while (! Serial)
    ;
  Serial.println("Try to read 3-axis ACCs from the device.");

  // Generate 100Hz sync. signal
  Timer1.initialize(10000);  // 10000 us = 100 Hz
  Timer1.pwm(syncPin, 10);

  // Reset ADXL355
  acc.writeRegister(a5REG_RESET, a5RESET_CODE);
  delay(100);

  // Set up ADXL355
  acc.writeRegister(a5REG_FILTER, FILTER);
  delay(10);
  acc.writeRegister(a5REG_SYNC, SYNC);
  delay(10);
  acc.writeRegister(a5REG_POWER_CTL, POWER_CTL);
  delay(10);
}

void loop() {
  uint8_t status = acc.readRegister(a5REG_FIFO_ENTRIES);
  if ((status & a5FIFO_ENTRIES_MASK) >= N_SAMPLES) {
    uint32_t  us = micros();
    int samples[N_SAMPLES];
    acc.readFIFO(samples, N_SAMPLES);
    if (nCount++ % 10 == 0)
      Serial.println(us);
    char  line[60];
    sprintf(line, "%d,%d,%d", samples[0], samples[1], samples[2]);
    Serial.println(line);
  }
  delayMicroseconds(10);
}

