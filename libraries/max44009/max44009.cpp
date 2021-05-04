/*
 *	MAX44009.cpp
 *
 *		R1.0 - 2014.04.21 A.D
*/

#include <Wire.h>
#include "max44009.h"

// Constructor
MAX44009::MAX44009(int a0)
{
	if (a0 == HIGH)
		_i2caddr = 0x4b;
	else
		_i2caddr = 0x4a;
}

// getIlluminance() -- Get illuminance by MAX44009
uint32_t MAX44009::getIlluminance()
{
	int luxHigh = readRegister(0x03);
	int luxLow = readRegister(0x04); 

	if (luxHigh < 0 || luxLow < 0)
		return MAX_LUX_ERR;		// error!

	int exponent = (luxHigh & 0xf0) >> 4;
	int mant = ((luxHigh & 0x0f) << 4) | (luxLow & 0x0f);

	return ((pow(2, exponent) * mant) * 0.045);
}

int MAX44009::readRegister(int addr)
{
	Wire.beginTransmission(_i2caddr);
	Wire.write(addr);
	Wire.endTransmission();
	Wire.requestFrom(_i2caddr, 1);

	for (int n = 0; n < 25; n++) {
		if (Wire.available())
			return Wire.read();  // Ok
		delay(100);
	}

	return (-1);  // Timeout error
}

// END OF max44009.cpp
