/*
 *	ADXL362J.h
 *
 *	ADXL362 control library for Arduino (interface)
 *
 *	Version:
 *		R1.0  2016.05.15
 *		R1.1  2017.01.03
 *		R1.2  2017.01.10  change readFIFO() interface
 *		R1.3  2017.02.01  add getAccelerations()
 *
 *	Note:
 *		ADXL362 is a ultra low power 3-Axis MEMS Accelerometer.
 *		This library support a 32bit MPU only.
 *
 *	Copyright(c) 2016-2017 A.Daikoku
 */

#ifndef _ADXL362J_
#define _ADXL362J_

#include <Arduino.h>
#include <SPI.h>

/*
 *	Constants
 */
#define	a2MAX_FIFO_SAMPLES		256			// Maximum number of samples that can be held in the FIFO
		/* The "sample" is defined as one acceleration of one axis. 
		   That is, the XYZ set of accelerations is assumed to be three samples.
		 */
#define	a2MAX_SPI_CLOCK			4000000U	// Maxmimum SPI clock on ADXL362 [Hz]

/*
 *	Define using SPI name
 */
#define	a2SPIx					SPI1		// for Teensy 3.x
 
/*
 *	Register Address
 */
/**	Register_name				Register_No	   [mode] Default_value **/
#define	a2REG_DEVID_AD			0x00		// [R] 0xAD
#define	a2REG_MST				0x01		// [R] 0x1D
#define	a2REG_PARTID			0x02		// [R] 0xF2
#define	a2REG_REVID				0x03		// [R] 0x01
#define	a2REG_XDATA				0x08		// [R] 0x00
#define	a2REG_YDATA				0x09		// [R] 0x00
#define	a2REG_ZDATA				0x0A		// [R] 0x00
#define	a2REG_STATUS			0x0B		// [R] 0x40
#define	a2REG_FIFO_ENTRIES_L	0x0C		// [R] 0x00
#define	a2REG_FIFO_ENTRIES_H	0x0D		// [R] 0x00
#define	a2REG_XDATA_L			0x0E		// [R] 0x00
#define	a2REG_XDATA_H			0x0F		// [R] 0x00
#define	a2REG_YDATA_L			0x10		// [R] 0x00
#define	a2REG_YDATA_H			0x11		// [R] 0x00
#define	a2REG_ZDATA_L			0x12		// [R] 0x00
#define	a2REG_ZDATA_H			0x13		// [R] 0x00
#define	a2REG_TEMP_L			0x14		// [R] 0x00
#define	a2REG_TEMP_H			0x15		// [R] 0x00
#define	a2REG_RESERVED1			0x16		// [R] 0x00
#define	a2REG_RESERVED2			0x17		// [R] 0x00
#define	a2REG_SOFT_RESET		0x1F		// [W] 0x00
#define	a2REG_THRESH_ACT_L		0x20		// [RW] 0x00
#define	a2REG_THRESH_ACT_H		0x21		// [RW] 0x00
#define	a2REG_TIME_ACT			0x22		// [RW] 0x00
#define	a2REG_THRESH_INACT_L	0x23		// [RW] 0x00
#define	a2REG_THRESH_INACT_H	0x24		// [RW] 0x00
#define	a2REG_TIME_INACT_L		0x25		// [RW] 0x00
#define	a2REG_TIME_INACT_H		0x26		// [RW] 0x00
#define	a2REG_ACT_INACT_CTL		0x27		// [RW] 0x00
#define	a2REG_FIFO_CONTROL		0x28		// [RW] 0x00
#define	a2REG_FIFO_SAMPLES		0x29		// [RW] 0x80
#define	a2REG_INTMAP1			0x2A		// [RW] 0x00
#define	a2REG_INTMAP2			0x2B		// [RW] 0x00
#define	a2REG_FILTER_CTL		0x2C		// [RW] 0x13
#define	a2REG_POWER_CTL			0x2D		// [RW] 0x00
#define	a2REG_SELF_TEST			0x2E		// [RW] 0x00
 
