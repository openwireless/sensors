/*
 *	ADXL355J.h
 *
 *	ADXL355 control library for Arduino (Interface)
 *
 *	Version:
 *		R1.0  2017.01.10
 *		R1.1  2017.02.01  add getAccelerations()
 *
 *	Note:
 *		ADXL355 is a 20bit ADC, low noise, low drift 3-Axis MEMS Accelerometer.
 *		This library support a 32bit MPU only.
 *
 *	Copyright(c) 2017 A.Daikoku
 */

#ifndef _ADXL355J_
#define _ADXL355J_

#include <Arduino.h>
#include <SPI.h>

/*
 *	Define using SPI name
 */
#define	a5SPIx					SPI			// for Teensy 3.x
 
/*
 *	Constants
 */
#define	a5MAX_FIFO_SAMPLES		96			// Maximum number of samples that can be held in the FIFO
		/* The "sample" is defined as one acceleration of one axis. 
		   That is, the XYZ set of accelerations is assumed to be three samples.
		 */
#define	a5MAX_SPI_CLOCK			10000000U	// Maxmimum SPI clock on ADXL355 [Hz]

/*
 *	Register Address
 */
/**	Register_name				Register_No	   [mode] Default_value **/
#define	a5REG_DEVID_AD			0x00		// [R] 0xAD
#define	a5REG_DEVID_MST			0x01		// [R] 0x1D
#define	a5REG_PARTID			0x02		// [R] 0xED
#define	a5REG_REVID				0x03		// [R] 0x01
#define	a5REG_STATUS			0x04		// [R] 0x00
#define	a5REG_FIFO_ENTRIES		0x05		// [R] 0x00
#define	a5REG_TEMP2				0x06		// [R] 0x00
#define	a5REG_TEMP1				0x07		// [R] 0x00
#define	a5REG_XDATA3			0x08		// [R] 0x00 (bit19-12)
#define	a5REG_XDATA2			0x09		// [R] 0x00 (bit11-4)
#define	a5REG_XDATA1			0x0a		// [R] 0x00 (bit3-0)
#define	a5REG_YDATA3			0x0b		// [R] 0x00 (bit19-12)
#define	a5REG_YDATA2			0x0c		// [R] 0x00 (bit11-4)
#define	a5REG_YDATA1			0x0d		// [R] 0x00 (bit3-0)
#define	a5REG_ZDATA3			0x0e		// [R] 0x00 (bit19-12)
#define	a5REG_ZDATA2			0x0f		// [R] 0x00 (bit11-4)
#define	a5REG_ZDATA1			0x10		// [R] 0x00 (bit3-0)
#define	a5REG_FIFO_DATA			0x11		// [R] 0x00
#define	a5REG_OFFSET_X_H		0x1e		// [RW] 0x00
#define	a5REG_OFFSET_X_L		0x1f		// [RW] 0x00
#define	a5REG_OFFSET_Y_H		0x20		// [RW] 0x00
#define	a5REG_OFFSET_Y_L		0x21		// [RW] 0x00
#define	a5REG_OFFSET_Z_H		0x22		// [RW] 0x00
#define	a5REG_OFFSET_Z_L		0x23		// [RW] 0x00
#define	a5REG_ACT_EN			0x24		// [RW] 0x00
#define	a5REG_ACT_THRESH_H		0x25		// [RW] 0x00
#define	a5REG_ACT_THRESH_L		0x26		// [RW] 0x00
#define	a5REG_ACT_COUNT			0x27		// [RW] 0x01
#define	a5REG_FILTER			0x28		// [RW] 0x00
#define	a5REG_FIFO_SAMPLES		0x29		// [RW] 0x60
#define	a5REG_INT_MAP			0x2a		// [RW] 0x00
#define	a5REG_SYNC				0x2b		// [RW] 0x00
#define	a5REG_RANGE				0x2c		// [RW] 0x81
#define	a5REG_POWER_CTL			0x2d		// [RW] 0x01
#define	a5REG_SELF_TEST			0x2e		// [RW] 0x00
#define	a5REG_RESET				0x2f		// [W] 0x00

// REG_DEVID_AD..REG_REVID Values
#define	a5DEVID_AD				0xad		// Analog Devices ID
#define	a5DEVID_MST				0x1d 		// MEMS sensor ID
#define	a5PARTID				0xed		// ADXL355 ID
#define	a5REVID					0x01		// Revision ID(1..)

// REG_STATUS
#define	a5STATUS_NVM_BUSY		0x10		// [R] 0x0
#define	a5STATUS_ACTIVITY		0x08		// [R] 0x0
#define	a5STATUS_FIFO_OVR		0x04		// [R] 0x0
#define	a5STATUS_FIFO_FULL		0x02		// [R] 0x0
#define	a5STATUS_DATA_RDY		0x01		// [R] 0x0

// REG_FIFO_ENTRIES
#define	a5FIFO_ENTRIES_MASK		0x7f		// [R] 0x00

// REG_TEMP2
#define	a5TEMP2_TEMPERATURE_HIGH	0x0f	// [R] 0x0

// REG_TEMP1
#define	a5TEMP1_TEMPERATURE_LOW		0xff	// [R] 0x00

// REG_XDATA1
#define	a5XDATA1_MASK			0xf0		// [R] 0x0

// REG_YDATA1
#define	a5YDATA1_MASK			0xf0		// [R] 0x0

// REG_ZDATA1
#define	a5ZDATA1_MASK			0xf0		// [R] 0x0

