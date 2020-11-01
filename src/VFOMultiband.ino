/*------Hard ware Configuration ---------------------
 * 
 * HF Multiband SMD 2x SI5351!!
 * AS169 based bpf filter
 * Need to add lpf for Power amp and RX-Tx relais switch
 * 
<<ESP32-DevKitC>>
pin No.  Connection
  17 :  Rotary Encoder A
  16 :  Rotary Encoder B

  22 :  si5351A SCL
  21 :  si5351A SDA

  18 :  SCLK      / ILI 9143 (320x240 display)  
  23 :  SDO MOSI  / ILI 9143 (320x240 display)
   5 :  CS        / ILI 9143 (320x240 display)  
   2 :  DC        / ILI 9143 (320x240 display)
  15 :  RESET     / ILI 9143 (320x240 display)


<<si5351A>>
CLK0 : Car Signal (I)
CLK1 : Car Signal (Q)
CLK2 : Lo Signal
------------------------------------------------*/


#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <AceButton.h>
using namespace ace_button;

#include <Adafruit_ILI9341.h>
#include "Fonts/FreeSans18pt7b.h"
#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeMonoBold24pt7b.h"

#include <ESP32Encoder.h>

#include "driver/pcnt.h"
#include "i2c.h"
#include "si5351.h"
#include "measurement.h"
#include "ringmeter.h"
#include "setup.h"

#define Xw 320
#define Yw 240

#define Nx Xw
#define Ny Yw

/*-------------------------------------------------------
   S-Meter Connection and TXRX switch
--------------------------------------------------------*/

//#define RIT_SWITCH  32  
#define TXRX_SWITCH   39


#define SMETER_X     40
#define SMETER_Y     200
#define TMETER_X     40
#define TMETER_Y     165
#define SMETER_DAIL_X 0
#define SMETER_DAIL_Y 210
#define TMETER_DAIL_X 0
#define TMETER_DAIL_Y 175

uint16_t smeterval=0;
uint16_t tmeterval=0;
uint16_t ref_meterval=0;
uint16_t smeterval_old=0;
uint16_t tmeterval_old=0;
uint16_t ref_meterval_old=0;

#define SMETER_TEXT_COLOR2 tft.color565(255,0,0)
#define SMETER_TEXT_COLOR1 tft.color565(0,255,255)
#define TMETER_TEXT_COLOR1 tft.color565(250,250,0)
#define TMETER_TEXT_COLOR2 tft.color565(250,0,0)
#define REF_METER_TEXT_COLOR1 tft.color565(250,250,0)
#define REF_METER_TEXT_COLOR2 tft.color565(250,0,0)
#define POWER_COLOR ILI9341_OLIVE




/*-------------------------------------------------------
   74HC595 Connection
--------------------------------------------------------*/

#define SER   13  // (pin 14)
#define SRCLK 12  // (pin 11)
#define RCLK  25  // (pin 12)

/*-------------------------------------------------------
   Adafruit_ILI9341 hardware connection
--------------------------------------------------------*/

#define SCLK  18 // SPI clock pin
#define SDO -1 // MISO(master input slave output) not using
#define SDI 23 // MOSI(master output slave input) pin
#define CS  5 // Chip Select pin
#define DC  2 // Data Command pin
#define RST_PIN 15 // Reset pin

Adafruit_ILI9341 tft = Adafruit_ILI9341(CS, DC);

/*-------------------------------------------------------
   Rotary hardware connection (used for bandswitching, and mode switching)
--------------------------------------------------------*/

#define ROTARY_A      19      //      27  //  pin
#define ROTARY_B      36      //      26  //  pin
#define ROTARY_PRESS  26      //      14  //  pin

volatile int rotState = 0;
volatile int rotAval = 1;
volatile int rotBval = 1;

ESP32Encoder   Enc_band;

/*-------------------------------------------------------
   Optical Rotary encoder settings (used for frequency)
--------------------------------------------------------*/
#define PULSE_INPUT_PIN 34  // Rotaty Encoder A
#define PULSE_CTRL_PIN  35  // Rotaty Encoder B

ESP32Encoder    Enc_vfo;
AceButton       rotary_button(ROTARY_PRESS);
void            rotary_button_eventhandler(AceButton*, uint8_t, uint8_t);
 
/*-------------------------------------------------------
   Si5351
--------------------------------------------------------*/

#define I2C_SDA2  16
#define I2C_SCL2  17
//#define SI5351_XTAL_FREQ1 SI5351_XTAL_FREQ //32000000
#define SI5351_XTAL_FREQ1 32000000

#define CLK_BFO_RX           SI5351_CLK2
#define CLK_VFO_RX           SI5351_CLK2
#define CLK_BFO_TX           SI5351_CLK1
#define CLK_VFO_TX           SI5351_CLK1
#define CLK_NA               SI5351_CLK0



Si5351 si5351;
Si5351 si5351_bfo(SI5351_BUS_BASE_ADDR, I2C_SDA2, I2C_SCL2);

/*-------------------------------------------------------
   Frequency settings
--------------------------------------------------------*/

