
#include <arduino.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_ILI9341.h>
#include <EEPROM.h>
#include "i2c.h"
#include "si5351.h"
#include "measurement.h"
#include "ringmeter.h"
#include "setup.h"
#include "_EEPROMAnything.h"
#include "Cal_config.h"

#include "Fonts/DroidSansMono8pt7b.h"
#include "Fonts/DroidSansMono12pt7b.h"
#include "Fonts/DroidSansMono24pt7b.h"
#include "Fonts/DroidSansMono48pt7b.h"

extern  Adafruit_ILI9341 tft;

uint8_t setup_menu_item = 1, c_setup_menu_item = 1, setup_select = 0;

void setup_menu(void)
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&DroidSansMono8pt7b);
  
  tft.setCursor(30, 20);
  tft.print("Setup");
  tft.setCursor(30, 40);
  tft.print("-> 1: SI5351 VFO Calibration");
  tft.setCursor(30, 60);
  tft.print("   2: SI5351 BFO Calibration");
  tft.setCursor(30, 80);
  tft.print("   3: AD8307 Calibration");
  tft.setCursor(30, 120);
  tft.print("   4: Exit");
  
	while(f_button == 3)
	{
	if (c_setup_menu_item != setup_menu_item)
    {
    c_setup_menu_item = setup_menu_item;
    
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(30, 20);
    tft.print("Setup");
  
    tft.setCursor(30, 40);
    if (c_setup_menu_item == 1)
      tft.print("-> 1: SI5351 VFO Calibration");
    else
      tft.print("   1: SI5351 VFO Calibration");
    
    tft.setCursor(30, 60);
    if (c_setup_menu_item == 2)
      tft.print("-> 2: SI5351 BFO Calibration");
    else
      tft.print("   2: SI5351 BFO Calibration");

    tft.setCursor(30, 80);
    if (c_setup_menu_item == 3)
      tft.print("-> 3: AD8307 Calibration");
    else
      tft.print("   3: AD8307 Calibration");

    tft.setCursor(30, 120);
    if (c_setup_menu_item == 4)
      tft.print("-> 4: Exit");
    else
      tft.print("   4: Exit");
    }
   
   switch (setup_select)
      {
      case 0:
        break;  
      case 1:
        calibrate_vfo_si5351();
        setup_select = 0;
        break;  
      case 2:
        calibrate_bfo_si5351();
        setup_select = 0;
        break;        
      case 3:
        calibrate_ad8307();
        setup_select = 0;
        break;  
      }
	delay(1);
	}
	
	c_mhz = 0;
	f_bachange = f_rxchange = f_mchange = f_bchange = f_dchange = -1;
  tft.fillScreen(ILI9341_BLACK);
  setup_display();
}


void calibrate_vfo_si5351(void)
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&DroidSansMono8pt7b);
  
  tft.setCursor(30, 20);
  tft.print("Calibrate VFO Si5351");
  loop_si5351 ( 0 );
}

void calibrate_bfo_si5351(void)
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&DroidSansMono8pt7b);
  
  tft.setCursor(30, 20);
  tft.print("Calibrate BFO Si5351");
  loop_si5351 ( 1 );
}

void calibrate_ad8307(void)
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&DroidSansMono8pt7b);
  
  tft.setCursor(30, 20);
  tft.print("Calibrate ad8307");
}

void LoadEEPROM ()
{
  uint8_t coldstart;

  coldstart = EEPROM.read(0);               // Grab the coldstart byte indicator in EEPROM for
                                             // comparison with the COLDSTART_REFERENCE
  // Initialize all memories if first upload or if COLDSTART_REF has been modified
  // either through PSWR_A.h or through Menu functions
  if (coldstart != COLDSTART_REF)
  { 
    EEPROM.write(0,COLDSTART_REF);          // COLDSTART_REF in first byte indicates all initialized
    EEPROM_writeAnything(1,R);              // Write default settings into EEPROM
    EEPROM.commit ();
  }
  else                                      // EEPROM contains stored data, retrieve the data
  {
    EEPROM_readAnything(1,R);               // Read the stored data
  }
}

void SaveEEPROM ()
{
    EEPROM.write(0,COLDSTART_REF);          // COLDSTART_REF in first byte indicates all initialized
    EEPROM_writeAnything(1,R);              // Write default settings into  
    EEPROM.commit ();
}
