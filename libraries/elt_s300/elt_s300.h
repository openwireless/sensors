/*
 * @file    elt_s300.h
 * @brief   CO2センサELT S300の制御ライブラリ
 * @author  TABrain Inc.
 * @date    04/July/2020 (R0)
 * @note    Use S300X via UART
 */

#ifndef _ELT_S300_H_
#define _ELT_S300_H_

#include <Arduino.h>

// Symbols
#define s3SERIAL_S300       Serial1
#define s3ERR_TIMEOUT       (-1)
#define s3ERR_INVALIED      (-2)

// Class
class ELT_S300 {
  public:
    void begin(void);
    void sleep(void);
    void awake(void);
    int getConcentration(uint32_t timeout = 0, boolean preClear = true);
/**
    // Calibration
    startACDL(void);
    endACDL(void);
**/
};

#endif // _ELT_S300_H_
