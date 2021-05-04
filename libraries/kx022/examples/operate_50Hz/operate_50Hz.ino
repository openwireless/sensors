// KX022 test sketch -- operate accelerometer

#include <Wire.h>
#include <kx022.h>

const int buttonPin = 2;

KX022  kx022(KX022_I2C_ADDR_GND);
uint16_t xOut, yOut, zOut;
int xOffset, yOffset, zOffset;

void setup() {
  delay(2000);

  Serial.begin(115200);  // start serial for output
  Serial.println("OK.");

  pinMode(buttonPin, INPUT_PULLUP);

  Wire.begin();        // join i2c bus (address optional for master)
  kx022.writeRegister(REGISTER_CNTL1, 0x40);  // 16bit and +/-2g
  kx022.writeRegister(REGISTER_ODCNTL, 0x02); // 50Hz
  kx022.writeRegister(REGISTER_CNTL1, 0xC0);  // enter operating mode
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    // Calibrate x/y/z accelearometer
    xOffset = - (int)xOut;
    yOffset = - (int)xOut;
    zOffset = - (int)xOut;
  }

  uint16_t data = kx022.readRegister(REGISTER_XOUTH) << 8;
  xOut = data | kx022.readRegister(REGISTER_XOUTL);
  data = kx022.readRegister(REGISTER_YOUTH) << 8;
  yOut = data | kx022.readRegister(REGISTER_YOUTL);
  data = kx022.readRegister(REGISTER_ZOUTH) << 8;
  zOut = data | kx022.readRegister(REGISTER_ZOUTL);

  Serial.print(((int)xOut+xOffset) / 16384.0);
  Serial.print(',');
  Serial.print(((int)yOut+yOffset) / 16384.0);
  Serial.print(',');
  Serial.println(((int)zOut+zOffset) / 16384.0);

  delay(80);
}
