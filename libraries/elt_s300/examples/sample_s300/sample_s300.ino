/*
 *  sample sketch for elt_s300
 */

#include <elt_s300.h>

const uint32_t interval = 5000;
const int ledPin = 13;

ELT_S300  s300;
uint32_t last = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Start.");

    pinMode(ledPin, OUTPUT);
    s300.begin();
}

void loop() {
    if (millis() - last < interval)
        return;

    last = millis();

    digitalWrite(ledPin, HIGH);
    int ppm = s300.getConcentration(0);
    if (ppm >= 0)
        Serial.println(ppm);
    digitalWrite(ledPin, LOW);
}