const unsigned long bandswitch[] = {160,80,40,20,15,10}; 
const  uint8_t bandconf[] = {0,1,1,1,0,1}; 
const unsigned long freqswitch_low[] = {1800000,3500000,7000000,14000000,21000000,28000000};
const unsigned long freqswitch_high[] = {1880000,3800000,7200000,14350000,21450000,29000000};
long  current_frq[] = {1800000,3500000,7000000,14000000,21000000,28000000};

#define bmax 5

uint8_t f_band = 1;  // initital band
uint8_t c_band = 0;  // detect band change
uint8_t c_rxtx = -1;  // detect rx_tx change
uint8_t f_rxtx = 0;  // initial rx
#define BAND_X 250 
#define BAND_Y 60 
#define RX_X 200 
#define RX_Y 60 
#define BAND_BACKGROUND_COLOR ILI9341_BLACK
#define BAND_TEXT_COLOR tft.color565(255,128,0)

#define POWER_X 200 
#define POWER_Y 104 

volatile long oldfreq = 0;
volatile long currentfreq = 0;
volatile uint32_t freq = freqswitch_low[f_band];

/*-------------------------------------------------------
   Mode (USV, LSB, CW) settings
--------------------------------------------------------*/

uint8_t c_mode = 1;  // if need to renew display, set this flag to 1
uint8_t f_mode = 0;  //
#define USB_x 70 
#define USB_y 86 
#define LSB_x 0 
#define LSB_y 86 

#define BUTTON_x 0 
#define BUTTON_y 112 
#define BUTTON_MODE_TEXT_COLOR ILI9341_WHITE
#define MAX_BUTTON  3
uint8_t   pressed = 0, pressed_old = 10;
uint8_t   c_button = -1, f_button = 0;

#define MODE_BACKGROUND_COLOR ILI9341_DARKGREY
#define MODE_SELTEXT_COLOR tft.color565(255,255,0)
#define MODE_NSELTEXT_COLOR 0

#define USB_FREQUENCY 9001000 //9000500 //8998000 
#define LSB_FREQUENCY 8997000 //8998000 //8995000 

#define F_MAX_MODE  1
#define BFO_STEP 100  // BFO adjust 10Hz
#define BFO_MIN 8900000 
#define BFO_MAX 9008450 
#define BFO_COLOR ILI9341_MAGENTA
#define BFO_Y 60
#define BFO_X 5

/*-------------------------------------------------------
   Bandswitching and RX/TX switching
--------------------------------------------------------*/
// SMD PCB
#define SW_RX     0x55    //0101 0101
#define SW_TX     0x95    //1001 0000 // 0000 0101


// Band pass filter
#define BP_160M 0x55    //01010101  (all off)   01 = off
#define BP_80M 0x56     //01010110 
#define BP_40M 0x59     //01011001
#define BP_20M 0x65     //01100101
#define BP_15M 0x55     //01010101  (all off)
#define BP_10M 0x95     //10010101   

// Low pass filter
// Pin 1 Q4 0x08   80m
// Pin 2 Q5 0x10   40m
// Pin 3 Q7 0x40   20m
// Pin 4 Q3 0x04   15m
// Pin 5 Q2 0x02   10m
// Pin 6 Q1 0x01   Not used / No filter yet fitted
// Pin 7 Q6 0x20   RX/TX
#define LP_160M 0x01
#define LP_80M 0x08
#define LP_40M 0x10
#define LP_20M 0x40
#define LP_15M 0x04
#define LP_10M 0x02

#define LP_TX  0x20

/*-------------------------------------------------------
   Frequency offsets
--------------------------------------------------------*/

uint32_t   offset_frq = LSB_FREQUENCY;  // Offset Frequency[Hz]
uint32_t   bfo_frq    = LSB_FREQUENCY;     // BFO Frequency[Hz]
uint32_t   mfo_frq    = 26000000UL;        // MF frequency [HZ]

#define freq_step 10           // step[Hz]

/*----------------------------------------------------------------------------------
    Control flags
-----------------------------------------------------------------------------------*/
uint8_t f_fchange = 1;  // if frequency changed, set this flag to 1
uint8_t f_bchange = 1;  // if bfo frequency changed set this flag to 1
uint8_t f_dchange = 0;  // if need to renew display, set this flag to 1
uint8_t f_mchange = 1;
uint8_t f_rxchange = 1;
uint8_t f_bachange = 1;

/*-----------------------------------------------------------------------------
 *       Global
-----------------------------------------------------------------------------*/
long    frq;      // Contains vfo frequency
int16_t RE_Count = 0;
uint8_t f_redraw; 
uint8_t c_mhz=0;  // Only draw Mhz once

SemaphoreHandle_t swrBinarySemaphore = NULL;
QueueHandle_t     rotary_queue = NULL;

/*--------------------------------------------------------------------------
        Initialize 74HC595
---------------------------------------------------------------------------*/
//Pin connected to ST_CP pin 12 of 74HC595
int latchPin = RCLK;
//Pin connected to SH_CP pin 11 of 74HC595
int clockPin = SRCLK;
////Pin connected to DS of 74HC595
int dataPin = SER;

void filter_init(void){ 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
 }

