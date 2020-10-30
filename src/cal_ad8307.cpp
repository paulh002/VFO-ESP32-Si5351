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

extern uint8_t setup_menu_item, c_setup_menu_item, setup_select;


// These defines to aid readability of code
#define CAL_BAD  0                                      // Input signal of insufficient quality for calibration
#define CAL_FWD 1                                      // Good input signal detected, forward direction
#define CAL_REV 2                                      // Good input signal detected, reverse direction (redundant)
// Below variable can take one of the three above defined values, based on the
#define CAL_SET0_MENU 0
// detected input "calibration" signal


void one_level_calibration(void)
{
  int16_t value=0, value_old = 0, cal_set = 0, menu_level;
  int16_t val_sub = 0;
  int16_t val = 0;
  char    lcd_buf[80];
  int     count;
  int16_t x1, y1; 
  uint16_t w, h;
  
  static uint8_t  cal_sig_direction_quality = 0, cal_sig_direction_quality_old = 3;
   
	tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&DroidSansMono8pt7b);
  tft.setCursor(30, 20);
  tft.print("One level calibration");

  menu_level = CAL_SET0_MENU;
  cal_set = 0;
  value = R.cal_AD[cal_set].db10m;
      
  f_button = 4;
  while(f_button == 4)
   {
      pswr_sync_from_interrupt();
  
      // Determine direction and level of calibration signal input
      // Check forward direction and sufficient level
      if ( ((adc_ref*(fwd - rev)/4096.0 * 1000/LOGAMP2_SLOPE) > CAL_INP_QUALITY)
          && (cal_sig_direction_quality != CAL_FWD))
      {
        cal_sig_direction_quality = CAL_FWD;
      }
      // Check reverse direction and sufficient level
      else if ( ((adc_ref*(rev - fwd)/4096.0 * 1000/LOGAMP1_SLOPE) > CAL_INP_QUALITY)
               && (cal_sig_direction_quality != CAL_REV))
      {
        cal_sig_direction_quality = CAL_REV;
      }
      // Check insufficient level
      else if (  (adc_ref*(ABS((fwd - rev))/4096.0 * 1000/LOGAMP2_SLOPE) <= CAL_INP_QUALITY)
               && (cal_sig_direction_quality != CAL_BAD))
      {
        cal_sig_direction_quality = CAL_BAD;
      }
        
      val_sub = value;
      val = val_sub / 10;
      val_sub = val_sub % 10;
      if (value < 0)
          {
            val*=-1;
            val_sub*=-1;
            sprintf(lcd_buf,"-%1u.%01u",val, val_sub);
          }
      else
          {
            sprintf(lcd_buf,"%2u.%01u",val, val_sub);
          }
      
      if (value != value_old)
      {
        value_old = value;
        tft.getTextBounds("40.0",30, 40, &x1, &y1, &w, &h);
        tft.fillRect(x1,40 - h + 1, ILI9341_TFTHEIGHT - x1, h,ILI9341_BLACK);
        tft.setCursor(30, 40);
        tft.print("Adjust (dBm) level: ");
        tft.print(lcd_buf);
      }
      if (xQueueReceive(rotary_queue, &count, 0))
      {
        value = value + count;
        int16_t max_value = 530;                             // Highest permissible Calibration value in dBm * 10
        int16_t min_value = -100;                            // Lowest permissible Calibration value in dBm * 10
        if(value > max_value) value = max_value;
        if(value < min_value) value = min_value;
        R.cal_AD[cal_set].db10m = value;
      }
    
      if (cal_sig_direction_quality != cal_sig_direction_quality_old)
        {
        cal_sig_direction_quality_old = cal_sig_direction_quality;
        tft.getTextBounds("40.0",30, 80, &x1, &y1, &w, &h);
        tft.fillRect(x1,80 - h + 1, ILI9341_TFTHEIGHT - x1, h,ILI9341_BLACK);
        tft.setCursor(30, 80);   
        switch(cal_sig_direction_quality)
          {
          case CAL_FWD:
             tft.print("Forward power");
             break;
          case CAL_REV:
             tft.print("Reverse power");
             break;
          case CAL_BAD:
             tft.print("Bad power");
             break;
          }
        }
   } 
   tft.getTextBounds("Forward Power",30, 80, &x1, &y1, &w, &h);
   tft.fillRect(0,80 - h, ILI9341_TFTHEIGHT, h+2,ILI9341_BLACK);

   if (cal_sig_direction_quality == CAL_FWD)
    {
    if (menu_level == CAL_SET0_MENU)
      {
        R.cal_AD[0].Fwd = adc_ref * fwd/4096.0;
        R.cal_AD[0].Rev = R.cal_AD[0].Fwd;
        // Set second calibration point at 30 dB less
        R.cal_AD[1].db10m = R.cal_AD[0].db10m - 300;
        R.cal_AD[1].Fwd = R.cal_AD[0].Fwd - LOGAMP1_SLOPE * .001 * 30;
        R.cal_AD[1].Rev = R.cal_AD[0].Fwd - LOGAMP2_SLOPE * .001 * 30;
      }
      else
      {
        R.cal_AD[cal_set].Fwd = adc_ref * fwd/4096.0;
        R.cal_AD[cal_set].Rev = R.cal_AD[cal_set].Fwd;
      }
    EEPROM_writeAnything(1,R);
    tft.setCursor(30, 80);
    tft.print("Value Stored");  
    }
    else if (cal_sig_direction_quality == CAL_REV)
    {
      if (menu_level == CAL_SET0_MENU)
      {
        R.cal_AD[0].Rev = adc_ref * rev/4096.0;
        // Set second calibration point at 30 dB less
        R.cal_AD[1].Rev = R.cal_AD[0].Rev - LOGAMP2_SLOPE * .001 * 30;
      }
      else
      {
        R.cal_AD[cal_set].Rev = adc_ref * rev/4096.0;
      }
    EEPROM_writeAnything(1,R);
    tft.setCursor(30, 80);
    tft.print("Value Stored"); 
    }
    else
    {
    tft.setCursor(30, 80);
    tft.print("Nothing Stored");   
    }
delay(1000);
}

void first_level_calibration(void)
{
	
}

void second_level_calibration(void)
{
	
}
