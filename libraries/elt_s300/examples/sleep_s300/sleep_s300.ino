/*
 *  sample sketch for elt_s300 with sleep/awake
 */

#include <elt_s300.h>

const uint32_t interval = 5000; // CO2濃度を計測する間隔 [mS]
const int ledPin = 13;          // 状態を表すLEDのピン番号
const int buttonPin = 0;        // タクトスイッチのピン番号

ELT_S300  s300;
uint32_t last = 0;
boolean pressed = false;
boolean wakeup = true;

void setup() {
    Serial.begin(115200);
    Serial.println("Start.");

    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);

    s300.begin();
}

void loop() {
    if (digitalRead(buttonPin) == 0)
        pressed = true;

    if (millis() - last < interval)
        return;

    last = millis();

    //  ボタンが押されたときは、sleep/awakeを切り替える
    if (pressed) {
        if (wakeup) {
            // 起きている時はスリープに入る
            s300.sleep();
            wakeup = false;
            digitalWrite(ledPin, LOW);
            Serial.println("Sleep..");
        }
        else {
            // スリープしている時は起きる
            s300.awake();
            wakeup = true;
            digitalWrite(ledPin, HIGH);
            Serial.println("Wake up..");
        }
        pressed = false;
    }

    if (wakeup) {
        // 起きている時だけ、CO2濃度を計測する
        int ppm = s300.getConcentration(0, true);
        if (ppm < 0) {
            Serial.print("ERR: ");
            Serial.println(ppm);
        }
        else
            Serial.println(ppm);
    }
}
