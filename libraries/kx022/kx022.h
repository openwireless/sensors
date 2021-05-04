/*
 *	kx022.h -- KX022 control functions
 *
 *	@date		2014.12.26
 *	@author		A.D (TABrain Inc.)
 *	@note
 *				This library is control library for KX022(Kionix Tri-axis digital Accelerometer)
 */

#ifndef	_KX022_H_
#define	_KX022_H_		1

#include <Arduino.h>

// KX022 I2C Address - selectable by ADDR pin
#define	KX022_I2C_ADDR_VDD		0x1f		// when ADDR=VDD
#define	KX022_I2C_ADDR_GND		0x1e		// when ADDR=GND

// Register address
#define	REGISTER_XOUTL			((uint8_t)0x06)
#define	REGISTER_XOUTH			((uint8_t)0x07)
#define	REGISTER_YOUTL			((uint8_t)0x08)
#define	REGISTER_YOUTH			((uint8_t)0x09)
#define	REGISTER_ZOUTL			((uint8_t)0x0a)
#define	REGISTER_ZOUTH			((uint8_t)0x0b)
#define	REGISTER_COTR			((uint8_t)0x0c)
#define	REGISTER_WHO_AM_I		((uint8_t)0x0f)
#define	REGISTER_INS2			((uint8_t)0x13)
#define	REGISTER_INS3			((uint8_t)0x14)
#define	REGISTER_CNTL1			((uint8_t)0x18)
#define	REGISTER_CNTL2			((uint8_t)0x19)
#define	REGISTER_CNTL3			((uint8_t)0x1a)
#define	REGISTER_ODCNTL			((uint8_t)0x1b)
#define	REGISTER_INC1			((uint8_t)0x1c)
#define	REGISTER_INC4			((uint8_t)0x1f)
#define	REGISTER_INC5			((uint8_t)0x20)
#define	REGISTER_INC6			((uint8_t)0x21)
#define	REGISTER_BUF_CNTL1		((uint8_t)0x3a)
#define	REGISTER_BUF_CNTL2		((uint8_t)0x3b)
#define	REGISTER_BUF_STATUS_1	((uint8_t)0x3c)
#define	REGISTER_BUF_CLEAR		((uint8_t)0x3e)
#define	REGISTER_BUF_READ		((uint8_t)0x3f)
#define	REGISTER_SELF_TEST		((uint8_t)0x60)

// Register values
	//	Operating mode (REGISTER_CNTL1:PC1)
#define	CNTL1_PC1_MASK			0x80		// PC1 mask bits
#define	CNTL1_PC1_STANDBY		0x00		// Stand-by mode -- default
#define	CNTL1_PC1_OPERATING		0x80		// Operating mode
	//	Operating moide (REGISTER_CNTL1:RES)
#define	CNTL1_RES_MASK			0x40		// RES mask bits
#define	CNTL1_RES_LOW_CURRENT	0x00		// Low current -- default
#define	CNTL1_RES_HIGH_CURRENT	0x40		// High current. Bandwidth(Hz)=ODR/2
	//	Range of accelerometer (REGISTER_CNTL1:GSEL1-0)
#define	CNTL1_GSEL_MASK			0x18		// GSEL mask bits
#define	CNTL1_GSEL_2G			0x00		// +/-2g -- default
#define	CNTL1_GSEL_4G			0x08		// +/-4g
#define	CNTL1_GSEL_8G			0x18		// +/-8g
	//	Software reset (REGISTER_CNTL2:SRST)
#define	CNTL2_SRST_MASK			0x80		// SRST mask bits
#define	CNTL2_SRST_NOACT		0x00		// No action -- default
#define	CNTL2_SRST_RESET		0x80		// Reboot
	//	Filter bypass mode (REGISTER_ODCNTL:IIR_BYPASS)
#define	ODCNTL_IIR_BYPASS_MASK	0x80		// IIR_BYPASS mask bits
#define	ODCNTL_IIR_BYPASS_APPLY	0x00		// filter applied -- default
#define	ODCNTL_IIR_BYPASS_BYPASS 0x80		// filter bypassed
	//	Low-pass filter roll off control (REGISTER_ODCNTL:LPRO)
#define	ODCNTL_LPRO_MASK		0x40		// LPRO mask bits
#define	ODCNTL_LPRO_ODR_BY9		0x00		// filter corner frequency seto to ODR/9 -- default
#define	ODCNTL_LPRO_ODR_BY2		0x40		// filter corner frequency seto to ODR/4
	//	Output data rate (REGISTER_ODCNTL:OSA3-0)
#define	ODCNTL_OSA_MASK			0x0f		// OSA mask bits
#define	ODCNTL_OSA_12			0x00		// 12.5Hz
#define	ODCNTL_OSA_25			0x01		// 25Hz
#define	ODCNTL_OSA_50			0x02		// 50Hz -- default
#define	ODCNTL_OSA_100			0x03		// 100Hz
#define	ODCNTL_OSA_200			0x04		// 200Hz
#define	ODCNTL_OSA_400			0x05		// 400Hz
#define	ODCNTL_OSA_800			0x06		// 800Hz
#define	ODCNTL_OSA_1600			0x07		// 1600Hz
#define	ODCNTL_OSA_DEFAULT		ODCNTL_OSA_50
	// Control activation of buffer (REGISTER_BUF_CNTL2:BFE)
#define	BUF_CNTL2_BUFE_MASK		0x80		// BFE mask bits
#define	BUF_CNTL2_BUFE_INACTIVE	0x00		// sample buffer inactive -- default
#define	BUF_CNTL2_BUFE_ACTIVE	0x80		// sample buffer active
	// Resolution of the acceleration data (REGISTER_BUF_CNTL2:BRES)
#define	BUF_CNTL2_BRES_MASK		0x40		// BRES mask bits
#define	BUF_CNTL2_BRES_8BIT		0x00		// 8-bit sample -- default
#define	BUF_CNTL2_BRES_16BIT	0x40		// 16-bit sample
	// Buffer full interrupt (REGISTER_BUF_CNTL2:BFIE)
#define	BUF_CNTL2_BFIE_MASK		0x20		// BFIE mask bits
#define	BUF_CNTL2_BFIE_DISABLE	0x00		// buffer full interrupt disable -- default
#define	BUF_CNTL2_BFIE_ENABLE	0x20		// buffer full interrupt enable in INS2
	//	Buffer mode (REGISTER_BUF_CNTL2:BM1-0)
#define	BUF_CNTL2_BM_MASK		0x03	// BM mask bits
#define	BUF_CNTL2_BM_FIFO		0x00	// FIFO -- default
#define	BUF_CNTL2_BM_STREAM		0x01	// Stream
#define	BUF_CNTL2_BM_TRIGGER	0x02	// Trigger
#define	BUF_CNTL2_BM_FILO		0x03	// FILO

//	Class
class KX022 {
  public:
	KX022(uint8_t i2cAddress);		// Constructor
	uint8_t readRegister(uint8_t reg);		// Read byte from register
	uint16_t readRegister2(uint8_t reg);	// Read word from register
	int writeRegister(uint8_t reg, uint8_t data);	// Write byte to register
	int	readBuffer(uint8_t samples[], int numSamples);	// Read bytes from FIFO

  private:
	uint8_t   _i2cAddress;				// This kx022's i2c address
};

#endif	// _KX022_H_
