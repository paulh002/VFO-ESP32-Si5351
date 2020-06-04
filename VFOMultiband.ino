/*------Hard ware Configuration ---------------------
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

#define   OLDPCB
#undef    NEWPCB

#include "driver/pcnt.h"
#include "si5351.h"

#define Xw 320
#define Yw 240

#define Nx Xw
#define Ny Yw

/*-------------------------------------------------------
   S-Meter Connection and TXRX switch
--------------------------------------------------------*/

#define RIT_SWITCH  32
#define TXRX_SWITCH 33
#define S_METER     36 
#define T_METER     39

#define SMETER_X     40
#define SMETER_Y     200
#define TMETER_X     40
#define TMETER_Y     165

int smeterval=0;
int tmeterval=0;
int smeterval_old=0;
int tmeterval_old=0;

#define SMETER_TEXT_COLOR2 tft.color565(255,0,0)
#define SMETER_TEXT_COLOR1 tft.color565(0,255,255)
#define TMETER_TEXT_COLOR1 tft.color565(250,250,0)
#define TMETER_TEXT_COLOR2 tft.color565(250,0,0)


/*-------------------------------------------------------
   74HC595 Connection
--------------------------------------------------------*/

#define SER   13  // (pin 14)
#define SRCLK 12  // (pin 11)
#define RCLK 25  // (pin 12)

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
#ifdef NEWPCB
#define ROTARY_A  27      //  pin
#define ROTARY_B  26      //  pin
#define ROTARY_PRESS  14  //  pin
#endif

#ifdef OLDPCB
#define ROTARY_A  27      //  pin
#define ROTARY_B  26      //  pin
#define ROTARY_PRESS  14  //  pin
#endif



volatile int rotState = 0;
volatile int rotAval = 1;
volatile int rotBval = 1;

/*-------------------------------------------------------
   Optical Rotary encoder settings (used for frequency)
--------------------------------------------------------*/
#define PULSE_INPUT_PIN 34  // Rotaty Encoder A
#define PULSE_CTRL_PIN  35  // Rotaty Encoder B

/*-------------------------------------------------------
   Frequency settings
--------------------------------------------------------*/

const unsigned long bandswitch[] = {160,80,40,20,15,10};
const unsigned long freqswitch_low[] = {1800000,3500000,7000000,14000000,21000000,28000000};
const unsigned long freqswitch_high[] = {1880000,3800000,7200000,14350000,21450000,29000000};
#define bmax 5

uint8_t f_band = 1;  // initital band
uint8_t c_band = 0;  // detect band change
uint8_t c_rxtx = 1;  // detect rx_tx change
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

#define USB_FREQUENCY 8998000 
#define LSB_FREQUENCY 8995000 
//#define USB_FREQUENCY_TX 899430 
//#define LSB_FREQUENCY_TX 899730 
#define USB_FREQUENCY_TX 8998000 // 899430 
#define LSB_FREQUENCY_TX 8995000 //899730 
#define F_MAX_MODE  1
#define BFO_STEP 10  // BFO adjust 10Hz
#define BFO_MIN 8900000 
#define BFO_MAX 9008450 
#define BFO_COLOR ILI9341_MAGENTA
#define BFO_Y 60
#define BFO_X 5

/*-------------------------------------------------------
   Bandswitching and RX/TX switching
--------------------------------------------------------*/

#define SW_RX 0x00
#define SW_TX 0x03
#define BP_TX 0x24

// Band pass filter
#define BP_160M 0x08
#define BP_80M 0x10
#define BP_40M 0x40
#define BP_20M 0x80
#define BP_15M 0x02
#define BP_10M 0x01

// Low pass filter
#define LP_160M 0x08
#define LP_80M 0x10
#define LP_40M 0x40
#define LP_20M 0x80
#define LP_15M 0x02
#define LP_10M 0x01

/*-------------------------------------------------------
   Frequency offsets
--------------------------------------------------------*/

int32_t   offset_frq = LSB_FREQUENCY;  // Offset Frequency[Hz]
int32_t   bfo_frq    = LSB_FREQUENCY;  // BFO Frequency[Hz]

#define freq_step 10           // step[Hz]

/*----------------------------------------------------------------------------------
    Control flags
-----------------------------------------------------------------------------------*/
uint8_t f_fchange = 0;  // if frequency changed, set this flag to 1
uint8_t f_bchange = 0;  // if bfo frequency changed set this flag to 1
uint8_t f_dchange = 0;  // if need to renew display, set this flag to 1

