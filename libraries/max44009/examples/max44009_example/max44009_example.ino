// Sample for max44009 library

#include <Wire.h>
#include <max44009.h>

MAX44009 luxsensor(LOW);

void setup()
{
    Serial.begin(9600);
    Wire.begin();
}

void loop()
{
    uint32_t lux = luxsensor.getIlluminance();

    if (lux == MAX_LUX_ERR) {
        Serial.println("getIlluminance() error");
        return;
    }

    Serial.print("lux: ");
    Serial.println(lux);
    
    delay(3000);
}

