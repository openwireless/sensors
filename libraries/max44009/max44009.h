/*
 *	MAX44009.h
 *
 *		R1.0 - 2014.04.21 A.D
*/

#ifndef _MAX44009_H_
#define _MAX44009_H_	1

#include <Arduino.h>

// Symbols
#define	MAX_LUX_ERR   999999UL		// error value


class MAX44009
{
  public:
	MAX44009(int a0 = LOW);
	uint32_t getIlluminance(void);

  private:
	int readRegister(int addr);
	int _i2caddr;
};

#endif // _MAX44009_H_