void shiftOut(byte bpf, byte lpf) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low
  byte myDataOut;

//  Serial.println("Shift Out"); 


//internal function setup
  int i=0, ii=0;
  int pinState;
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

//clear everything out just in case to
//prepare shift register for bit shifting
  digitalWrite(dataPin, 0);
  digitalWrite(clockPin, 0);
  digitalWrite(latchPin, 0);

  // for each 74HC595
  for (ii=0; ii<3; ii++)
  {
    switch (ii)
    {
      case 0:
        myDataOut = lpf;
        if (f_rxtx)
          myDataOut |= LP_TX;
        break;
      
      case 1:
        if (f_rxtx)
          myDataOut = SW_TX;   
        else
          myDataOut = SW_RX;
        break;

      case 2:
        myDataOut = bpf;   
        break;
    }   
  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
    for (i=7; i>=0; i--)  {
      digitalWrite(clockPin, 0);
      //if the value passed to myDataOut and a bitmask result 
      // true then... so if we are at i=6 and our value is
      // %11010100 it would the code compares it to %01000000 
      // and proceeds to set pinState to 1.
      if ( myDataOut & (1<<i) ) {
        pinState= 1;
      }
      else {  
        pinState= 0;
      }

      //Sets the pin to HIGH or LOW depending on pinState
      digitalWrite(dataPin, pinState);
      //register shifts bits on upstroke of clock pin  
      digitalWrite(clockPin, 1);
      //zero the data pin after shift to prevent bleed through
      digitalWrite(dataPin, 0);
      delay(1);
    }
//  Serial.println(myDataOut);
  }
  //stop shifting
  digitalWrite(clockPin, 0);
  digitalWrite(latchPin, 1);
}


  
/*--------------------------------------------------------------------------
        Initialize and clear Display
---------------------------------------------------------------------------*/

void display_init(void){ 

  tft.begin();
  tft.setRotation(1);

  tft.fillScreen(ILI9341_BLACK);
}

/*--------------------------------------------------------------------------
       S-Meter
---------------------------------------------------------------------------*/

void display_smeter(){
  int16_t x1, y1; 
  uint16_t w, h;
  
 if (f_rxtx == 0)
 {
  smeterval=analogRead(S_METER);
  smeterval=(10 * smeterval)/4096;
 }
 else
 {
  smeterval = 0;
  if (rev_power_db > 0.0)
  smeterval = floor(rev_power_db / 6.0) ;
 }
 if (smeterval_old == smeterval)
  return;
 smeterval_old = smeterval;
 if (smeterval>10){smeterval=10;}
 tft.setFont(&FreeMonoBold24pt7b);
 tft.getTextBounds("----------", (int16_t) SMETER_X, (int16_t) SMETER_Y, &x1, &y1, &w, &h);
 tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,ILI9341_BLACK);  
 tft.setCursor(SMETER_X,SMETER_Y);
 if(f_rxtx == 0)
 {
   for(int i=1;i<=(10 - smeterval);i++)
   {
    if (i<=6)
      {
        tft.setTextColor(SMETER_TEXT_COLOR1);
        tft.print("-");
      }
    else
      {
        tft.setTextColor(SMETER_TEXT_COLOR2);
        tft.print("-");
      }
    }
 }
 else
 {
  for(int i=1;i<=smeterval;i++)
   {
    if (i<=6)
      {
        tft.setTextColor(SMETER_TEXT_COLOR1);
        tft.print("-");
      }
    else
      {
        tft.setTextColor(SMETER_TEXT_COLOR2);
        tft.print("-");
      }
    } 
 }
vTaskDelay(10); 
}

void display_tmeter(){
  int16_t x1, y1; 
  uint16_t w, h;
  
 if (f_rxtx == 0)
 {
  tmeterval = 0;
 }
 else
 {
  tmeterval = 0;
  if (power_db > 0.0)
    tmeterval = floor(power_db / 6.0) ;
 }
 
 if (tmeterval_old == tmeterval)
  return;
 tmeterval_old = tmeterval;
 if (tmeterval>10){tmeterval=10;}
  tft.setFont(&FreeMonoBold24pt7b);
  tft.getTextBounds("----------", (int16_t) TMETER_X, (int16_t) TMETER_Y, &x1, &y1, &w, &h);
  tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,ILI9341_BLACK);  
  tft.setCursor(TMETER_X,TMETER_Y);
  for(int i=1;i<=tmeterval;i++){
    if (i<=6){
      tft.setTextColor(TMETER_TEXT_COLOR1);
      tft.print("-");
    }
    else{
      tft.setTextColor(TMETER_TEXT_COLOR2);
      tft.print("-");
    }
  }
vTaskDelay(10); 
}


