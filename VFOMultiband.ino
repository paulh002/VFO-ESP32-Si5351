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

#include <Adafruit_ILI9341.h>
#include "Fonts/FreeSans18pt7b.h"
#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeMonoBold24pt7b.h"

#include <ESP32Encoder.h>

#include "driver/pcnt.h"
#include "i2c.h"
#include "si5351.h"
#include <EEPROM.h>      // Contains Si5351 crystal calibration frequency

#define Xw 320
#define Yw 240

#define Nx Xw
#define Ny Yw

/*-------------------------------------------------------
   S-Meter Connection and TXRX switch
--------------------------------------------------------*/

//#define RIT_SWITCH  32  
#define TXRX_SWITCH   39
#define S_METER       32 
#define T_METER       33

#define SMETER_X     40
#define SMETER_Y     200
#define TMETER_X     40
#define TMETER_Y     165

uint16_t smeterval=0;
uint16_t tmeterval=0;
uint16_t smeterval_old=0;
uint16_t tmeterval_old=0;

#define SMETER_TEXT_COLOR2 tft.color565(255,0,0)
#define SMETER_TEXT_COLOR1 tft.color565(0,255,255)
#define TMETER_TEXT_COLOR1 tft.color565(250,250,0)
#define TMETER_TEXT_COLOR2 tft.color565(250,0,0)


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

ESP32Encoder   Enc_vfo;

/*-------------------------------------------------------
   Si5351
--------------------------------------------------------*/

#define I2C_SDA2  16
#define I2C_SCL2  17
#define SI5351_XTAL_FREQ1 SI5351_XTAL_FREQ //32000000
#define CLK_BFO_RX           SI5351_CLK2
#define CLK_VFO_RX           SI5351_CLK2
#define CLK_BFO_TX           SI5351_CLK1
#define CLK_VFO_TX           SI5351_CLK1


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
#define band_x 250 
#define band_y 82 
#define rx_x 250 
#define rx_y 60 
#define BAND_BACKGROUND_COLOR ILI9341_DARKGREY
#define BAND_TEXT_COLOR tft.color565(255,128,0)

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
#define MODE_BACKGROUND_COLOR ILI9341_DARKGREY
#define MODE_SELTEXT_COLOR tft.color565(255,255,0)
#define MODE_NSELTEXT_COLOR 0

#define USB_FREQUENCY 9000000 //9000500 //8998000 
#define LSB_FREQUENCY 8997000 //8998000 //8995000 

#define F_MAX_MODE  1
#define BFO_STEP 10  // BFO adjust 10Hz
#define BFO_MIN 8900000 
#define BFO_MAX 9008450 
#define BFO_COLOR ILI9341_MAGENTA
#define BFO_Y 60
#define BFO_X 5

#define EEPROM_SIZE    64   // Size of EEPROM block
int32_t       correction =  0;      // Si5351 correction factor
int16_t       corrAddr   =  0;      // EEPROM address of crystal frequency
int32_t       correction1 =  0;      // Si5351 correction factor
int16_t       corrAddr1   =  8;      // EEPROM address of crystal frequency

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
/*-----------------------------------------------------------------------------
 *       Global
-----------------------------------------------------------------------------*/
long    frq;      // Contains vfo frequency
int16_t RE_Count = 0;
uint8_t f_redraw; 
uint8_t c_mhz=0;  // Only draw Mhz once


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
  
 smeterval=analogRead(S_METER);
// Serial.print(" S METER: ");
// Serial.println(smeterval);
 smeterval=(10 * smeterval)/4096;
 if (smeterval_old == smeterval)
  return;
 smeterval_old = smeterval;
 if (smeterval>10){smeterval=10;}
 tft.setFont(&FreeMonoBold24pt7b);
 tft.getTextBounds("----------", (int16_t) SMETER_X, (int16_t) SMETER_Y, &x1, &y1, &w, &h);
 tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,ILI9341_BLACK);  
 tft.setCursor(SMETER_X,SMETER_Y);
  for(int i=1;i<=(10 - smeterval);i++){
    if (i<=6){
      tft.setTextColor(SMETER_TEXT_COLOR1);
      tft.print("-");
    }
    else{
      tft.setTextColor(SMETER_TEXT_COLOR2);
      tft.print("-");
    }
  }
vTaskDelay(10); 
}

