/*
 * MMA8451Q Sample sketch -- Read 3-axis accelarations with FIFO
 */

#include <Wire.h>
#include "mma8451q.h"

// Initial settings
#define XYZ_DATA_CFG  (m8XYZ_DATA_FS_2G)
#define F_SETUP       (m8F_SETUP_F_MODE_FILL_BUF)
#define CTRL_REG1     (m8CTRL_REG1_DR_100HZ | m8CTRL_REG1_LNOISE)
#define CTRL_REG2     (m8CTRL_REG2_MODE_HIGH_RES)

#define N_SAMPLES     (5*3)    // Sample number read at once

MMA8451Q  acc(HIGH);

void setup() {
  Wire.begin();
  
  Serial.begin(115200);
  while (! Serial)
    ;

  // Check id
  uint8_t id = acc.readRegister(m8REG_WHO_AM_I);
  if (id != m8WHO_AM_I_MMA8451Q_ID) {
    Serial.print("Bad id=0x");
    Serial.println(id, HEX);
    while (1) ;
  }

  Serial.print("id=0x");
  Serial.println(id, HEX);

  // Reset MMA8451Q
  acc.writeRegister(m8REG_CTRL_REG2, m8CTRL_REG2_RST);
  while (acc.readRegister(m8REG_CTRL_REG2) & m8CTRL_REG2_RST)
    ;   // wait for ready

  // Set up MMA8451Q
  acc.writeRegister(m8REG_CTRL_REG1, CTRL_REG1);  
  delay(10);
  acc.writeRegister(m8REG_XYZ_DATA_CFG, XYZ_DATA_CFG);
  delay(10);
  acc.writeRegister(m8REG_F_SETUP, F_SETUP);
  delay(10);
  acc.writeRegister(m8REG_CTRL_REG2, CTRL_REG2);
  delay(10);
  uint8_t reg1 = acc.readRegister(m8REG_CTRL_REG1);
  acc.writeRegister(m8REG_CTRL_REG1, (reg1 | m8CTRL_REG1_ACTIVE));  // Go!  
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
  delayMicroseconds(100);
}