// REG_DEVID_AD..REG_REVID Values
#define	a2DEVID_AD				0xAD		// Analog Devices ID
#define	a2DEVID_MST				0x1D 		// MEMS sensor ID
#define	a2PARTID				0xF2		// ADXL362 ID
#define	a2REVID					0x01		//

// REG_STATUS
#define	a2STATS_ERR_USER_REGS	0x80		// [R] 0x0
#define	a2STATS_AWAKE			0x40		// [R] 0x1
#define	a2STATUS_INACT			0x20		// [R] 0x0
#define	a2STATUS_ACT			0x10		// [R] 0x0
#define	a2STATUS_FIFO_OVERRUN	0x08		// [R] 0x0
#define	a2STATUS_FIFO_WATERMARK	0x04		// [R] 0x0
#define	a2STATUS_FIFO_READY		0x02		// [R] 0x0
#define	a2STATUS_DATA_READY		0x01		// [R] 0x0

// REG_SOFT_RESET
#define	a2SOFT_RESET_RESET		0x52		// [W] 0x00 - Execute reset

// REG_FIFO_CONTROL
#define	a2FIFO_CONTROL_AH			0x08		// [RW] 0x0
#define	a2FIFO_CONTROL_FIFO_TEMP	0x04		// [RW] 0x0
#define	a2FIFO_CONTROL_MODE_MASK	0x03		// [RW] 0x0
	// FIFO_CONTROL_MODE_MASK's values
#define	a2FIFO_CONTROL_MODE_DISABLE	0x00		// Not use fifo
#define	a2FIFO_CONTROL_MODE_OLDEST_SAVED 0x01	// Use fifo - Oldest saved mode
#define	a2FIFO_CONTROL_MODE_STREAM	0x02		// Use fifo - Streaming mode
#define	a2FIFO_CONTROL_MODE_TRIGGER	0x03		// Use fifo - Trigger mode

// REG_INTMAP1
#define	a2INTMAP1_INT_LOW		0x80		// [RW] 0x0
#define	a2INTMAP1_AWAKE			0x40		// [RW] 0x0
#define	a2INTMAP1_INACT			0x20		// [RW] 0x0
#define	a2INTMAP1_ACT			0x10		// [RW] 0x0
#define	a2INTMAP1_FIFO_OVERRUN	0x08		// [RW] 0x0
#define	a2INTMAP1_FIFO_WATERMARK 0x04		// [RW] 0x0
#define	a2INTMAP1_FIFO_READY	0x02		// [RW] 0x0
#define	a2INTMAP1_DATA_READY	0x01		// [RW] 0x0

// REG_INTMAP2
#define	a2INTMAP2_INT_LOW		0x80		// [RW] 0x0
#define	a2INTMAP2_AWAKE			0x40		// [RW] 0x0
#define	a2INTMAP2_INACT			0x20		// [RW] 0x0
#define	a2INTMAP2_ACT			0x10		// [RW] 0x0
#define	a2INTMAP2_FIFO_OVERRUN	0x08		// [RW] 0x0
#define	a2INTMAP2_FIFO_WATERMARK 0x04		// [RW] 0x0
#define	a2INTMAP2_FIFO_READY	0x02		// [RW] 0x0
#define	a2INTMAP2_DATA_READY	0x01		// [RW] 0x0

// REG_FILTER_CTL
#define	a2FILTER_CTL_RANGE_MASK	0xC0		// [RW] 0x0
#define	a2FILTER_CTL_RES		0x20		// [RW] 0x0
#define	a2FILTER_CTL_HALF_BW_MASK 0x10		// [RW] 0x10
#define	a2FILTER_CTL_EXT_SAMPLE	0x08		// [RW] 0x0
#define	a2FILTER_CTL_ODR_MASK	0x07		// [RW] 0x3
	// a2FILTER_CTL_RANGE_MASK's values
