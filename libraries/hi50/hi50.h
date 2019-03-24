/*
 *	hi50.h -- HI50 Sensor Control Class Library (Interface)
 *
 *	  2018/09/17 R0.1 初版リリース
 *    2018/09/22 R0.2 begin()を修正(センサの電源を入り切りしても、正しく動作するように修正)
 *
 *  Copyright(c) 2018 TABrain Inc.
 */

#ifndef _HI50_
#define _HI50_

#include <Arduino.h>

// Symbols
  // シリアルポートの定義
#define h5Serial				Serial1
#define debugSerial			ioSerial
  // 最大定数の定義
#define H5_MAX_ID_LENGTH	15		// HI50センサのシリアルIDの最大桁数（実際は11桁）
#define H5_VERSION_LENGTH	6		// HI50センサのファームウェアのバージョン番号の最大桁数（実際は5桁）

// Class
class HI50 {
  public:
	int begin(void);
	void end(void);
	int turnOnLaser(void);
	int turnOffLaser(void);
	int getState(int &voltage, int &tempX10);
	int getDistance(int measurementProcess, int &quality);
	int getID(char id[], char ver[]);

  private:
	int getResponse(char resp[], int respLength, uint32_t timeout);
};

extern HI50		hi50;

#endif // _HI50_ 