/*-----------------------------------------------------------------------------
 *       Global
-----------------------------------------------------------------------------*/
long    frq;      // Contains vfo frequency
int16_t RE_Count = 0;
uint8_t f_redraw; 
uint8_t c_mhz=0;  // Only draw Mhz once

/*--------------------------------------------------------------------------
        Timer ISR
---------------------------------------------------------------------------*/
hw_timer_t * timer = NULL;
void IRAM_ATTR onTimer(){}

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

  Serial.println("Shift Out"); 


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
        Serial.println("lpf"); 
        myDataOut = lpf;
        break;
      case 1:
        Serial.println("bpf"); 
        if (f_rxtx)
          { // TX
          myDataOut = bpf | BP_TX;
          }
        else
          {
          myDataOut = bpf;   
          }
        break;
      case 2:
        if (f_rxtx)
          {
          Serial.println("TX"); 
          myDataOut = SW_TX;
          }
        else
          {
          Serial.println("RX"); 
          myDataOut = SW_RX;
          }
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
    }
  Serial.println(myDataOut);
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
  
 adcAttachPin(S_METER);
 adcStart(S_METER);
 while ( adcBusy(S_METER));
 adcEnd(S_METER);
 smeterval=analogRead(S_METER);
// Serial.print(" S METER: ");
// Serial.println(tmeterval);
 smeterval=(10 * smeterval)/4096;
 if (smeterval_old == smeterval)
  return;
 smeterval_old = smeterval;
 if (smeterval>10){smeterval=10;}
  tft.setFont(&FreeMonoBold24pt7b);
  tft.getTextBounds("----------", (int16_t) SMETER_X, (int16_t) SMETER_Y, &x1, &y1, &w, &h);
  tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,ILI9341_BLACK);  
  tft.setCursor(SMETER_X,SMETER_Y);
  for(int i=1;i<=smeterval;i++){
    if (i<=6){
      tft.setTextColor(SMETER_TEXT_COLOR1);
      tft.print("-");
    }
    else{
      tft.setTextColor(SMETER_TEXT_COLOR2);
      tft.print("-");
    }
  }
}

void display_tmeter(){
  int16_t x1, y1; 
  uint16_t w, h;
  
 adcAttachPin(T_METER);
 adcStart(S_METER);
 while ( adcBusy(S_METER));
 adcEnd(S_METER);
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
    f_bchange=0;
    set_bfo_freq((unsigned long)bfo_frq);                         
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

void display_rx_tx()
{
  int16_t x1, y1; 
  uint16_t w, h;
  
  if (c_rxtx != f_rxtx)
  {
    c_rxtx = f_rxtx;
    
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextColor(BAND_TEXT_COLOR);
    tft.setCursor(rx_x,rx_y);
    tft.getTextBounds("RX", (int16_t)rx_x,(int16_t)rx_y, &x1, &y1, &w, &h); // use enough space to draw background
    tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
    if (c_rxtx == 0)
      {
        tft.print("RX");
        if (f_mode == 0)
          bfo_frq = LSB_FREQUENCY;
        else
          bfo_frq = USB_FREQUENCY; 
      }
    else
      {
        tft.print("TX");
        if (f_mode == 0)
          bfo_frq = LSB_FREQUENCY_TX;
        else
          bfo_frq = USB_FREQUENCY_TX;    
        }    
    f_bchange = 1; // bfo has changed
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
    switch (f_band)
    {
    case 0:
      f_mode = 0;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,band_y);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h); // use enough space to draw background
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print("160 M");
      shiftOut(BP_160M,LP_160M);
      break;
  
    case 1:
      f_mode = 0;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,82);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 80 M");
      shiftOut(BP_80M,LP_80M);
      break;
  
    case 2:
      f_mode = 0;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,band_y);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 40 M");
      shiftOut(BP_40M,LP_40M);
      break;
  
  case 3:
      f_mode = 1;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,band_y);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 20 M");
      shiftOut(BP_20M,LP_20M);
      break;
      
  case 4:
      f_mode = 1;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,band_y);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 15 M");
      shiftOut(BP_15M,LP_15M);
      break;

  case 5:
      f_mode = 1;
      tft.setFont(&FreeSansBold12pt7b);
      tft.setTextColor(BAND_TEXT_COLOR);
      tft.setCursor(band_x,band_y);
      tft.getTextBounds("160 M", (int16_t)band_x,(int16_t)band_y, &x1, &y1, &w, &h);
      tft.fillRoundRect(x1-2,y1-2,w+4,h+4,5,BAND_BACKGROUND_COLOR);  
      tft.print(" 10 M");
      shiftOut(BP_10M,LP_10M);
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
      if (c_rxtx == 0)
          bfo_frq = LSB_FREQUENCY;
      else
          bfo_frq = LSB_FREQUENCY_TX;  
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
      if (c_rxtx == 0)
          bfo_frq = USB_FREQUENCY;
      else
          bfo_frq = USB_FREQUENCY_TX;  
      f_bchange = 1;
      break;

    case 2:
      break;
  
    case 3:
      break;
    }
  }
}


