/*
 *	ads1115.h -- ADS1115 control functions
 *
 *	@date		2014.12.26
 *	@author		A.D (TABrain Inc.)
 *	@note
 *				This library is control library for ADS1115(TI ADC chip)	
 */

#ifndef	_ADS1115_H_
#define	_ADS1115_H_		1

#include <Arduino.h>
#include <Wire.h>

// ADS1115 I2C Address - selectable by ADDR pin
#define	ADS1115_I2C_ADDRESS_GND				0x48		// ADDR pin -> GND
#define	ADS1115_I2C_ADDRESS_VDD				0x49		// ADDR pin -> VDD
#define	ADS1115_I2C_ADDRESS_SDA				0x4a		// ADDR pin -> SDA
#define	ADS1115_I2C_ADDRESS_SCL				0x4b		// ADDR pin -> SCL

// ADS1115 REGISTER Address
#define	ADS1115_REGISTER_CONVERSION			0x00		// Conversion data 
#define	ADS1115_REGISTER_CONFIG				0x01		// Configuration
#define	ADS1115_REGISTER_LO_THRESH			0x02		// Threshold value(low byte)
#define	ADS1115_REGISTER_HI_THRESH			0x03		// Threshold value(high byte)

// CONFIG REGISTER Symbols
	// Default config value
#define	CONFIG_DEFAULT						(CONFIG_OS_DEFAULT | CONFIG_MUX_DEFAULT | CONFIG_FS_DEFAULT | CONFIG_MODE_DEFAULT | CONFIG_DR_DEFAULT | CONFIG_COMP_MODE_DEFAULT | CONFIG_COMP_POL_DEFAULT | CONFIG_COMP_LAT_DEFAULT | CONFIG_COMP_QUE_DEFAULT)
	// OS[15] - Operational status/single-shot conversion start
#define	CONFIG_OS_MASK						0x8000	// OS Bit mask
		// for write status
#define	CONFIG_OS_NO_EFFECT					0x0000	// No effect
#define	CONFIG_OS_BEGIN_CONVERSION			0x8000	// Begin a single conversion (when in power-down mode)
#define	CONFIG_OS_DEFAULT					CONFIG_OS_NO_EFFECT
		// for read status
#define	CONFIG_OS_PERFORMING				0x0000	// Device is currently performing a conversion
#define	CONFIG_OS_IDLING					0x8000	// Device is not currently performing a conversion 
	// MUX[14:12] - Input multiplexer configuration
#define	CONFIG_MUX_MASK						0x7000	// MUX Bit mask
#define	CONFIG_MUX_DIFFERENTIAL_AIN01		0x0000	// AIN0(p)/AIN1(n)  -- default
#define	CONFIG_MUX_DIFFERENTIAL_AIN03		0x1000	// AIN0(p)/AIN3(n)
#define	CONFIG_MUX_DIFFERENTIAL_AIN13		0x2000	// AIN1(p)/AIN3(n)
#define	CONFIG_MUX_DIFFERENTIAL_AIN23		0x3000	// AIN2(p)/AIN3(n)
#define	CONFIG_MUX_SINGLE_AIN0				0x4000	// AIN0(p)/GND(n)
#define	CONFIG_MUX_SINGLE_AIN1				0x5000	// AIN1(p)/GND(n)
#define	CONFIG_MUX_SINGLE_AIN2				0x6000	// AIN2(p)/GND(n)
#define	CONFIG_MUX_SINGLE_AIN3				0x7000	// AIN3(p)/GND(n)
#define	CONFIG_MUX_DEFAULT					CONFIG_MUX_DIFFERENTIAL_AIN01
	// PGA[11:9] - Programable gain amplifier configuration
#define	CONFIG_FS_MASK						0x0e00	// PGA Bit mask
#define	CONFIG_FS_6144						0x0000	// +/-6144mV(x2/3)
#define	CONFIG_FS_4096						0x0200	// +/-4096mV(x1)
#define	CONFIG_FS_2048						0x0400	// +/-2048mV(x2) -- default
#define	CONFIG_FS_1024						0x0600	// +/-1024mV(x4)
#define	CONFIG_FS_0512						0x0800	// +/-512mV(x8)
#define	CONFIG_FS_0256						0x0a00	// +/-256mV(x16)
#define	CONFIG_FS_DEFAULT					CONFIG_FS_2048
	// MODE[8] - Device operating mode
#define	CONFIG_MODE_MASK					0x0100	// MODE Bit mask
#define	CONFIG_MODE_CONTINUOUS				0x0000	// Contiuous conversion mode
#define	CONFIG_MODE_SINGLE_SHOT				0x0100	// Power-down single-shot mode -- default
#define	CONFIG_MODE_DEFAULT					CONFIG_MODE_SINGLE_SHOT
	// DR[7:5] - Data rate
