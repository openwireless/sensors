/*
  *	twelite.h -- TWELite DIP Control Library (Interface)
  *
  *	R0.1  2015/08/31  1st release
  *	R1.0  2016/08/30  2nd release
  *	R1.1  2016/09/25  3nd rellease (add CTYPE_WF ann CTYPE_T)
  *	R1.2  2016/10/16  4nd rellease (add CTYPE_AD)
  *
  *	Note:
  *		Use one hardware serial port, can't use software serial port
  *
  *	Copyright(c) 2015-2016 Atsushi Daikoku, All Rightds Resevred	
  */
 
 #ifndef _TWELITE_H_
 #define _TWELITE_H_	1

#include <Arduino.h>

// Return values
#define	tweOK					0
#define	tweError				(-1)

// Child type
#define	CTYPE_UNKNOWN			0		// Unknown type
#define	CTYPE_TANDH				1		// Temperature and Humidity type
#define	CTYPE_IL				2		// Illumimance type
#define	CTYPE_WF				3		// Water flow type
#define	CTYPE_T					4		// Temperature type
#define	CTYPE_AD				5		// ADC and Digital input type

 class TWELite {
  public:
	TWELite(HardwareSerial &port) : tweSerial(port) { };
	
	void begin(uint32_t baudrate);
	void end(void);
	boolean hasReceivedMessages(void);
	int parseMessage(void);
	void discardMessages(void);

	inline uint8_t getChildID(void) {
		return (this->childId);				// Logical ID(0-100)
	}

	inline uint8_t getChildType(void) {
		return (this->childType);			// CTYPE_TANDH or CTYPE_IL 
	}

	inline int getBatteryVoltage(void) {
		return (this->batteryVoltage);		// Battery voltage [mV]
	}

	inline int getRSSI(void) {
		return ((7 * this->lqi - 1970) / 20);	// RSSI [dBm]
	}

	inline int getTemperature(void) {
		return (this->temperature);			// Temperature [x100 C]
	}

	inline int getHumidity(void) {
		return (this->humidity);			// Humidity [x100 %]
	}

	inline long getIlluminance(void) {
		return (this->illuminance);			// illuminance [x1 lux]
	}

	inline long getWaterFlow(void) {
		return (this->waterFlow);			// Water flow [x1 dL]
	}

	inline int getTemperature2(void) {
		return (this->temperature2);		// Temperature [x100 C]
	}

	inline int getDigitalInput(void) {
		return (this->bitmap);				// DI1-4 (bit0-3, 1:LOW/0:HIGH)
	}

	inline int getVoltageADC1(void) {
		return (this->voltageADC1);			// ADC1(0..2400) [mV]
	}
	
	inline int getVoltageADC2(void) {
		return (this->voltageADC2);			// ADC2(0..2400) [mV]
	}

  private:
	HardwareSerial tweSerial;
	uint8_t childId, childType, lqi;
	int16_t temperature, humidity, temperature2, bitmap, batteryVoltage, voltageADC1, voltageADC2;
	int32_t waterFlow, illuminance;
};

typedef class TWELite  TWELite; 

#endif // _TWELITE_H_
