// 
// 
// 
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESP32Encoder.h>
#include <AceButton.h>
using namespace ace_button;
#include "gui.h"
#include "setup.h"
#include "network.h"
#include "vfo.h"
#include "io.h"
#include "FT891_CAT.h"
#include "credentials.h"

SemaphoreHandle_t GuiBinarySemaphore = NULL;
SemaphoreHandle_t swrBinarySemaphore = NULL;

// the setup function runs once when you press reset or power the board
void setup() {
	
	Serial.begin(115200); /* prepare for possible serial debug */
	pinMode(TXRX_SWITCH, INPUT);
	pinMode(S_METER, ANALOG);
	pinMode(FWD_METER, ANALOG);
	pinMode(REV_METER, ANALOG);
	adc_init();
	memset(&R, 0, sizeof(R));
	init_vfo(); // load default values for R
	LoadEEPROM();
	GuiBinarySemaphore = xSemaphoreCreateMutex();
	if (GuiBinarySemaphore == NULL) {
		Serial.println("Error creating the GuiBinarySemaphore");
	}
	swrBinarySemaphore = xSemaphoreCreateMutex();
	if (swrBinarySemaphore == NULL) {
		Serial.println("Error creating the swrBinarySemaphore");
	}
	net_queue = xQueueCreate(1, sizeof(uint8_t));
	if (net_queue == NULL) {
		Serial.println("Error creating the queue");
	}
	Serial.println("db10m[0]" + String(R.cal_AD[0].db10m) + "Fwd[0]" + String(R.cal_AD[0].Fwd) + "Rev[0]" + String(R.cal_AD[0].Rev));
	Serial.println("db10m[1]" + String(R.cal_AD[1].db10m) + "Fwd[1]" + String(R.cal_AD[1].Fwd) + "Rev[1]" + String(R.cal_AD[1].Rev));
	init_io();
	start_vfo(); 
	guisetup();
	CAT.begin(true);
	start_measurement();
	delay(10);
	strcpy(R.ssid[0], _ssid[1]);
	strcpy(R.password[0], _password[1]);
	if (R.wifi_onoff)
	{
		for (int i = 0; i < 5; i++)
		{
			if (strlen(R.ssid[i]))
			{
				if (vfo_network.begin(TRXNET_SERVER))
				{
					return;
				}
			}
		}
	}
}

void LoadEEPROM()
{
	uint8_t coldstart;

	EEPROM.begin(sizeof(var_t) + sizeof(uint8_t));

	coldstart = EEPROM.read(0);               // Grab the coldstart byte indicator in EEPROM for
											   // comparison with the COLDSTART_REFERENCE

	Serial.println(String("Coldstart : ") + String(coldstart));

	// Initialize all memories if first upload or if COLDSTART_REF has been modified
	// either through PSWR_A.h or through Menu functions
	if (coldstart != COLDSTART_REF)
	{
		EEPROM.write(0, COLDSTART_REF);          // COLDSTART_REF in first byte indicates all initialized
		EEPROM_writeAnything(1, R);              // Write default settings into EEPROM
		EEPROM.commit();
	}
	else                                      // EEPROM contains stored data, retrieve the data
	{
		EEPROM_readAnything(1, R);               // Read the stored data
	}
}

void SaveEEPROM()
{
	EEPROM.write(0, COLDSTART_REF);          // COLDSTART_REF in first byte indicates all initialized
	EEPROM_writeAnything(1, R);              // Write default settings into  
	EEPROM.commit();
}

