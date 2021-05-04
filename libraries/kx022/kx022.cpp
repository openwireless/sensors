/*
 *	kx022.h -- KX022 control functions
 *
 *	@date		2015.01.11
 *	@author		A.D (TABrain Inc.)
 *	@note
 *				This library is control library for KX022(Kionix Tri-axis digital Accelerometer)
 */

#include <Wire.h>
#include "kx022.h"

// Constructor
KX022::KX022(uint8_t i2cAddress)
{
	_i2cAddress = i2cAddress;
}

// 
uint8_t KX022::readRegister(uint8_t reg)
{
	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(_i2cAddress, (uint8_t)1);
	return Wire.read();
}

// 
uint16_t KX022::readRegister2(uint8_t reg)
{
	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(_i2cAddress, (uint8_t)2);
	uint16_t value = Wire.read() << 8;
	return (value | Wire.read());
}

// 
int KX022::writeRegister(uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	Wire.write(value);
	return Wire.endTransmission();
}

//
int	KX022::readBuffer(uint8_t samples[], int numSamples)
{
	int i;
	for (i = 0; i < numSamples; i++) {
		Wire.beginTransmission(_i2cAddress);
		Wire.write(REGISTER_BUF_READ);
		Wire.endTransmission(false);
		Wire.requestFrom(_i2cAddress, (uint8_t)1);
		samples[i] = Wire.read();
//		delayMicroseconds(100);
	}

	return (i);
}

// end of kx022.cpp
