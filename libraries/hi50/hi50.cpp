/*
 *	hi50.h -- HI50 Sensor Control Class Library (Interface)
 *
 *	  2018/09/17 R0.1 初版リリース
 *    2018/09/22 R0.2 begin()を修正(センサの電源を入り切りしても、正しく動作するように修正)
 *
 *  Copyright(c) 2018 TABrain Inc.
 */

//#define DEBUG    // Define me when you want to debug

#include "hi50.h"

// HI50 Sensor Instance
HI50 hi50;

// begin() -- HI50センサを初期化して使用可能にする
int HI50::begin(void) {
    h5Serial.begin(19200);

	delay(200);
	while (h5Serial.available() > 0)
		(void)h5Serial.read();
	
    char resp[20];
    h5Serial.print("V\n");
    int stat = getResponse(resp, sizeof(resp)-1, 1000);
    return stat;
}

// end() -- HI50センサの使用をやめる
void HI50::end(void) {
    h5Serial.end();
}

// turnOnLaser() -- レーザーをOnにする
int HI50::turnOnLaser(void) {
    char resp[20];
    h5Serial.print("O\n");
    int stat = getResponse(resp, sizeof(resp)-1, 1000);
	if (stat != 0 || strncmp(resp, "O,OK", 4))
		return (-3);	// Open failed

    return stat;
}

// turnOffLaser() -- レーザーをOffにする
int HI50::turnOffLaser(void) {
    char resp[20];
    h5Serial.print("C\n");
    int stat = getResponse(resp, sizeof(resp)-1, 1000);
	if (stat != 0 || strncmp(resp, "C,OK", 4))
		return (-4);	// Close failed

    return stat;
}	

// getState() -- HI50センサから電源電圧と温度を取得する（いずれもあまり精度は良くない）
int HI50::getState(int &voltage, int &tempX10) {
    h5Serial.print("S\n");

    char resp[20];
    int stat = getResponse(resp, sizeof(resp)-1, 1000);
    if (stat != 0)
        return stat;

    // State format: "S:ZZ9.9`C,9.9V"
    int t10 = 0, sign = 1;
    char *p = resp + 2;
    if (*p == '-')
        sign = -1;
    while (! isdigit(*p))
        p++;   // Skip to digits of digits
    for ( ; isdigit(*p) || *p == '.'; p++) {
        if (*p == '.')
            continue;   // Skip period
        t10 *= 10;
        t10 += *p - '0';
    }
    tempX10 = t10 * sign;	// 10倍した摂氏温度を整数で返却する

    int mv = 0;
    while (! isdigit(*p))
        p++;   // Skip to digits of digits
    mv = (*p - '0') * 1000;
    mv += (*(p+2) - '0') * 100;
    voltage = mv;			// mV単位の電源電圧を整数で返却する
    
    return 0;   // OK
}

// getDistance() -- 指定された方法で距離を計測する。併せて信号品質も取得する
// [補足]
//  - 引数measurementProcessは、0:Dコマンド(お任せ)/1:Mコマンド(精度優先)/2:Fコマンド(速度優先)のいずれかを指定
//  - 信号品質は小さいほど品質が良いらしい
//  - 距離が正しく取得できなかったときは0を返す。正常に計測できる距離の範囲は、カタログスペック上では20mm～50mとのこと
int HI50::getDistance(int measurementProcess, int &quality) {
    // Send command
    switch (measurementProcess) {
      case 0 :  // Automatic measurement
        h5Serial.print("D\n");
        break;        
      case 1 :  // Slow measurement
        h5Serial.print("M\n");
        break;        
      case 2 :  // Fast measurement
        h5Serial.print("F\n");
        break;        
    }

    // Get response
    char resp[20];
    int stat = getResponse(resp, sizeof(resp)-1, 1000);
    if (stat != 0)
        return stat;

    // Parse response (format: "X:Z9.999m,9999\r\n")
    int mm = 0;
    char *p = resp + 2;
    while (! isdigit(*p))
        p++;   // Skip to digits of digits
    for ( ; isdigit(*p) || *p == '.'; p++) {
        if (*p == '.')
            continue;   // Skip period
        mm *= 10;
        mm += *p - '0';
    }

    int sq = 0;
    while (! isdigit(*p))
        p++;   // Skip to digits of digits
    for ( ; isdigit(*p); p++) {
        sq *= 10;
        sq += *p - '0';
    }
    quality = sq;
    
    return mm;   // OK - return value is positive integer
}

// getID() -- HI50センサのシリアルIDとファームウェアのバージョンを取得する
int HI50::getID(char id[], char ver[]) {
    h5Serial.print("V\n");

    char resp[20];
    int stat = getResponse(resp, sizeof(resp)-1, 1000);
    if (stat != 0)
        return stat;

    // State format: "V:99999999999,99999"
    char *p = resp + 2;
    while (*p != ',' && *p != '\0')
        *id++ = *p++;
    *id = '\0';

    p++;   // Skip to digits of digits
    while (isdigit(*p))
        *ver++ = *p++;
    *ver = '\0';

    return 0;   // OK
}

/*
 *	Private methods
 */

// getResponse() -- HI50センサからのレスポンス(1行分)を取得する
int HI50::getResponse(char resp[], int respLength, uint32_t timeout) {
    uint32_t startTime = millis();
    int n = 0;
    while (millis() - startTime < timeout) {
        if (h5Serial.available() <= 0)
            continue;
        char c = h5Serial.read();
        if (n < respLength)
            resp[n++] = c;
        if (c == '\n')
            break;      // end-of-line found    
    }

    if (millis() - startTime >= timeout)
        return (-1);    // Error - time out
        
    if (n < respLength)
        resp[n] = '\0';
    else
        resp[respLength] = '\0';

#ifdef DEBUG
    SerialUSB.print("RESPONSE>");
    SerialUSB.println(resp);
#endif

    return 0;       // OK
}
