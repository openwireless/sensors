/*
 *	ads1115.cpp -- ADS1115 control functions
 *
 *	@date		2014.12.26
 *	@author		A.D (TABrain Inc.)
 *	@note
 *				This library is control library for ADS1115(TI ADC chip)	
 */

#include "ads1115.h"
 
// Constructor
ADS1115::ADS1115(uint8_t i2cAddress)
{
	_i2cAddress = i2cAddress;
	_config =CONFIG_DEFAULT;
	_conversionDelay = getConversionDelay(CONFIG_DR_DEFAULT);
}

//	getConversionDelay() -- get conversion delay time[uS] from data rate
uint32_t ADS1115::getConversionDelay(uint16_t dataRate)
{
	switch (dataRate) {
		case CONFIG_DR_8SPS:
			return 125000UL;
		case CONFIG_DR_16SPS:
			return 62500UL;
		case CONFIG_DR_32SPS:
			return 31250UL;
		case CONFIG_DR_64SPS:
			return 15625UL;
		case CONFIG_DR_128SPS:
			return 7820UL;
		case CONFIG_DR_250SPS:
			return 4000UL;
		case CONFIG_DR_475SPS:
			return 2120UL;
		case CONFIG_DR_860SPS:
			return 1180UL;
		default:		// bad data rate
			break;
	}
	return 0UL;		// error
}

//	configure() -- Configure ADS1115 settings
int ADS1115::configure(uint16_t configValue)
{
	writeRegister(ADS1115_REGISTER_CONFIG, configValue);
	_config = configValue;
	_conversionDelay = getConversionDelay(_config & CONFIG_DR_MASK);	
}

// readSingleEnded() -- Read conversion result specified single ended channel by single-shot mode
int16_t ADS1115::readSingleEnded(int channel)
{
	uint16_t config = _config;
	if ((config & CONFIG_MODE_MASK) != CONFIG_MODE_SINGLE_SHOT) {
		// change to single-shot mode
		config = (config & ~CONFIG_MODE_MASK) | CONFIG_MODE_SINGLE_SHOT;
	}

	switch (channel) {
		case 0 :
			config = (config & ~CONFIG_MUX_MASK) | CONFIG_MUX_SINGLE_AIN0;
			break;
		case 1 :
			config = (config & ~CONFIG_MUX_MASK) | CONFIG_MUX_SINGLE_AIN1;
			break;
		case 2 :
			config = (config & ~CONFIG_MUX_MASK) | CONFIG_MUX_SINGLE_AIN2;
			break;
		case 3 :
			config = (config & ~CONFIG_MUX_MASK) | CONFIG_MUX_SINGLE_AIN3;
			break;
		default :
			return 0;		// error
	}
	config |= CONFIG_OS_BEGIN_CONVERSION;
	writeRegister(ADS1115_REGISTER_CONFIG, config);
	delayMicroseconds(_conversionDelay);		// Wait for the conversion to complete

	_config = config;	// Save last configuration

	return readRegister(ADS1115_REGISTER_CONVERSION);
}

// readDifferential0and1() -- Read conversion result specified differencial between 0 and 1 by single-shot mode
int16_t ADS1115::readDifferential0and1(void)
{
	uint16_t config = _config;
	if ((config & CONFIG_MODE_MASK) != CONFIG_MODE_SINGLE_SHOT) {
		// change to single-shot mode
		config = (config & ~CONFIG_MODE_MASK) | CONFIG_MODE_SINGLE_SHOT;
	}
	config = ((config & ~CONFIG_MUX_MASK) | CONFIG_MUX_DIFFERENTIAL_AIN01) | CONFIG_OS_BEGIN_CONVERSION;

	writeRegister(ADS1115_REGISTER_CONFIG, config);
	delayMicroseconds(_conversionDelay);		// Wait for the conversion to complete

	_config = config;	// Save last configuration

	return readRegister(ADS1115_REGISTER_CONVERSION);  
}

// readDifferential2and3() -- Read conversion result specified differencial between 2 and 3 by single-shot mode
int16_t ADS1115::readDifferential2and3(void)
{
	uint16_t config = _config;
	if ((config & CONFIG_MODE_MASK) != CONFIG_MODE_SINGLE_SHOT) {
		// change to single-shot mode
		config = (config & ~CONFIG_MODE_MASK) | CONFIG_MODE_SINGLE_SHOT;
	}
	config = ((config & ~CONFIG_MUX_MASK) | CONFIG_MUX_DIFFERENTIAL_AIN23) | CONFIG_OS_BEGIN_CONVERSION;

	writeRegister(ADS1115_REGISTER_CONFIG, config);
	delayMicroseconds(_conversionDelay);		// Wait for the conversion to complete

	_config = config;	// Save last configuration
	
	return readRegister(ADS1115_REGISTER_CONVERSION);  
}

// startConversion() -- Start specified channel conversion coninuously
void ADS1115::startConversion(uint8_t channel, boolean singleShot)
{
	uint16_t config = _config;
	if (singleShot) {
		if ((config & CONFIG_MODE_MASK) != CONFIG_MODE_SINGLE_SHOT) {
			// change to single shot mode
			config = (config & ~CONFIG_MODE_MASK) | CONFIG_MODE_SINGLE_SHOT;
			writeRegister(ADS1115_REGISTER_CONFIG, config);
		}
		config |= CONFIG_OS_BEGIN_CONVERSION;	// Begin a single shot conversion
	}
	else {
		if ((config & CONFIG_MODE_MASK) != CONFIG_MODE_CONTINUOUS) {
			// change to continuous mode
			config = (config & ~CONFIG_MODE_MASK) | CONFIG_MODE_CONTINUOUS;
		}
	}

	switch (channel) {
		case 0 :
			config = (config & ~CONFIG_MUX_MASK) | CONFIG_MUX_SINGLE_AIN0;
			break;
		case 1 :
			config = (config & ~CONFIG_MUX_MASK) | CONFIG_MUX_SINGLE_AIN1;
			break;
		case 2 :
			config = (config & ~CONFIG_MUX_MASK) | CONFIG_MUX_SINGLE_AIN2;
			break;
		case 3 :
			config = (config & ~CONFIG_MUX_MASK) | CONFIG_MUX_SINGLE_AIN3;
			break;
		default :
			return;		// error
	}
	
	writeRegister(ADS1115_REGISTER_CONFIG, config);
	_config = config;	// Save last configuration

	// Start conversion ..
}

int16_t ADS1115::getLastConversionResult(void)
{
	return readRegister(ADS1115_REGISTER_CONVERSION); 
}

// setThresholds() -- Set lower and upper thresholds
void ADS1115::setThresholds(int16_t lower, int16_t upper)
{
	writeRegister(ADS1115_REGISTER_LO_THRESH, (uint16_t)lower);	
	writeRegister(ADS1115_REGISTER_HI_THRESH, (uint16_t)upper);
}

/*
 *	Private functions
 */

void ADS1115::writeRegister(uint8_t reg, uint16_t value)
{
	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	Wire.write((uint8_t)(value >> 8));
	Wire.write((uint8_t)(value & 0xFF));
	Wire.endTransmission();
}

int16_t ADS1115::readRegister(uint8_t reg)
{
	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(_i2cAddress, (uint8_t)2);
	uint16_t value = Wire.read() << 8;	// read high byte
	value |= Wire.read() & 0xff;		// read low byte
	return value;
}

// end of ads1115.cpp
