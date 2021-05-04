/*
 * @file    elt_s300.cpp
 * @brief   CO2センサELT S300の制御ライブラリ
 * @author  TABrain Inc.
 * @date    04/July/2020 (R0)
 * @note    Use S300X via UART(s3SERIAL_S300)
 */

#include "elt_s300.h"

// Symbols
#define STX             0x02            // コマンド開始コード
#define ETX             0x03            // コマンド終了コード
#define BAUDRATE_S300   38400           // S300Gのデフォルトのボーレート [bps]
#define MAX_PPM         10000           // 最大計測範囲 [ppm]
#define DEFAULT_TIMEOUT 3000            // デフォルトのタイムアウト時間 [mS]

/**
 *  @fn
 *
 *  S300の利用を開始する
 *
 *  @param              なし
 *  @return             なし
 *  @detail
 */
void ELT_S300::begin(void) {
    s3SERIAL_S300.begin(BAUDRATE_S300);
}

/**
 *  @fn
 *
 *  S300をスリープさせる
 *
 *  @param              なし
 *  @return             なし
 *  @detail
 */
void ELT_S300::sleep(void) {
    s3SERIAL_S300.write(STX);
    s3SERIAL_S300.print("SLEEP");
    s3SERIAL_S300.write(ETX);
    s3SERIAL_S300.flush();
    delay(100);
}

/**
 *  @fn
 *
 *  スリープしているS300を起こす
 *
 *  @param              なし
 *  @return             なし
 *  @detail
 */
void ELT_S300::awake(void) {
    s3SERIAL_S300.write(STX);
    s3SERIAL_S300.print("AWAKE");
    s3SERIAL_S300.write(ETX);
    s3SERIAL_S300.flush();
    delay(100);
}

/**
 *  @fn
 *
 *  CO2濃度を取得する
 *
 *  @param(timeout)     取得するまでのタイムアウト時間(0が指定されたときは3秒とする)
 *  @param(preClear)    取得する前に、シリアルバッファをクリアする
 *  @return             0以上:取得したCO2濃度[ppm]/0未満:エラーコード
 *  @detail
 */
int ELT_S300::getConcentration(uint32_t timeout, boolean preClear) {
    // 指定があれば、まずバッファをクリアする
    if (preClear) {
        while (s3SERIAL_S300.available() > 0)
            (void)s3SERIAL_S300.read();
    }

    // タイムアウトを設定する
    if (timeout == 0)
        timeout = DEFAULT_TIMEOUT;      // タイムアウトの指定がゼロの時は、デフォルトのタイムアウト値を使う
    uint32_t limit = millis() + timeout;

    // センサから値を読み出す
    while (millis() <= limit) {
        char line[50];
        int len = sizeof(line) - 1;
        if ((len = s3SERIAL_S300.readBytesUntil('\n', line, len)) == 0)
            continue;
        line[len] = '\0';

        //  line format:
        //      <SP>..<SP>9..9 ppm<CR>
        if (strlen(line) < 11)
            continue;       // Skip -- 短すぎる
        if (line[len-1] != '\r')
            continue;       // Skip -- 行末が想定外
        for (char *p = line; *p != '\0'; p++) {
            if (isdigit(*p)) {
                int ppm = atoi(p);
                return (ppm <= MAX_PPM ? ppm : s3ERR_INVALIED);
                    // おかしい値の時は、エラーを返す
            }
        }
    }

    // タイムアウトしたとき
    return (s3ERR_TIMEOUT);
}