/*--------------------------------------------------------------------------
        Display the current bfo frequence
---------------------------------------------------------------------------*/
void display_bfo()
{
  char str[64];
  int16_t x1, y1; 
  uint16_t w, h;
    
  if(f_bchange==1)
  {
    tft.setFont(&FreeSans9pt7b);
    tft.getTextBounds("9.000,00 Mhz", (int16_t) BFO_X, (int16_t) BFO_Y, &x1, &y1, &w, &h);
    tft.setCursor(BFO_X, BFO_Y);
    tft.setTextColor(BFO_COLOR);  
  
  //-------- Display Digital Frquency ---------------------------------------
    sprintf(str, "bfo:%1d.%03d,%02d ",  bfo_frq/1000000, (bfo_frq/1000)%1000, (bfo_frq/10)%100 );
    tft.getTextBounds("bfo: 9.000,00   ",BFO_X, (int16_t) BFO_Y, &x1, &y1, &w, &h);
    tft.fillRect(BFO_X,BFO_Y - h + 1,w,h,0);
    tft.drawRoundRect(0,BFO_Y - h,w+2,h+4,5,BFO_COLOR);
    tft.print(str);
  }
}

/*--------------------------------------------------------------------------
        Display the current bfo frequence
---------------------------------------------------------------------------*/

#define X_SWR_RING  250
#define Y_SWR_RING  70
#define R_SWR_RING  35 // radius


#define X_PEP_RING  160
#define Y_PEP_RING  70
#define R_PEP_RING  35 // radius




uint16_t fpower = 1;

void display_power()
{
  char str[82], str1[82];
  int16_t x1, y1; 
  uint16_t w, h , h1, swr, pep;
    
  if(f_rxtx==1)
  {
/*  tft.setFont(&FreeSans9pt7b);
    tft.getTextBounds("PEP:1.00KW", POWER_X, POWER_Y, &x1, &y1, &w, &h);
    tft.setCursor(BAND_X, BAND_Y + 60);
    tft.setTextColor(POWER_COLOR);  
    print_p_mw(power_mw_pep);
    
  //-------- Display Power ---------------------------------------
    sprintf(str, "PEP: %s", lcd_buf  );
    tft.getTextBounds("PEP: 1000pW",POWER_X, POWER_Y, &x1, &y1, &w, &h);  
    tft.fillRect(BAND_X,BAND_Y + 60 - h + 1, ILI9341_TFTHEIGHT - POWER_X, h + 3,ILI9341_BLACK);
    tft.print(str);
*/   
//-------- Display SWR ---------------------------------------
/*    h1 = h + 4;
    tft.setCursor(POWER_X, POWER_Y+h1);
    tft.setTextColor(POWER_COLOR);  
    swr = print_swr();
    sprintf(str, "SWR: %s", lcd_buf  );
    tft.setFont(&FreeSans9pt7b);
    tft.getTextBounds("PEP: 1000pW",POWER_X, POWER_Y, &x1, &y1, &w, &h);
    tft.fillRect(POWER_X,POWER_Y +h1 - h + 1, ILI9341_TFTHEIGHT - POWER_X, h + 3,ILI9341_BLACK);
    tft.print(str); */
     
    swr = print_swr();
    pep = power_mw_pep / 1000.0 ;//print_p_mw(power_mw_pep);
    
    //Serial.print("power_mw_pep : ");
    //Serial.println(power_mw_pep);
    //Serial.print("power_db : ");
    //Serial.print(power_db);
    //Serial.println("dbm");
    
    ringMeter((int)swr, 0, 500, X_SWR_RING, Y_SWR_RING, R_SWR_RING, (char *) "SWR", GREEN3RED,2);
    ringMeter((int)pep, 0, 100, X_PEP_RING, Y_PEP_RING, R_PEP_RING, (char *) "PEP", GREEN2RED,0);
    fpower = 1;
  }
  else
  {
    
    if (fpower)
    {
    /*tft.setFont(&FreeSans9pt7b);
    tft.getTextBounds("PEP: 1000pW",POWER_X, POWER_Y, &x1, &y1, &w, &h);
    tft.fillRect(POWER_X,POWER_Y - h,w,h+3,ILI9341_BLACK);  
    h1 = h + 4;
    tft.getTextBounds("PEP: 1000pW",POWER_X, POWER_Y, &x1, &y1, &w, &h);
    tft.fillRect(POWER_X,POWER_Y - h + h1,w,h+3,ILI9341_BLACK);  
    */

    tft.fillRect(X_SWR_RING,Y_SWR_RING,R_SWR_RING * 2,R_SWR_RING *2,ILI9341_BLACK);  
    tft.fillRect(X_PEP_RING,Y_PEP_RING,R_PEP_RING * 2,R_PEP_RING *2,ILI9341_BLACK);  
    
    fpower = 0;
    }
  }
}

void switch_band()
{
    switch (c_band)
    {
    case 0:
      shiftOut(BP_160M,LP_160M);
      break;
      
    case 1:    
      shiftOut(BP_80M,LP_80M);
      break;
      
    case 2:
      shiftOut(BP_40M,LP_40M);
      break;
  
    case 3:
      shiftOut(BP_20M,LP_20M);
      break;
    
    case 4:
      shiftOut(BP_15M,LP_15M);
      break;
    
    case 5:
      shiftOut(BP_10M,LP_10M);
      break;
    }
}  

