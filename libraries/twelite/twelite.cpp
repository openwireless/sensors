/*
  *	twelite.cpp -- TWELite DIP Control Library (Implementation)
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
 
#include "twelite.h"

//#define	DEBUG_NOW

#define	PACKET_SIZE		41		// [bytes]
#define	DELIMITTER		';'

#ifdef DEBUG_NOW
#include <SoftwareSerial.h>
extern SoftwareSerial dbgSerial;
#endif

void TWELite::begin(uint32_t baudrate) {
	tweSerial.begin(baudrate);
}

void TWELite::end(void) {
	tweSerial.end();
}

boolean TWELite::hasReceivedMessages(void) {
	return (tweSerial.available() >= PACKET_SIZE);
}

int TWELite::parseMessage(void) {
	char message[PACKET_SIZE];

	for (int n = 0; n < PACKET_SIZE && tweSerial.available() > 0; n++) {
		message[n] = tweSerial.read();
#ifdef DEBUG_NOW
		dbgSerial.print("message[");
		dbgSerial.print(n);
		dbgSerial.print("] ");
		dbgSerial.println(message[n]);
#endif
	}
		// Check packet format
	if (message[0] != DELIMITTER)
		return tweError;
	if (message[PACKET_SIZE-2] != '\r' || message[PACKET_SIZE-1] != '\n')
		return tweError;

	// Identify child type
	childType = CTYPE_UNKNOWN;
	switch (message[PACKET_SIZE-4]) {
		case 'S' : 
			childType = CTYPE_TANDH;
			break;
		case 'M' :
			childType = CTYPE_IL;
			break;
		case 'A' :
			childType = CTYPE_T;
			break;
		case 'N' :
			childType = CTYPE_WF;
			break;
		case 'Z' :
			childType = CTYPE_AD;
			break;
		default:
			return tweError;
	}

	// Parse message
	childId = atoi(message+1);
	lqi = atoi(message+13);
	batteryVoltage = atoi(message+17);
	switch (childType) {
		case CTYPE_TANDH :
			temperature = atoi(message+22);
			humidity = atoi(message+27);
			voltageADC2 = atoi(message+32);
			break;
		case CTYPE_IL:
			illuminance = ((long)atoi(message+22) * 10000L) + (long)atoi(message+27);
			voltageADC2 = atoi(message+32);
			break;
		case CTYPE_T:
			temperature2 = atoi(message+22);
			bitmap = atoi(message+27);
			voltageADC2 = atoi(message+32);
			break;
		case CTYPE_WF:
			waterFlow = ((long)atoi(message+22) * 10000L) + (long)atoi(message+27);
			bitmap = atoi(message+32);
			break;
		case CTYPE_AD:
			voltageADC1 = atoi(message+22);
			voltageADC2 = atoi(message+27);
			bitmap = atoi(message+32);
			break;
		default:
			return tweError;
	}
	return tweOK;
}

void TWELite::discardMessages(void) {
	// Discard whole data in buffer
	while (tweSerial.available() > 0)
		(void)tweSerial.read();
}

// end