void display_tmeter(){
  int16_t x1, y1; 
  uint16_t w, h;
  
 tmeterval=analogRead(T_METER);
// Serial.print(" T METER: ");
// Serial.println(tmeterval);
 tmeterval= (10 * tmeterval)/4096;
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
    sprintf(str, "Bfo:%1d.%03d,%02d ",  bfo_frq/1000000, (bfo_frq/1000)%1000, (bfo_frq/10)%100 );
    tft.getTextBounds("Bfo: 9.000,00   ",BFO_X, (int16_t) BFO_Y, &x1, &y1, &w, &h);
    tft.fillRect(BFO_X,BFO_Y - h + 1,w,h,0);
    tft.drawRoundRect(0,BFO_Y - h,w+2,h+4,5,BFO_COLOR);
    tft.print(str);
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
 
 if (c_rxtx != f_rxtx)
  {
    c_rxtx = f_rxtx;
    
    f_dchange = 1;
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextColor(BAND_TEXT_COLOR);
    tft.setCursor(rx_x,rx_y);
    tft.getTextBounds("RX", (int16_t)rx_x,(int16_t)rx_y, &x1, &y1, &w, &h); // use enough space to draw background
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
  
if (c_band != f_band)
  {
    c_band = f_band;
    
    switch(c_band)
    {
    case 0:
      f_mode = 0;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,band_y);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h); // use enough space to draw background
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print("160 M");
      break;
  
    case 1:
      f_mode = 0;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,82);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 80 M");
      break;
  
    case 2:
      f_mode = 0;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,band_y);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 40 M");
      break;
  
  case 3:
      f_mode = 1;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,band_y);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 20 M");
      break;
      
  case 4:
      f_mode = 1;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,band_y);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 15 M");
      break;

  case 5:
      f_mode = 1;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,band_y);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 10 M");
      break;
      }
   switch_band();
  }
} 
/*--------------------------------------------------------------------------
        Display the current Mode LSB, USB, ..
---------------------------------------------------------------------------*/

