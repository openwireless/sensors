/*
 *	ADXL355J.cpp
 *
 *	ADXL355 control library for Arduino (implementation)
 *
 *	Version:
 *		R1.0  2017.01.10
 *		R1.1  2017.02.01  add getAccelerations()
 *
 *	Note:
 *		ADXL355 is a 20bit ADC, low noise, low drift 3-Axis MEMS Accelerometer.
 *		This library support a 32bit MPU only.
 *
 *	Copyright(c) 2016-2017 A.Daikoku
 */

#include <ADXL355J.h>

/*
 *	Public methods
 */

ADXL355J::ADXL355J(uint8_t csPin) {
	_csPin = csPin;
}

uint8_t ADXL355J::readRegister(uint8_t reg) {
	a5SPIx.beginTransaction(SPISettings(a5MAX_SPI_CLOCK, MSBFIRST, SPI_MODE0));
	digitalWrite(_csPin, LOW);

	uint8_t regValue = 0;  
	a5SPIx.transfer((reg << 1) | 0x01);		// (bit0)R/nW=1
	regValue = a5SPIx.transfer(0x00);

	digitalWrite(_csPin, HIGH);
	a5SPIx.endTransaction();

	return regValue;
}

void ADXL355J::writeRegister(uint8_t reg, uint8_t data) {  
	a5SPIx.beginTransaction(SPISettings(a5MAX_SPI_CLOCK, MSBFIRST, SPI_MODE0));
	digitalWrite(_csPin, LOW);

	a5SPIx.transfer(reg << 1);				// (bit0)R/nW=0
	a5SPIx.transfer(data);

	digitalWrite(_csPin, HIGH);
	a5SPIx.endTransaction();
}

void ADXL355J::readMultiRegisters(uint8_t reg, int bytes, uint8_t data[]) {
	a5SPIx.beginTransaction(SPISettings(a5MAX_SPI_CLOCK, MSBFIRST, SPI_MODE0));
	digitalWrite(_csPin, LOW);

	a5SPIx.transfer((reg << 1) | 0x01);		// (bit0)R/nW=1
	// Burst transfer
	while (bytes-- > 0)
		*data++ = a5SPIx.transfer(0x00);

	digitalWrite(_csPin, HIGH);
	a5SPIx.endTransaction();
}

int ADXL355J::getAccelerations(int *ax, int *ay, int *az) {
    int8_t data[9];
	for (int i = 0; i < 9; i++)
		data[i] = 0;
    readMultiRegisters(a5REG_XDATA3, 9, (uint8_t *)data);
    *ax = toRaw(data[0], data[1], data[2]); 
    *ay = toRaw(data[3], data[4], data[5]); 
    *az = toRaw(data[6], data[7], data[8]); 	

	return 0;	// OK
}

int ADXL355J::readFIFO(int samples[], int numSamples) {
	a5SPIx.beginTransaction(SPISettings(a5MAX_SPI_CLOCK, MSBFIRST, SPI_MODE0));
	digitalWrite(_csPin, LOW);
	
	a5SPIx.transfer((a5REG_FIFO_DATA << 1) | 0x01); 
	int i; 
	for (i = 0; i < numSamples; i++) {
		int32_t high = a5SPIx.transfer(0x00) & 0xff;	// high byte
		uint32_t middle = a5SPIx.transfer(0x00) & 0xff;	// middle byte
		uint32_t low = ((a5SPIx.transfer(0x00) & 0xf0) >> 4) & 0x0f;	// low byte
		int raw = (high << 12) + (middle << 4) + low;
		if (high & 0x80)
			raw |= 0xfff00000;
		samples[i] = raw;
	}

	digitalWrite(_csPin, HIGH);
	a5SPIx.endTransaction();

	return (i);
}

// End of ADXL355J.cpp
