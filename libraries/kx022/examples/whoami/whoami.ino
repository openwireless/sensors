// KX022 test sketch -- who am i

#include <Wire.h>
#include <kx022.h>

KX022  kx022(KX022_I2C_ADDR_GND);

void setup() {
  delay(2000);
  Serial.begin(9600);  // start serial for output
  Serial.println("OK.");

  Wire.begin();        // join i2c bus (address optional for master)
}

void loop() {
  uint8_t data = kx022.readRegister(REGISTER_WHO_AM_I);
  Serial.print("id=");
  Serial.println(data, BIN); 

  delay(1000);
}
