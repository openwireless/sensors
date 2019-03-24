/*
 * ADXL362J Sample sketch -- Read 3-axis accelarations with FIFO
 */

#include <SPI.h>
#include "adxl362j.h"

const int CS1Pin = 31;
ADXL362J  acc(CS1Pin);

// Initial values for registers
#define FIFO_CONTROL  (a2FIFO_CONTROL_MODE_STREAM)        // no temperature
#define INTMAP1       0                                   // All interrupts are disabled
#define INTMAP2       0                                   // All interrupts are disabled
#define FILTER_CTL    (a2FILTER_CTL_RANGE_2g | a2FILTER_CTL_HALF_BW_2 | a2FILTER_CTL_ODR_100Hz)
#define POWER_CTL     (a2POWER_CTL_LOW_NOISE_LOW | a2POWER_CTL_MEASURE_RUN)
// Other symbols
#define N_SAMPLES     (5*3)		// Sample number read at once

void setup() {
  pinMode(CS1Pin, OUTPUT);
  a2SPIx.begin();  
  delay(30);    // Wait for booting

  Serial.begin(115200);
  while (! Serial)
    ;
  Serial.println("Try to read 3-axis ACCs from the fifo.");

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
