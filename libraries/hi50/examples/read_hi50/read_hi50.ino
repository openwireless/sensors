/*
 * Sample sketch for HI50 sensor
 */
#include <iotab.h>
#include <hi50.h>

const uint32_t  interval = 5000;
int count = 0;

void setup() {
    while (! ioSerial)
        ;
    ioSerial.begin(9600);

    // Set up iotab
    if (iotab.begin() != ioSuccess) {
        ioSerial.println("iotab.begin(): error.");
        while (1) ;     // Stop here
    }

    // begin HI50 sensor
    if (hi50.begin() != 0) {
        ioSerial.println("h5begin(): error.");
        while (1) ;     // Stop here
    }

    // get id and firmware version of HI50
    char id[H5_MAX_ID_LENGTH+1], ver[H5_VERSION_LENGTH+1];
    if (hi50.getID(id, ver) != 0) {
        ioSerial.println("h5getID(): error.");
        while (1) ;     // Stop here
    }
    else {
        ioSerial.print("ID: ");
        ioSerial.println(id);
        ioSerial.print("VERSION: ");
        ioSerial.println(ver);
    }
    
    ioSerial.println("Start.");
}

void loop() {
    if (++count % 10 == 0) {
		// 10回に一度、センサの電源電圧と温度を計測して出力する
        int mV = 0, tX10 = 0;
        if (hi50.getState(mV, tX10) == 0) {
            char buf[30];
            sprintf(buf, "State: %dmV,%d.%dC", mV, (tX10 / 10), abs(tX10 % 10));
            ioSerial.println(buf);
        }
    }

    int quality = 0, dis = 0;
    if ((dis = hi50.getDistance(0, quality)) == 0) {
        ioSerial.println("h5getDistance() error.");        
    }
    else {
        char buf[30];
        sprintf(buf, "Distance: %dmm,%d", dis, quality);
        ioSerial.println(buf);
    }

    delay(interval);	// ちょっと待つ
}