void UpdateRot()
{
  char str[64];
  uint16_t i;
 
  i = digitalRead(ROTARY_PRESS);
  if (digitalRead(ROTARY_PRESS) == LOW)
  {
    f_mode++;
    if (f_mode > F_MAX_MODE) f_mode = 0;
    f_dchange=1;
    f_bchange=1;
  }
  else
  {
// Switch band
  if(rotAval) 
    { if (f_band == 0) {f_band = bmax; } else {f_band--;}  frq= freqswitch_low[f_band];}
  else
    { f_band++; if (f_band > bmax) {f_band = 0; } frq= freqswitch_low[f_band];} 
  f_dchange=1;
  }
}


volatile int lastEncoding3 = 0;
void IRAM_ATTR ISRXChange()
{
  int currMillis = millis();
  if (currMillis - lastEncoding3 > 50)
    {
      // 0 = TX
      if (digitalRead(TXRX_SWITCH))
        {
        f_rxtx = 0;
        f_dchange=1;
        c_band = -1;
        }
      else 
      {
        f_rxtx = 1;
        f_dchange=1;
        c_band = -1;
      }
    lastEncoding3 = currMillis;
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
 //------------------------------------- update vfo frequency 
 if(f_fchange==1){
    f_fchange=0;
    // Output Lo freq
    set_vfo_freq( (unsigned long)(frq + offset_frq) );                
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
 tft.print("DDS-VFO Ver1.0 PA0PHH");  
 }


void setup() {
  bool i2c_found;
  uint64_t reg_a;
  uint64_t reg_b;
  uint64_t reg_c;
  
  Serial.begin(115200);
  Serial.println("VFO PA0PHH !"); 
  frq = freqswitch_low[f_band];
  
  display_init();
  setup_display();
  filter_init();
  xTaskCreatePinnedToCore(task0, "Task0", 4096, NULL, 1, NULL, 0);
        
  //--------- Set up Interrupt Timer -------------------------------
  timer = timerBegin(0, 80, true); //use Timer0, div80 for 1us clock
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10000, true); // T=10000us
  timerAlarmEnable(timer); // Start Timer
    
  //--- Counter setup for Rotary Encoder ---------------------
  pcnt_config_t pcnt_config_A;// structure for A   
  pcnt_config_t pcnt_config_B;// structure for B
  //
  pcnt_config_A.pulse_gpio_num = PULSE_INPUT_PIN;
  pcnt_config_A.ctrl_gpio_num = PULSE_CTRL_PIN;
  pcnt_config_A.lctrl_mode = PCNT_MODE_REVERSE;
  pcnt_config_A.hctrl_mode = PCNT_MODE_KEEP;
  pcnt_config_A.channel = PCNT_CHANNEL_0;
  pcnt_config_A.unit = PCNT_UNIT_0;
  pcnt_config_A.pos_mode = PCNT_COUNT_INC;
  pcnt_config_A.neg_mode = PCNT_COUNT_DEC;
  pcnt_config_A.counter_h_lim = 10000;
  pcnt_config_A.counter_l_lim = -10000;
//
  pcnt_config_B.pulse_gpio_num = PULSE_CTRL_PIN;
  pcnt_config_B.ctrl_gpio_num = PULSE_INPUT_PIN;
  pcnt_config_B.lctrl_mode = PCNT_MODE_KEEP;
  pcnt_config_B.hctrl_mode = PCNT_MODE_REVERSE;
  pcnt_config_B.channel = PCNT_CHANNEL_1;
  pcnt_config_B.unit = PCNT_UNIT_0;
  pcnt_config_B.pos_mode = PCNT_COUNT_INC;
  pcnt_config_B.neg_mode = PCNT_COUNT_DEC;
  pcnt_config_B.counter_h_lim = 10000;
  pcnt_config_B.counter_l_lim = -10000;
//
  pcnt_unit_config(&pcnt_config_A);//Initialize A
  pcnt_unit_config(&pcnt_config_B);//Initialize B
  pcnt_counter_pause(PCNT_UNIT_0);
  pcnt_counter_clear(PCNT_UNIT_0);
  pcnt_counter_resume(PCNT_UNIT_0); //Start


//--- Counter setup for 2nd Rotary Encoder ---------------------
  pcnt_config_t pcnt_config_C;// structure for A   
  pcnt_config_t pcnt_config_D;// structure for B
  //
  pcnt_config_C.pulse_gpio_num = ROTARY_A;
  pcnt_config_C.ctrl_gpio_num = ROTARY_B;
  pcnt_config_C.lctrl_mode = PCNT_MODE_REVERSE;
  pcnt_config_C.hctrl_mode = PCNT_MODE_KEEP;
  pcnt_config_C.channel = PCNT_CHANNEL_0;
  pcnt_config_C.unit = PCNT_UNIT_1;
  pcnt_config_C.pos_mode = PCNT_COUNT_INC;
  pcnt_config_C.neg_mode = PCNT_COUNT_DEC;
  pcnt_config_C.counter_h_lim = 10000;
  pcnt_config_C.counter_l_lim = -10000;
//
  pcnt_config_D.pulse_gpio_num = ROTARY_B;
  pcnt_config_D.ctrl_gpio_num = ROTARY_A;
  pcnt_config_D.lctrl_mode = PCNT_MODE_KEEP;
  pcnt_config_D.hctrl_mode = PCNT_MODE_REVERSE;
  pcnt_config_D.channel = PCNT_CHANNEL_1;
  pcnt_config_D.unit = PCNT_UNIT_1;
  pcnt_config_D.pos_mode = PCNT_COUNT_INC;
  pcnt_config_D.neg_mode = PCNT_COUNT_DEC;
  pcnt_config_D.counter_h_lim = 10000;
  pcnt_config_D.counter_l_lim = -10000;
//
  pcnt_unit_config(&pcnt_config_C);//Initialize A
  pcnt_unit_config(&pcnt_config_D);//Initialize B
  pcnt_counter_pause(PCNT_UNIT_1);
  pcnt_counter_clear(PCNT_UNIT_1);
  pcnt_counter_resume(PCNT_UNIT_1); //Start


  // set tx rx and rit switch ports
  pinMode(TXRX_SWITCH, INPUT);
  pinMode(RIT_SWITCH, INPUT);
  analogReadResolution(12);
 
    
  // Set pushbutton rotary encoder
  pinMode(ROTARY_PRESS, INPUT);
  digitalWrite(ROTARY_PRESS, HIGH);
  
  // Set up interrupt pins for rxtx
  attachInterrupt(digitalPinToInterrupt(TXRX_SWITCH), ISRXChange, CHANGE);
  
  si5351_init();
   
  set_vfo_freq((unsigned long) (frq + offset_frq) );
  set_bfo_freq((unsigned long) bfo_frq);
  shiftOut(BP_80M,LP_80M);
}


void loop() {
  // put your main code here, to run repeatedly:
 
  if(f_dchange==1){
    f_dchange=0;
    UpdateDisplay();
  }
  display_smeter();
  display_tmeter();
}


/*-----------------------------------------------------------------------------------------------
        Alternative Loop (core0)
------------------------------------------------------------------------------------------------*/

void task0(void* arg)
{
    while (1)
     {    
         pcnt_get_counter_value(PCNT_UNIT_0, &RE_Count); // Poll the counter
         int count=RE_Count;
         pcnt_counter_clear(PCNT_UNIT_0);
         
         if (count!=0)
         {                           
            f_dchange=1; // send for update display
            f_fchange=1; // update the si5351 vfo frequency
            frq+= count * freq_step;
            if(frq>freqswitch_high[f_band]) { f_band++; if (f_band > bmax) {f_band = 0; } frq= freqswitch_low[f_band];} 
            if(frq<freqswitch_low[f_band]) { if (f_band == 0) {f_band = bmax; } else {f_band--;}  frq= freqswitch_high[f_band];}
         }

         pcnt_get_counter_value(PCNT_UNIT_1, &RE_Count); // Poll the counter of 2nd Rotary
         count=RE_Count;
         pcnt_counter_clear(PCNT_UNIT_1);
         if (count!=0)
         {
          if (count > 0)
            rotAval =1;
          else
            rotAval = 0;
          UpdateRot(); 
          delay(50);                          
          pcnt_counter_clear(PCNT_UNIT_1);
         }
      delay(1);
     }
}
