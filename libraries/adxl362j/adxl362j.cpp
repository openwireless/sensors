/*
 *	ADXL362J.cpp
 *
 *	ADXL362 control library for Arduino (implementation)
 *
 *	Version:
 *		R1.0  2016.05.15
 *		R1.1  2017.01.03
 *		R1.2  2017.01.10  change readFIFO() interface
 *		R1.3  2017.02.04  add getAccelerations()
 *
 *	Note:
 *		ADXL362 is a ultra low power 3-Axis MEMS Accelerometer.
 *		This library support a 32bit MPU only.
 *
 *	Copyright(c) 2016-2017 A.Daikoku
 */

#include <ADXL362J.h>

/*
 *	Public methods
 */

ADXL362J::ADXL362J(uint8_t csPin) {
	_csPin = csPin;
//	SPI.setClockDivider(SPI_CLOCK_DIV64);
}

uint8_t ADXL362J::readRegister(uint8_t reg) {
    a2SPIx.beginTransaction(SPISettings(a2MAX_SPI_CLOCK, MSBFIRST, SPI_MODE0));
	digitalWrite(_csPin, LOW);

	uint8_t regValue = 0;  
	a2SPIx.transfer(0x0B);  	// read register command
	a2SPIx.transfer(reg);
	regValue = a2SPIx.transfer(0x00);

	digitalWrite(_csPin, HIGH);
	a2SPIx.endTransaction();	

	return regValue;
}

void ADXL362J::writeRegister(uint8_t reg, uint8_t data) {  
    a2SPIx.beginTransaction(SPISettings(a2MAX_SPI_CLOCK, MSBFIRST, SPI_MODE0));
	digitalWrite(_csPin, LOW);

	a2SPIx.transfer(0x0A);  // write register command
	a2SPIx.transfer(reg);
	a2SPIx.transfer(data);

	digitalWrite(_csPin, HIGH);
	a2SPIx.endTransaction();
}

void ADXL362J::readMultiRegisters(uint8_t reg, int bytes, uint8_t data[]) {
    a2SPIx.beginTransaction(SPISettings(a2MAX_SPI_CLOCK, MSBFIRST, SPI_MODE0));
	digitalWrite(_csPin, LOW);

	a2SPIx.transfer(0x0B);  // read register command
	a2SPIx.transfer(reg);
	// Burst transfer
	while (bytes-- > 0)
		*data++ = a2SPIx.transfer(0x00);

	digitalWrite(_csPin, HIGH);
	a2SPIx.endTransaction();
}

int ADXL362J::getAccelerations(int *ax, int *ay, int *az) {
	while ((readRegister(a2REG_STATUS) & a2STATUS_DATA_READY) == 0)
		;		// Wait until data are ready

	uint8_t data[6];
	readMultiRegisters(a2REG_XDATA_L, 6, data);
	*ax = toRaw(data[1], data[0]);
	*ay = toRaw(data[3], data[2]);
	*az = toRaw(data[5], data[4]);

	return 0;	// OK
}

int ADXL362J::readFIFO(int samples[], int numSamples) {
    a2SPIx.beginTransaction(SPISettings(a2MAX_SPI_CLOCK, MSBFIRST, SPI_MODE0));
	digitalWrite(_csPin, LOW);

	a2SPIx.transfer(0x0D);  	// read fifo command
	int i = 0; 
	while (i < numSamples) {
		int low = a2SPIx.transfer(0x00) & 0xff;		// lower byte
		int high = a2SPIx.transfer(0x00) & 0xff;	// high byte with data type(b7-6)

		if (low == 0 && (high & 0x0f) == 0)
			continue;	// retry when can't read data

		int xyz = i % 3;
		int axis = high & 0xc0;
		switch (xyz) {
		case 0 :	// X-axis data
			if (axis != 0x00)
				continue;
			break;
		case 1 :	// Y-axis data
			if (axis != 0x40)
				continue;
			break;
		case 2 :	// Z-axis data
			if (axis != 0x80)
				continue;
			break;
		default :	// Unknown data
			continue;
		}

		int raw = ((high << 8) + low) & 0x0fff;
		if (high & 0x08)
			raw |= 0xfffff000;    // extent sign
		samples[i++] = raw;
	}

	digitalWrite(_csPin, HIGH);
	a2SPIx.endTransaction();

	return (i);
}

// End of ADXL362Jj.cpp