// REG_ACT_EN
#define	a5ACT_EN_ACT_Z			0x04		// [R] 0x0
#define	a5ACT_EN_ACT_Y			0x02		// [R] 0x0
#define	a5ACT_EN_ACT_X			0x01		// [R] 0x0

// REG_FILTER
#define	a5FILTER_HPF_CORNER_MASK	0x70	// [RW] 0x0
#define	a5FILTER_ODR_LPF_MASK		0x0f	// [RW] 0x0
	// a5FILTER_ODR_LPF_MASK
#define	a5FILTER_ODR_LPF_4000HZ		0x0
#define	a5FILTER_ODR_LPF_2000HZ		0x1
#define	a5FILTER_ODR_LPF_1000HZ		0x2
#define	a5FILTER_ODR_LPF_500HZ		0x3
#define	a5FILTER_ODR_LPF_250HZ		0x4
#define	a5FILTER_ODR_LPF_125HZ		0x5
#define	a5FILTER_ODR_LPF_62_5HZ		0x6
#define	a5FILTER_ODR_LPF_31_25HZ	0x7
#define	a5FILTER_ODR_LPF_15_625HZ	0x8
#define	a5FILTER_ODR_LPF_7_813HZ	0x9
#define	a5FILTER_ODR_LPF_3_906HZ	0xa

// REG_FIFO_SAMPLES
#define	a5FIFO_SAMPLES_MASK		0x7f		// [RW] 0x60 (Water Mark)

// REG_INT_MAP
#define	a5INT_MAP_ACT_EN2		0x80		// [RW] 0x0
#define	a5INT_MAP_OVR_EN2		0x40		// [RW] 0x0
#define	a5INT_MAP_FULL_EN2		0x20		// [RW] 0x0
#define	a5INT_MAP_RDY_EN2		0x10		// [RW] 0x0
#define	a5INT_MAP_ACT_EN1		0x08		// [RW] 0x0
#define	a5INT_MAP_OVR_EN1		0x04		// [RW] 0x0
#define	a5INT_MAP_FULL_EN1		0x02		// [RW] 0x0
#define	a5INT_MAP_RDY_EN1		0x01		// [RW] 0x0

// REG_SYNC
#define	a5SYNC_EXT_CLK			0x04		// [RW] 0x0
#define	a5SYNC_EXT_SYNC_MASK	0x03		// [RW] 0x0
	// a5SYNC_EXT_SYNC_MASK's values
#define	a5SYNC_EXT_SYNC_INTERNAL	0x00		// Disable external sync. control
#define	a5SYNC_EXT_SYNC_EXTERNAL1	0x01		// External sync. without filter
#define	a5SYNC_EXT_SYNC_EXTERNAL2	0x02		// External sync. with filter
#define	a5SYNC_EXT_SYNC_RESERVED	0x03

// REG_RANGE
#define	a5RANGE_I2C_HS			0x80		// [RW] 0x1
#define	a5RANGE_INT_POL			0x40		// [RW] 0x0
#define	a5RANGE_RANGE_MASK		0x03		// [RW] 0x1
	// a5RANGE_RANGE_MASK's values
#define	a5RANGE_RANGE_2G		0x01
#define	a5RANGE_RANGE_4G		0x02
#define	a5RANGE_RANGE_8G		0x03

// REG_POWER_CTL
#define	a5POWER_CTL_DRDY_OFF	0x04		// [RW] 0x0(disable DRDY)
#define	a5POWER_CTL_TEMP_OFF	0x02		// [RW] 0x0(Enable temperature control)
#define	a5POWER_CTL_STANDBY		0x01		// [RW] 0x1(Stand by mode)

// REG_SELF_TEST
#define	a5SELF_TEST_ST2			0x02		// [RW] 0x0
#define	a5SELF_TEST_ST1			0x01		// [RW] 0x0

// REG_RESET
#define	a5RESET_CODE			0x52		// [W] 0x0

/*
 *		Class
 */
class ADXL355J {
  public:
	ADXL355J(uint8_t csPin = SS);					// Constructor with chip select pin
	uint8_t readRegister(uint8_t reg);				// Read byte data from a register
	void readMultiRegisters(uint8_t reg, int bytes, uint8_t data[]); // Read multi bytes data from registers
	void writeRegister(uint8_t reg, uint8_t data);	// Write byte data to a register

	int getAccelerations(int *ax, int *ay, int *az); // Get 3-axis accelerations
	int getFIFOEntries(void) __attribute__((always_inline)) {
													// Get FIFO entries [samples]
		return (readRegister(a5REG_FIFO_ENTRIES) & a5FIFO_ENTRIES_MASK);
	}
	int readFIFO(int samples[], int numSamples); 	// Read data from FIFO
    int toRaw(uint8_t data3, uint8_t data2, uint8_t data1) __attribute__((always_inline)) {
													// Convert three bytes to raw value
		int data = ((int32_t)data3 << 12) | (((uint32_t)data2 << 4) & 0xff00) | ((uint32_t)(data1 >> 4) & 0x0f);
		if (data3 & 0x80)
			data |= 0xfff00000;		// extent sign bits
		return data;
    }
	int tomg(int raw, int range) __attribute__((always_inline)) {
													// Convert raw to mg
		if (range == a5RANGE_RANGE_2G)
			return (raw >> 8);		// 256000 LSB/g @+/-2g
		else if (range == a5RANGE_RANGE_2G)
			return (raw >> 7);		// 128000 LSB/g @+/-4g
		else
			return (raw >> 6);		//  64000 LSB/g @+/-8g
	}

  private:
	uint8_t	_csPin;		// CS Pin number
};

#endif	// _ADXL355J_
 