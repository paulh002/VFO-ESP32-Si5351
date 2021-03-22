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

var_t	R;
SemaphoreHandle_t GuiBinarySemaphore = NULL;

// the setup function runs once when you press reset or power the board
void setup() {
	
	Serial.begin(115200); /* prepare for possible serial debug */
	pinMode(TXRX_SWITCH, INPUT);
	pinMode(S_METER, ANALOG);
	pinMode(FWD_METER, ANALOG);
	pinMode(REV_METER, ANALOG);
	memset(&R, 0, sizeof(R));
	init_vfo_save(0);
	LoadEEPROM();
	R.wifi_onoff = 1;
	GuiBinarySemaphore = xSemaphoreCreateMutex();
	if (GuiBinarySemaphore == NULL) {
		Serial.println("Error creating the GuiBinarySemaphore");
	}
	
	net_queue = xQueueCreate(1, sizeof(uint8_t));
	if (net_queue == NULL) {
		Serial.println("Error creating the queue");
	}

	init_io();
	guisetup();
	CAT.begin(true);
	init_vfo(active_vfo);
	delay(10);
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
		memset(&R, 0, sizeof(var_t));
		init_vfo_save(0);
		EEPROM.write(0, COLDSTART_REF);          // COLDSTART_REF in first byte indicates all initialized
		EEPROM_writeAnything(1, R);              // Write default settings into EEPROM
		EEPROM.commit();
	}
	else                                      // EEPROM contains stored data, retrieve the data
	{
		EEPROM_readAnything(1, R);               // Read the stored data
		init_vfo_load();
	}
}

void SaveEEPROM()
{
	EEPROM.write(0, COLDSTART_REF);          // COLDSTART_REF in first byte indicates all initialized
	EEPROM_writeAnything(1, R);              // Write default settings into  
	EEPROM.commit();
}

