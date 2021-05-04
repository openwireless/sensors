//
//    Sample sketch using ADS1115
//

#include <Wire.h>
#include <ads1115.h>

#define CHANNEL        0        // use AIN0

ADS1115  adc(ADS1115_I2C_ADDRESS_GND);
uint32_t interval;

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    interval = adc.getConversionDelay(CONFIG_DR_860SPS);
    uint16_t config = 0;
    config |= CONFIG_MUX_SINGLE_AIN0;
    config |= CONFIG_FS_2048;
    config |= CONFIG_MODE_CONTINUOUS;
    config |= CONFIG_DR_860SPS;
    config |= CONFIG_COMP_MODE_DEFAULT;
    config |= CONFIG_COMP_POL_DEFAULT;
    config |= CONFIG_COMP_LAT_DEFAULT;
    config |= CONFIG_COMP_QUE_DEFAULT;
    adc.configure(config);

    adc.startConversion(CHANNEL, false);
    Serial.println("Start..");
}

void loop()
{
    delayMicroseconds(interval);

    uint16_t value = adc.getLastConversionResult();
    Serial.println((int)value);
}
