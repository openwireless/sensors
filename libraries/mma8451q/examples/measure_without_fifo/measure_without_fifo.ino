/*
 * MMA8451Q Sample sketch -- Read 3-axis accelarations without FIFO
 */

#include <Wire.h>
#include "mma8451q.h"

// Initial settings
#define XYZ_DATA_CFG  (m8XYZ_DATA_FS_2G)
#define CTRL_REG1     (m8CTRL_REG1_DR_100HZ | m8CTRL_REG1_LNOISE)
#define CTRL_REG2     (m8CTRL_REG2_MODE_HIGH_RES)

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

  // Reset MMA8451Q
  acc.writeRegister(m8REG_CTRL_REG2, m8CTRL_REG2_RST);
  while (acc.readRegister(m8REG_CTRL_REG2) & m8CTRL_REG2_RST)
    ;   // wait for ready

  // Set up MMA8451Q
  acc.writeRegister(m8REG_XYZ_DATA_CFG, XYZ_DATA_CFG);
  delay(10);
  acc.writeRegister(m8REG_CTRL_REG1, CTRL_REG1);
  delay(10);
  acc.writeRegister(m8REG_CTRL_REG2, CTRL_REG2);
  delay(10);
  uint8_t reg1 = acc.readRegister(m8REG_CTRL_REG1);
  reg1 |= m8CTRL_REG1_ACTIVE;
  acc.writeRegister(m8REG_CTRL_REG1, reg1);    // and Go!  
}

void loop() {
  uint8_t status = acc.readRegister(m8REG_STATUS);
  if (status & m8STATUS_ZYXD) {
    uint32_t  ts = micros();
    uint8_t data[6];
    acc.readMultiRegisters(m8REG_OUT_X_MSB, 6, data);
    int ax = acc.toRaw(data[0], data[1]);
    int ay = acc.toRaw(data[2], data[3]);
    int az = acc.toRaw(data[4], data[5]);
    char  line[60];
    sprintf(line, "%d,%d,%d", ax, ay, az);
    Serial.println(line);
  }
  delayMicroseconds(20);
}
