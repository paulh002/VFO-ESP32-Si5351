// ArduinoC.h

#ifndef _SETUP_H
#define _SETUP_H

#define COLDSTART_REF          0x08 // When started, the firmware examines this "Serial Number
									// and enforces factory reset if there is a mismatch.
									// Rolling this value is useful if the EEPROM structure has been modified

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "_EEPROMAnything.h"
#include "measurement.h"

extern	var_t				R;
extern	SemaphoreHandle_t	GuiBinarySemaphore;

void SaveEEPROM();
void LoadEEPROM();
void encoders_setup();
#endif

