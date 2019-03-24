/*
 * ADXL355J Sample sketch -- Read 3-axis accelarations with FIFO
 */

#include <SPI.h>
#include "adxl355j.h"

const int CS1Pin = 10;
ADXL355J  acc(CS1Pin);

// Initial values for registers
#define FILTER        	(0x05)
#define POWER_CTL     	(a5POWER_CTL_DRDY_OFF | a5POWER_CTL_TEMP_OFF)
//
#define N_SAMPLES	    (5*3)    // Number of samples processed at once

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
  if (acc.getFIFOEntries() >= N_SAMPLES) {
    int samples[N_SAMPLES];
    int nSamples = acc.readFIFO(samples, N_SAMPLES);
    for (int i = 0; i < nSamples; i += 3) {
      char  line[60];
      sprintf(line, "%d,%d,%d", samples[i], samples[i+1], samples[i+2]);
      Serial.println(line);
	}
  }
  delayMicroseconds(50);
}