void display_rx_tx()
{
  int16_t x1, y1; 
  uint16_t w, h;
  
 if (digitalRead(TXRX_SWITCH))
   f_rxtx = 0;
 else
   f_rxtx = 1;
 
 if ((c_rxtx != f_rxtx) || f_rxchange)
  {
    setup_smeter();
    c_rxtx = f_rxtx;
    f_rxchange = 0;
    
    f_dchange = 1;
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextColor(BAND_TEXT_COLOR);
    tft.setCursor(RX_X,RX_Y);
    tft.getTextBounds("RX", (int16_t)RX_X,(int16_t)RX_Y, &x1, &y1, &w, &h); // use enough space to draw background
    tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
    if (c_rxtx == 0)
      {
        tft.print("RX");
        si5351_bfo.output_enable(CLK_BFO_RX, 1);
        si5351_bfo.output_enable(CLK_BFO_TX, 0);
        si5351.output_enable(CLK_VFO_RX, 1);
        si5351.output_enable(CLK_VFO_TX, 0);
      }
    else
      {
        tft.print("TX");
        si5351_bfo.output_enable(CLK_BFO_RX, 0);
        si5351_bfo.output_enable(CLK_BFO_TX, 1);
        si5351.output_enable(CLK_VFO_RX, 0);
        si5351.output_enable(CLK_VFO_TX, 1);
      }
    switch_band(); // put bpf and lpf in tx mode
  }
}      

