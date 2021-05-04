//
//    Sample sketch using ADS1115 with ALERT/RDY interrupt
//

#include <Wire.h>
#include <ads1115.h>

#define CHANNEL       	0   // use AIN0
#define	INTERRUPT_NO	0	// use INT0(D2) as ALERT/RDY

ADS1115  adc(ADS1115_I2C_ADDRESS_GND);
volatile boolean	doneConversion = false;

void onInterrupt0()
{
    doneConversion = true;
}

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    uint16_t config = 0;
    config |= CONFIG_MUX_SINGLE_AIN0;
    config |= CONFIG_FS_4096;
    config |= CONFIG_MODE_SINGLE_SHOT;
    config |= CONFIG_DR_16SPS;
    config |= CONFIG_COMP_MODE_DEFAULT;
    config |= CONFIG_COMP_POL_DEFAULT;
    config |= CONFIG_COMP_LAT_DEFAULT;
    config |= CONFIG_COMP_QUE_ASSERT_ONE;	// use ALERT/RDY
    adc.configure(config);

    pinMode(2, INPUT_PULLUP);
    attachInterrupt(INTERRUPT_NO, onInterrupt0, FALLING);

    Serial.println("Start..");

    adc.setThresholds(0, 0x8000);
    adc.startConversion(CHANNEL, true);
}

void loop()
{
    if (doneConversion) {
        doneConversion = false;
        uint16_t value = adc.getLastConversionResult();
        Serial.println((int)value);

        adc.startConversion(CHANNEL, true);	// restart single-shot conversion		
    }

    Serial.print(".");
}