#define	a2FILTER_CTL_RANGE_2g	0x00		// +/-2g [default]
#define	a2FILTER_CTL_RANGE_4g	0x40		// +/-4g
#define	a2FILTER_CTL_RANGE_8g	0x80		// +/-8g
	// a2FILTER_CTL_HALF_BW_MASK's values
#define	a2FILTER_CTL_HALF_BW_2	0x00		// ODR/2
#define	a2FILTER_CTL_HALF_BW_4	0x10		// ODR/4 [default]
	// a2FILTER_CTL_ODR_MASK's values
#define	a2FILTER_CTL_ODR_12_5Hz	0x00		// 12.5Hz
#define	a2FILTER_CTL_ODR_25Hz	0x01		// 25Hz
#define	a2FILTER_CTL_ODR_50Hz	0x02		// 50Hz
#define	a2FILTER_CTL_ODR_100Hz	0x03		// 100Hz [default]
#define	a2FILTER_CTL_ODR_200Hz	0x04		// 200Hz
#define	a2FILTER_CTL_ODR_400Hz	0x05		// 400Hz

// REG_POWER_CTL
#define	a2POWER_CTL_RESERVED	0x80		// [RW] 0x0
#define	a2POWER_CTL_EXT_CLK		0x40		// [RW] 0x0
#define	a2POWER_CTL_LOW_NOISE_MASK 0x30		// [RW] 0x0
#define	a2POWER_CTL_WAKEUP		0x08		// [RW] 0x0
#define	a2POWER_CTL_AUTOSLEEP	0x04		// [RW] 0x0
#define	a2POWER_CTL_MEASURE_MASK 0x03		// [RW] 0x0
	// a2POWER_CTL_LOW_NOISE_MASK's values
#define	a2POWER_CTL_LOW_NOISE_NORMAL 0x00		// [default]
#define	a2POWER_CTL_LOW_NOISE_LOW	0x10		// 
#define	a2POWER_CTL_LOW_NOISE_SUPER_LOW	0x20	// 
	// a2POWER_CTL_MEASURE_MASK's values
#define	a2POWER_CTL_MEASURE_STANDBY	0x00		// Stand by [default]
#define	a2POWER_CTL_MEASURE_RUN		0x02		// Run

/*
 *		Class
 */
class ADXL362J {
  public:
	ADXL362J(uint8_t csPin = SS);					// Constructor with chip select pin
	uint8_t readRegister(uint8_t reg);				// Read byte data from a register
	void readMultiRegisters(uint8_t reg, int bytes, uint8_t data[]); // Read multi bytes data from registers
	void writeRegister(uint8_t reg, uint8_t data);	// Write byte data to a register

	int getAccelerations(int *ax, int *ay, int *az); // Get 3-axis accelerations
	int getFIFOEntries(void) __attribute__((always_inline)) {
													// Get FIFO entries [samples]
		uint8_t data[2];
		data[1] = readRegister(a2REG_FIFO_ENTRIES_H);
		data[0] = readRegister(a2REG_FIFO_ENTRIES_L);
		return (((((unsigned)data[1] & 0x03) << 8) + (unsigned)data[0]) / 2);
	}
	int readFIFO(int samples[], int numSamples); 	// Read data from FIFO
	int toRaw(uint8_t high, uint8_t low) __attribute__((always_inline)) {
													// Convert high byte and low byte to 12-bit accelaration
		int data = ((int)high << 8) + ((unsigned)low && 0xff);
		if (high & 0x08)
			data |= 0xfffff000;		// extent sign bits
		return data;
	}
	int tomg(int raw, int range) __attribute__((always_inline)) {
													// Convert raw to mg
		if (range == a2FILTER_CTL_RANGE_2g)
			return (raw);			// 1000 LSB/g @+/-2g
		else if (range == a2FILTER_CTL_RANGE_4g)
			return (raw << 1);		//  500 LSB/g @+/-4g
		else
			return (raw << 2);		//  250 LSB/g @+/-8g
	}

  private:
	uint8_t	_csPin;		// CS Pin number	
};

#endif // _ADXL362J_