/*--------------------------------------------------------------------------
        Display the current band 160,80,40,20,15,10
---------------------------------------------------------------------------*/
void display_band()
{

  int16_t x1, y1; 
  uint16_t w, h;
  
if ((c_band != f_band) || f_bachange)
  {
    c_band = f_band;
    f_bachange = 0;
    
    switch(c_band)
    {
    case 0:
      f_mode = 0;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(BAND_X,BAND_Y);
      tft.getTextBounds("160 M", (int16_t)BAND_X,(int16_t)BAND_Y, &x1, &y1, &w, &h); // use enough space to draw background
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print("160 M");
      break;
  
    case 1:
      f_mode = 0;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(BAND_X,BAND_Y);
      tft.getTextBounds("160 M", (int16_t)BAND_X,(int16_t)BAND_Y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 80 M");
      break;
  
    case 2:
      f_mode = 0;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(BAND_X,BAND_Y);
      tft.getTextBounds("160 M", (int16_t)BAND_X,(int16_t)BAND_Y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 40 M");
      break;
  
  case 3:
      f_mode = 1;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(BAND_X,BAND_Y);
      tft.getTextBounds("160 M", (int16_t)BAND_X,(int16_t)BAND_Y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 20 M");
      break;
      
  case 4:
      f_mode = 1;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(BAND_X,BAND_Y);
      tft.getTextBounds("160 M", (int16_t)BAND_X,(int16_t)BAND_Y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 15 M");
      break;

  case 5:
      f_mode = 1;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(BAND_X,BAND_Y);
      tft.getTextBounds("160 M", (int16_t)BAND_X,(int16_t)BAND_Y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 10 M");
      break;
      }
   switch_band();
  }
} 


void display_button()
{
  int16_t x1, y1; 
  uint16_t w, h;
  
if (c_button != f_button)
  {
    c_button = f_button;
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(BUTTON_MODE_TEXT_COLOR);
    tft.setCursor(BUTTON_x,BUTTON_y);
    tft.getTextBounds("Mode: Band switch", (int16_t)BUTTON_x,(int16_t) BUTTON_y, &x1, &y1, &w, &h);
    tft.fillRoundRect(x1-2,y1-2,w+4,h+6,5,ILI9341_BLACK);
    tft.setTextColor(BUTTON_MODE_TEXT_COLOR);
    switch(f_button)
    {
      case 0:
        tft.print("Band switch"); 
        break;
      case 1:
        tft.print("Mode switch"); 
        break;
      case 2:
        tft.print("BFO Adjust"); 
        break;      
      case 3:
        tft.print("Setup"); 
        break;     
    }
  }
}
/*--------------------------------------------------------------------------
        Display the current Mode LSB, USB, ..
---------------------------------------------------------------------------*/

void display_mode()
{
  int16_t x1, y1; 
  uint16_t w, h;
  
if ((c_mode != f_mode) || (f_mchange))
  {
    c_mode = f_mode;
    f_mchange = 0;
    
    switch (f_mode)
    {
    case 0:
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(MODE_NSELTEXT_COLOR);
      tft.setCursor(USB_x,USB_y);
      tft.getTextBounds("USB", (int16_t)USB_x,(int16_t)USB_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,MODE_BACKGROUND_COLOR);  
      tft.print("USB");
      tft.setCursor(LSB_x,LSB_y);
      tft.getTextBounds("LSB", (int16_t)LSB_x,(int16_t) LSB_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,MODE_BACKGROUND_COLOR);
      tft.setTextColor(MODE_SELTEXT_COLOR);
      tft.print("LSB");
      offset_frq = LSB_FREQUENCY;
      bfo_frq = LSB_FREQUENCY;
      f_bchange = 1;
      break;
  
    case 1:
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(MODE_SELTEXT_COLOR);
      tft.setCursor(USB_x,USB_y);
      tft.getTextBounds("USB", (int16_t)USB_x,(int16_t)USB_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,MODE_BACKGROUND_COLOR);  
      tft.print("USB");
      tft.setCursor(LSB_x,LSB_y);
      tft.getTextBounds("LSB", (int16_t)LSB_x,(int16_t) LSB_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,MODE_BACKGROUND_COLOR);
      tft.setTextColor(MODE_NSELTEXT_COLOR);
      tft.print("LSB");
      offset_frq = USB_FREQUENCY;
      bfo_frq = USB_FREQUENCY; 
      f_bchange = 1;
      break;

    case 2:
      break;
  
    case 3:
      break;
    }
  }
}

/*--------------------------------------------------------------------------
        Display the frequency in a box and call other display functions
---------------------------------------------------------------------------*/

void UpdateDisplay()
{
  char str[64];
  int16_t x1, y1; 
  uint16_t w, h;
    
  tft.setFont(&FreeSans18pt7b);
  tft.getTextBounds("29.000,00 Mhz", (int16_t) 5, (int16_t) 0, &x1, &y1, &w, &h);
  tft.setCursor(Nx /2 - (w / 2), h);
  tft.setTextColor(ILI9341_OLIVE);  
  
  //-------- Display Digital Frquency ---------------------------------------
  sprintf(str, "%3d.%03d,%02d ",  frq/1000000, (frq/1000)%1000, (frq/10)%100 );
  tft.getTextBounds("29.000,00   ", Nx /2 - (w / 2), (int16_t) h, &x1, &y1, &w, &h);
//  tft.getTextBounds(str, Nx /2 - (w / 2), (int16_t) h, &x1, &y1, &w, &h);
  tft.drawRoundRect(0,0,Nx,h+8,10,ILI9341_OLIVE);
  tft.fillRect(x1,y1,w,h,0);
  tft.print(str);

  
  if (c_mhz == 0 )
  {
    c_mhz=1;
    tft.setFont(&FreeSans18pt7b);
    tft.getTextBounds("29.000,00 ", Nx /2 - (w / 2), (int16_t) h, &x1, &y1, &w, &h);
    tft.setCursor(x1+w, h);
    tft.print("Mhz");
  }       
  display_band(); // first band 
  display_mode(); // also changes usb / lsb if band changes
  display_rx_tx(); // rx_tx can change bfo frequencies
  display_bfo();  // also change bfo if mode changes
  display_power();
}

/*--------------------------------------------------------------------------
        Display the background text once by setup
---------------------------------------------------------------------------*/

void setup_display()
 {
 UpdateDisplay();
 setup_smeter();
 tft.setCursor(10,230);
 tft.setTextColor(tft.color565(100,100,100));
 tft.print("     HF Tranceiver Ver1.0 PA0PHH");  
 }

void setup_smeter()
 {
  char str[64];
  int16_t x1, y1; 
  uint16_t w, h;
  
 if (f_rxtx)
 {
    tft.setFont(&FreeSans9pt7b);
    tft.getTextBounds("1-----3-------6-------9---Over--------",SMETER_DAIL_X, SMETER_DAIL_Y, &x1, &y1, &w, &h);
    tft.fillRect(SMETER_DAIL_X,SMETER_DAIL_Y - h + 1, ILI9341_TFTHEIGHT - SMETER_DAIL_X, h,ILI9341_BLACK);
    tft.getTextBounds("4-----8------16------24------32-----40",TMETER_DAIL_X, TMETER_DAIL_Y, &x1, &y1, &w, &h);
    
    tft.fillRect(TMETER_DAIL_X,TMETER_DAIL_Y - h + 1, ILI9341_TFTHEIGHT - TMETER_DAIL_X, h,ILI9341_BLACK);

    tft.setTextColor(ILI9341_DARKGREEN);
    tft.setCursor(SMETER_DAIL_X,SMETER_DAIL_Y);
    tft.print("rev 4-----8------16------24------32-----40");
    tft.setCursor(TMETER_DAIL_X,TMETER_DAIL_Y);
    tft.setTextColor(ILI9341_DARKCYAN);
    tft.print("fwd 4-----8------16------24------32-----40");   
 }
 else
 {
    smeterval = tmeterval = 0;
    tft.setFont(&FreeSans9pt7b);
    tft.getTextBounds("fwd 4-----8------16------24------32-----40",SMETER_DAIL_X, SMETER_DAIL_Y, &x1, &y1, &w, &h);
    tft.fillRect(SMETER_DAIL_X,SMETER_DAIL_Y - h + 1, ILI9341_TFTHEIGHT - SMETER_DAIL_X, h,ILI9341_BLACK);
    tft.getTextBounds("rev 4-----8------16------24------32-----40",TMETER_DAIL_X, TMETER_DAIL_Y, &x1, &y1, &w, &h);
    tft.fillRect(TMETER_DAIL_X,TMETER_DAIL_Y - h + 1, ILI9341_TFTHEIGHT - TMETER_DAIL_X, h,ILI9341_BLACK);
    
    tft.setTextColor(ILI9341_DARKGREEN);
    tft.setCursor(SMETER_DAIL_X,SMETER_DAIL_Y);
    tft.print("S   1-----3-------6-------9---Over--------");
    tft.setCursor(TMETER_DAIL_X,TMETER_DAIL_Y);
    // tft.setTextColor(ILI9341_DARKCYAN);
    // tft.print("rev 4-----8------16------24------32-----40");   
 }
}

void setbfo()
{
  if (f_bchange)
    {
    f_bchange = 0;
    uint64_t freq = (uint64_t)bfo_frq * SI5351_FREQ_MULT;
    si5351_bfo.set_freq(freq, CLK_BFO_RX);
    si5351_bfo.set_freq(freq, CLK_BFO_TX);
    }
}

void setvfo()
{
 if(f_fchange)
    {
    f_fchange=0;
    uint64_t freq = (uint64_t)(frq + offset_frq) * SI5351_FREQ_MULT;
    si5351.set_freq(freq, CLK_VFO_RX);
    si5351.set_freq(freq, CLK_VFO_TX);
    }
 }
      
void setup() {
  bool i2c_found;
  uint64_t reg_a;
  uint64_t reg_b;
  uint64_t reg_c;
  
  Serial.begin(115200);
  Serial.println("VFO PA0PHH !"); 
  frq = freqswitch_low[f_band];


 // set pin modes before display updates
  pinMode(TXRX_SWITCH, INPUT);
  pinMode(S_METER, ANALOG);
  pinMode(FWD_METER, ANALOG);
  pinMode(REV_METER, ANALOG);
  
  // Set pushbutton rotary encoder configuration
  pinMode(ROTARY_PRESS, INPUT);
  rotary_button.setEventHandler(rotary_button_eventhandler);
  rotary_button.getButtonConfig()->setFeature(ButtonConfig::kFeatureLongPress);
  rotary_button.getButtonConfig()->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
  
  adc_init();
  swrBinarySemaphore = xSemaphoreCreateMutex();
  if(swrBinarySemaphore == NULL){
    Serial.println("Error creating the swrBinarySemaphore");
  }
  
  rotary_queue = xQueueCreate( 10, sizeof( int ) );
  if(rotary_queue == NULL){
    Serial.println("Error creating the queue");
  }

  
  display_init();
  setup_display();
  filter_init();

  ESP32Encoder::useInternalWeakPullResistors=false;
  Enc_vfo.attachHalfQuad(PULSE_INPUT_PIN, PULSE_CTRL_PIN);
  Enc_band.attachHalfQuad(ROTARY_A, ROTARY_B);

  Enc_vfo.clearCount();
  Enc_band.clearCount();
  
  xTaskCreatePinnedToCore(task0, "Task0", 4096, NULL, 1, NULL, 0);
  

// Load correction and calibration information
  LoadEEPROM(); 

  sprintf(lcd_buf,"db10m : %d %f %f", R.cal_AD[0].db10m, R.cal_AD[0].Fwd, R.cal_AD[0].Rev);
  Serial.println ( lcd_buf );
  sprintf(lcd_buf,"db10m : %d %f %f", R.cal_AD[1].db10m, R.cal_AD[1].Fwd, R.cal_AD[1].Rev);
  Serial.println ( lcd_buf );
  sprintf(lcd_buf,"Correction 1 : %d", R.correction_si5351_no1);
  Serial.println ( lcd_buf );
  sprintf(lcd_buf,"Correction 2 : %d ", R.correction_si5351_no1);
  Serial.println ( lcd_buf );
  
  if (si5351.init(SI5351_CRYSTAL_LOAD_8PF, SI5351_XTAL_FREQ1, 0) == false)
  {  Serial.println ( "SI5351 not found" ); }

  si5351.set_correction(R.correction_si5351_no1, SI5351_PLL_INPUT_XO);
  si5351.drive_strength(CLK_VFO_RX, SI5351_DRIVE_2MA);
  si5351.drive_strength(CLK_VFO_TX, SI5351_DRIVE_2MA);
  
  if (si5351_bfo.init(SI5351_CRYSTAL_LOAD_8PF, SI5351_XTAL_FREQ1, 0) == false)
  {  Serial.println ( "SI5351 bfo not found" ); }

  si5351_bfo.set_correction(R.correction_si5351_no2, SI5351_PLL_INPUT_XO);
  si5351_bfo.drive_strength(CLK_BFO_RX, SI5351_DRIVE_2MA);
  si5351_bfo.drive_strength(CLK_BFO_TX, SI5351_DRIVE_2MA);

  shiftOut(BP_80M,LP_80M);
  setbfo();  // Auto output enable!
  setvfo();  

  si5351.output_enable(CLK_VFO_RX, 1);
  si5351.output_enable(CLK_VFO_TX, 0);
  si5351_bfo.output_enable(CLK_BFO_RX, 1);
  si5351_bfo.output_enable(CLK_BFO_TX, 0);
}

uint8_t  loop_counter = 0;

void loop() {

  if(f_dchange==1){
    f_dchange=0;
    UpdateDisplay();
  }

  setvfo();
  setbfo();
  if (f_rxtx != 0)
      {
        //adc_poll_and_feed_circular();
        pswr_sync_from_interrupt();
        calc_SWR_and_power();
        if (loop_counter == 0)
            display_power(); // only update power once per 64k
        loop_counter++;
      }
  else
      {
        display_power(); // clear the power meter from the screen done once
      } 
  display_rx_tx();
  display_smeter(); 
  display_tmeter();
  display_button();
  if (f_button == 3)
    setup_menu();
}

void next_band(uint8_t dir, uint8_t &band)
{
  if (dir > 0)
  {
  for (int i = 0; i < bmax; i++)
    {
      if (bandconf[band] == 0) { band++; if (band > bmax) {band = 0; } frq= freqswitch_low[band];} 
      if (bandconf[band] != 0) 
        break; 
    }
  }
  else
  {
  for (int i = 0; i < bmax; i++)
    {
      if (bandconf[band] == 0) { if (band == 0) {band = bmax; } else {band--;}  frq= freqswitch_high[band];} 
      if (bandconf[band] != 0) 
        break; 
    }    
  }
}
/*-----------------------------------------------------------------------------------------------
        Alternative Loop (core0)
------------------------------------------------------------------------------------------------*/
volatile int lastEncoding = 0, lastEncoding1 = 0;


void task0(void* arg)
{

    while (1)
     {    
         uint8_t band = f_band;
         int count = Enc_vfo.getCount();
         Enc_vfo.clearCount();
         
         if (count!=0)
         {                           
            frq+= count * freq_step;
            if(frq>freqswitch_high[band]) { band++; if (band > bmax) {band = 0; } frq= freqswitch_low[band]; next_band(1,band); } 
            if(frq<freqswitch_low[band]) { if (band == 0) {band = bmax; } else {band--;}  frq= freqswitch_high[band]; next_band(0,band);}
            current_frq[band] = frq;

            
            f_band   = band; // to make it more thread save
            f_dchange=1; // send for update display
            f_fchange=1; // update the si5351 vfo frequency
         }
        
         rotary_button.check();        

         count = 0;
         count = Enc_band.getCount();
         Enc_band.clearCount();
         if (count!=0)
             {
              int currMillis = millis();
              if (currMillis - lastEncoding > 100)
                {     
                    switch(f_button)
                    {
                      case 0:
                        band = f_band;
                        if(count > 0) 
                          { if (band == 0) {band = bmax; } else {band--;} next_band(0,band);}
                        else
                          { band++; if (band > bmax) {band = 0; } next_band(1,band);} 
                        
                        frq = current_frq[band];              
                        f_band   = band; // to make it more thread save
                        f_dchange=1;
                        f_fchange=1; // update the si5351 vfo frequency
                        break;
                      case 1:
                        if (f_mode)
                          f_mode = 0;
                        else
                          f_mode = 1;
                        f_bchange = 1;
                        f_dchange=1; // send for update display   
                        break;
                      case 2:
                         f_bchange=1; // Set bfo frequency and display New BFO frequency
                         f_dchange=1; // UpdateDisplay 
                         bfo_frq+= count * BFO_STEP;
                         if (bfo_frq > BFO_MAX) bfo_frq = BFO_MAX;
                         if (bfo_frq < BFO_MIN) bfo_frq = BFO_MIN;
                      break;
                      
                      case 3:
                        if(count > 0) 
                        {
                          switch (setup_menu_item)
                          {
                            case 1:
                              setup_menu_item = 2;
                              break;
                            case 2:
                              setup_menu_item = 3;
                              break;
                            case 3:
                              setup_menu_item = 4;
                              break;
                            case 4:
                              setup_menu_item = 1;
                              break;
                          }
                        }
                        else
                        {
                          switch (setup_menu_item)
                          {
                            case 1:
                              setup_menu_item = 4;
                              break;
                            case 2:
                              setup_menu_item = 1;
                              break;
                            case 3:
                              setup_menu_item = 2;
                              break;
                            case 4:
                              setup_menu_item = 3;
                              break;
                          }
                        }
                        break;
                      case 4:
                        xQueueSend(rotary_queue, &count, portMAX_DELAY);
                        break;
                    }
                lastEncoding = currMillis;
               }
           }
      
     // Sample power on core 0
     if (f_rxtx)
       {
       xSemaphoreTake( swrBinarySemaphore, portMAX_DELAY );
       adc_poll_and_feed_circular();
       xSemaphoreGive( swrBinarySemaphore );
       }
     delay(1);
     }
}

void rotary_button_eventhandler(AceButton*, uint8_t eventType, uint8_t buttonState)
{
  switch (eventType) {
    case AceButton::kEventLongPressed :
      f_button = 3; 
      break;
    case AceButton::kEventReleased:
      switch(f_button)
        {
        case 0:
          f_button = 1;
          break;               
        case 1:
          f_button = 2;
          break;                
        case 2:
          f_button = 0;
          break;
        case 3:
          if (setup_menu_item == 4)
            f_button = 0;
          if (setup_menu_item == 1)
            setup_select = 1;
          if (setup_menu_item == 2)
            setup_select = 2;
          if (setup_menu_item == 3)
            setup_select = 3;
          break;
        case 4:
          f_button = 0;
          break;
        }
      break;
  }
}
