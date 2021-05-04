/*
 * Sample sketch for TWE-Lite and I2C LCD
 * 
 * Hardware Configuration:
 *      Arduino UNO etc..
 *      LCD .. use AE-AQM0802(w/ Pull-up SDA and SCL)  http://akizukidenshi.com/catalog/g/gK-06795/
 *      TWE-Lite(Parent) .. use Customized "Samp_Monitor" and connect UNO via Serial
 *      TWE-Lite(Children) .. use "Samp_Monitor" with SHT-21, TLS2561, LM61BIZ or YF-S201
 */

#include <Wire.h>
#include <LCD_I2C.h>
#include <twelite.h>

TWELite twelite(Serial);

void setup() {
    // Set up i2c and lcd
    Wire.begin();
    LCD.begin(30, true); // specify contrast

    // Set up TWE-Lite
    twelite.begin(115200);  

    // Put welcome message
    displayLCD(true, "Sample", "TWE-Lite");
    delay(3000);
}

void loop() {
    if (! twelite.hasReceivedMessages())
        return;     // No message from my children

    displayLCD(true, ">MSG", "");        

    if (twelite.parseMessage() != tweOK) {
        // when unknown format message has received, clear Serial buffer and ignore it
        displayLCD(true, "TWE Lib", "BAD MSG");
        twelite.discardMessages();
        return;     // Ignore unknown format message
    }

    uint8_t id = twelite.getChildID();
    uint8_t type = twelite.getChildType();
    char line1[10], line2[10];
    int tX100, hX100, di, wf;
    long il;
    switch (type) {
        case CTYPE_TANDH:
            tX100 = twelite.getTemperature();     // Temperature [x100 C]
            hX100 = twelite.getHumidity();           // Humidity [x100 %]
            sprintf(line1, "T: %d.%d", (tX100 / 100), abs(tX100 % 100));
            sprintf(line2, "H: %d.%d", (hX100 / 100), abs(hX100 % 100));
            break;
        case CTYPE_IL:
            il = twelite.getIlluminance();             // Illuminance [x1 lux]
            sprintf(line1, "I: %ld", il);
            strcpy(line2, "");
            break;
		case CTYPE_T:
            tX100 = twelite.getTemperature2();    // Temperature [x100 C]
            di = twelite.getDigitalInput();          // DIN
            sprintf(line1, "T: %d.%d", (tX100 / 100), abs(tX100 % 100));
            sprintf(line2, "DI: %02x", di);
			break;
		case CTYPE_WF:
			wf = twelite.getWaterFlow();
            di = twelite.getDigitalInput();          // DIN
            sprintf(line1, "W: %d", wf);
            sprintf(line2, "DI: %02x", di);
			break;
        default:
			sprintf(line2, "%d %d", id, type);
            displayLCD(true, "MSG ERR:", line2);
            return;
    }
    displayLCD(true, line1, line2);
    delay(2000);

    int mV = twelite.getBatteryVoltage();         // Battery voltage [mV]
    int rssi = twelite.getRSSI();                      // RSSI [dBm]
    sprintf(line1, "V:%d", mV);
    sprintf(line2, "R:%d", rssi);
    displayLCD(true, line1, line2);
    delay(2000);
}

// displayLCD() -- display two lines on LCD(8chars * 2 lines)
void displayLCD(boolean preClear, char const *line1, char const *line2) {
    if (preClear)
        LCD.clear();
    LCD.move(0);
    LCD.print(line1);
    LCD.move(0x40);
    LCD.print(line2);
}