#define	CONFIG_DR_MASK						0x00e0	// DR Bit mask
#define	CONFIG_DR_8SPS						0x0000	// Data rate is 8 Sampling  Per Second, conversion delay is 125mS
#define	CONFIG_DR_16SPS						0x0020	// Data rate is 16 Sampling  Per Second, conversion delay is 62.5mS
#define	CONFIG_DR_32SPS						0x0040	// Data rate is 32 Sampling  Per Second, conversion delay is 31.3mS
#define	CONFIG_DR_64SPS						0x0060	// Data rate is 64 Sampling  Per Second, conversion delay is 15.6mS
#define	CONFIG_DR_128SPS					0x0080	// Data rate is 128 Sampling  Per Second, conversion delay is 7.8mS -- default
#define	CONFIG_DR_250SPS					0x00a0	// Data rate is 250 Sampling  Per Second, conversion delay is 4.0mS
#define	CONFIG_DR_475SPS					0x00c0	// Data rate is 475 Sampling  Per Second, conversion delay is 2.1mS
#define	CONFIG_DR_860SPS					0x00e0	// Data rate is 860 Sampling  Per Second, conversion delay is 1.2mS
#define	CONFIG_DR_DEFAULT					CONFIG_DR_128SPS
	// COMP_MODE[4] - Comparator mode
#define	CONFIG_COMP_MODE_MASK				0x0010	// COMP_MODE Bit mask
#define	CONFIG_COMP_MODE_TRADITIONAL		0x0000	// Traditional comparator with hysteresis -- default	
#define	CONFIG_COMP_MODE_WINDOW				0x0010	// Windows comparator	
#define	CONFIG_COMP_MODE_DEFAULT			CONFIG_COMP_MODE_TRADITIONAL	
	// COMP_POL[3] -- Comparator polarity
#define	CONFIG_COMP_POL_MASK				0x0008	// COMP_POL Bit mask
#define	CONFIG_COMP_POL_LOW					0x0000	// Active low -- default
#define	CONFIG_COMP_POL_HIGH				0x0008	// Active high
#define	CONFIG_COMP_POL_DEFAULT				CONFIG_COMP_POL_LOW
	// COMP_LAT[2] -- Latching comparator
#define	CONFIG_COMP_LAT_MASK				0x0004	// COMP_LAT Bit mask
#define	CONFIG_COMP_LAT_NON					0x0000	// Non-latching comparator -- default
#define	CONFIG_COMP_LAT_LATCHING			0x0004	// Latching comparator
#define	CONFIG_COMP_LAT_DEFAULT				CONFIG_COMP_LAT_NON
	// COMP_QUE[1:0] - Comparator queue and disable
#define	CONFIG_COMP_QUE_MASK				0x0003	// COMP_QUE Bit mask
#define	CONFIG_COMP_QUE_ASSERT_ONE			0x0000	// Assert after one conversion
#define	CONFIG_COMP_QUE_ASSERT_TWO			0x0001	// Assert after two conversions
#define	CONFIG_COMP_QUE_ASSERT_FOUR			0x0002	// Assert after four conversions
#define	CONFIG_COMP_QUE_DISABLE				0x0003	// Disable comparator -- default
#define	CONFIG_COMP_QUE_DEFAULT				CONFIG_COMP_QUE_DISABLE

// Class
class ADS1115 {
  public:
	ADS1115(uint8_t i2cAddress = ADS1115_I2C_ADDRESS_GND);
	int configure(uint16_t configValue);		// Configure ADS1115 settings
	int16_t readSingleEnded(int channel);		// Read conversion result specified single ended channel by single-shot mode
	int16_t readDifferential0and1(void);		// Read conversion result specified differencial between 0 and 1 by single-shot mode
	int16_t readDifferential2and3(void);		// Read conversion result specified differencial between 2 and 3 by single-shot mode 
	void startConversion(uint8_t channel, boolean singleShot);	// Start specified channel conversion coninuously
	int16_t getLastConversionResult(void);		// Get last conversion result
	uint32_t getConversionDelay(uint16_t dataRate);	// Get conversion delay time[uS] from data rate
	void setThresholds(int16_t lower, int16_t upper);	// Set lower and upper thresholds
	uint16_t getConfig(void) { return _config; }	// Get current configuration

  private:
	void writeRegister(uint8_t reg, uint16_t value);
	int16_t readRegister(uint8_t reg);
	uint8_t   _i2cAddress;				// This ads1115's i2c address
	uint16_t _config;					// Current Configuration
	uint32_t _conversionDelay;			// Current conversion delay time[uS]
};

#endif	// _ADS1115_H_