void display_mode()
{
  int16_t x1, y1; 
  uint16_t w, h;
  
if (c_mode != f_mode)
  {
    c_mode = f_mode;
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
}

/*--------------------------------------------------------------------------
        Display the background text once by setup
---------------------------------------------------------------------------*/

void setup_display()
 {
 UpdateDisplay();
 tft.setFont(&FreeSans9pt7b);
 tft.setTextColor(ILI9341_DARKGREEN);
 tft.setCursor(40,210);
 tft.print("1-----3-------6-------9---Over--------");
 tft.setCursor(40,175);
 tft.setTextColor(ILI9341_DARKCYAN);
 tft.print("1-----3------5----------10--------------");  
 tft.setCursor(10,230);
 tft.setTextColor(tft.color565(100,100,100));
 tft.print("     HF Tranceiver Ver1.0 PA0PHH");  
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
 if(f_fchange==1)
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
  pinMode(T_METER, ANALOG);
  
  display_init();
  setup_display();
  filter_init();

  ESP32Encoder::useInternalWeakPullResistors=false;
  Enc_vfo.attachHalfQuad(PULSE_INPUT_PIN, PULSE_CTRL_PIN);
  Enc_band.attachHalfQuad(ROTARY_A, ROTARY_B);

  Enc_vfo.clearCount();
  Enc_band.clearCount();
  
  xTaskCreatePinnedToCore(task0, "Task0", 4096, NULL, 1, NULL, 0);
        
  analogSetAttenuation(ADC_11db);
  analogReadResolution(12);

    
  // Set pushbutton rotary encoder
  pinMode(ROTARY_PRESS, INPUT);

  if ( !EEPROM.begin ( EEPROM_SIZE ))            // Start up the EEPROM library
  {
    Serial.println ( "\nFailed to initialise EEPROM" ); // It failed!
    Serial.print   ( "Initializing the correction factor to: " );
    Serial.println ( correction );
  }
  else
  {
    Serial.println ( "\nChecking for previous correction first factor." );

    correction = EEPROM.readLong ( corrAddr );        // Read what's there

#define LIMIT 10000000L                     // I think this is reasonable

    if (( correction < -LIMIT ) || ( correction > LIMIT ) // Invalid correction factor?
                  || ( correction == -1 ))  // '-1' indicates virgin EEPROM
    {
      correction =0;
      Serial.println ( "\nEEPROM does not contain a valid correction factor." );
      Serial.print   ( "Initializing the correction factor to: " );
      Serial.println ( correction );
    }
    else
    {
      Serial.print ( "EEPROM contains correction factor: " );
      Serial.println ( correction );
      Serial.println ( "We will use that value." );
    }
    Serial.println ( "\nChecking for previous correction second factor." );

    correction1 = EEPROM.readLong ( corrAddr1 );        // Read what's there

    if (( correction1 < -LIMIT ) || ( correction1 > LIMIT ) // Invalid correction factor?
                  || ( correction1 == -1 ))  // '-1' indicates virgin EEPROM
    {
      correction1 =0;
      Serial.println ( "\nEEPROM does not contain a valid correction factor." );
      Serial.print   ( "Initializing the correction factor to: " );
      Serial.println ( correction );
    }
    else
    {
      Serial.print ( "EEPROM contains correction factor: " );
      Serial.println ( correction1 );
      Serial.println ( "We will use that value." );
    }
  }

   
  if (si5351.init(SI5351_CRYSTAL_LOAD_8PF, SI5351_XTAL_FREQ1, 0) == false)
  {  Serial.println ( "SI5351 not found" ); }

  si5351.set_correction(correction, SI5351_PLL_INPUT_XO);
 
  si5351.drive_strength(CLK_VFO_RX, SI5351_DRIVE_2MA);
  si5351.output_enable(CLK_VFO_RX, 1);

  si5351.drive_strength(CLK_VFO_TX, SI5351_DRIVE_2MA);
  si5351.output_enable(CLK_VFO_TX, 1);
  
  if (si5351_bfo.init(SI5351_CRYSTAL_LOAD_8PF, SI5351_XTAL_FREQ1, 0) == false)
  {  Serial.println ( "SI5351 bfo not found" ); }

  si5351_bfo.set_correction(correction1, SI5351_PLL_INPUT_XO);
  si5351_bfo.drive_strength(CLK_BFO_RX, SI5351_DRIVE_2MA);
  si5351_bfo.output_enable(CLK_BFO_RX, 1);
  
  si5351_bfo.drive_strength(CLK_BFO_TX, SI5351_DRIVE_2MA);
  si5351_bfo.output_enable(CLK_BFO_TX, 1);
    
  shiftOut(BP_80M,LP_80M);
  setbfo();
  setvfo();

  
}


void loop() {

  if(f_dchange==1){
    f_dchange=0;
    UpdateDisplay();
  }

  setvfo();
  setbfo();
  
  display_rx_tx();
  display_smeter();
  display_tmeter();
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
uint8_t   pressed = 1, pressed_old =1;
uint8_t   button_pressed = 0;

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


        pressed = digitalRead(ROTARY_PRESS); 
        if (pressed == 0 && pressed_old != 0)
        {
            int currMillis = millis();
            if (currMillis - lastEncoding1 > 100)
            {
            pressed_old = 0;
            button_pressed++;
            if (button_pressed > 1)
              button_pressed = 0;           
            lastEncoding1 = currMillis;
            }
        }
        else
        {
            if (pressed_old == 0)
            {
              int currMillis = millis();
              if (currMillis - lastEncoding1 > 100)
              {
              pressed_old = 1;
              lastEncoding1 = currMillis;
              } 
            }      
        }
               

         count = 0;
         count = Enc_band.getCount();
         Enc_band.clearCount();
         if (count!=0)
             {
              int currMillis = millis();
              if (currMillis - lastEncoding > 100)
                {     
                    if (button_pressed)
                    {
                        // Switch band
                        band = f_band;
                        if(count > 0) 
                          { if (band == 0) {band = bmax; } else {band--;} next_band(0,band);}
                        else
                          { band++; if (band > bmax) {band = 0; } next_band(1,band);} 
                        
                        frq = current_frq[band];              
                        f_band   = band; // to make it more thread save
                        f_dchange=1;
                        f_fchange=1; // update the si5351 vfo frequency
                    }
                    else
                    {
                        if (f_mode)
                          f_mode = 0;
                        else
                          f_mode = 1;
    
                        f_bchange = 1;
                        f_dchange=1; // send for update display                    
                        }
                lastEncoding = currMillis;
               }
           }
      delay(1);
     }
}
