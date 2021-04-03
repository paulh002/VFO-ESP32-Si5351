// 
// 
// 
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESP32Encoder.h>
#include <AceButton.h>
using namespace ace_button;

#include "setup.h"
#include "gui.h"
#include "network.h"
#include "vfo.h"
#include "IO.h"
#include "driver/pcnt.h"
#include "i2c.h"
#include "si5351.h"


/*-------------------------------------------------------
   Si5351
--------------------------------------------------------*/

#define I2C_SDA2  16
#define I2C_SCL2  17
#define SI5351_XTAL_FREQ1 SI5351_XTAL_FREQ //25 mhz
//#define SI5351_XTAL_FREQ1 32000000

#define CLK_BFO_RX           SI5351_CLK2
#define CLK_VFO_RX           SI5351_CLK2
#define CLK_BFO_TX           SI5351_CLK0
#define CLK_VFO_TX           SI5351_CLK0
#define CLK_NA               SI5351_CLK1

Si5351 si5351;
Si5351 si5351_bfo(SI5351_BUS_BASE_ADDR, I2C_SDA2, I2C_SCL2);

/*-------------------------------------------------------
   74HC595 Connection
--------------------------------------------------------*/

#define SER   13  // (74HC595 GPIO pin 13)
#define SRCLK 12  // (74HC595 GPIO pin 12)
#define RCLK  25  // (74HC595 GPIO pin 25)

void init_io()
{
	if (si5351.init(SI5351_CRYSTAL_LOAD_8PF, SI5351_XTAL_FREQ1, 0) == false)
		Serial.println("SI5351 not found");
	else
		Serial.println("SI5351 found");

	Serial.println("SI5351 correcttion: " + String(R.correction_si5351_no1));
	si5351.set_correction(R.correction_si5351_no1, SI5351_PLL_INPUT_XO);
	si5351.drive_strength(CLK_VFO_RX, SI5351_DRIVE_2MA);
	si5351.drive_strength(CLK_VFO_TX, SI5351_DRIVE_2MA);

	if (si5351_bfo.init(SI5351_CRYSTAL_LOAD_8PF, SI5351_XTAL_FREQ1, 0) == false)
		Serial.println("SI5351 bfo not found");
	else
		Serial.println("SI5351 bfo found");

	Serial.println("SI5351 bfo correcttion: " + String(R.correction_si5351_no2));
	si5351_bfo.set_correction(R.correction_si5351_no2, SI5351_PLL_INPUT_XO);
	si5351_bfo.drive_strength(CLK_BFO_RX, SI5351_DRIVE_2MA);
	si5351_bfo.drive_strength(CLK_BFO_TX, SI5351_DRIVE_2MA);

	si5351.output_enable(CLK_VFO_RX, 1);
	si5351.output_enable(CLK_VFO_TX, 0);
	si5351_bfo.output_enable(CLK_BFO_RX, 1);
	si5351_bfo.output_enable(CLK_BFO_TX, 0);

	filter_init();
	shiftOut(BP_80M, LP_80M,0);
}

void setbfo(uint32_t bfo_frq)
{
	uint64_t freq = (uint64_t)bfo_frq * SI5351_FREQ_MULT;
	si5351_bfo.set_freq(freq, CLK_BFO_RX);
	si5351_bfo.set_freq(freq, CLK_BFO_TX);
}

void setvfo(uint32_t frq, uint32_t offset_frq)
{
	uint64_t freq = (uint64_t)(frq + offset_frq) * SI5351_FREQ_MULT;
	si5351.set_freq(freq, CLK_VFO_RX);
	si5351.set_freq(freq, CLK_VFO_TX);
}

void setvfo_rxtx(uint8_t rxtx)
{
	if (rxtx)
	{
		si5351_bfo.output_enable(CLK_BFO_RX, 0);
		si5351_bfo.output_enable(CLK_BFO_TX, 1);
		si5351.output_enable(CLK_VFO_RX, 0);
		si5351.output_enable(CLK_VFO_TX, 1);
		Serial.println("TX output enable");
	}else
	{
		si5351_bfo.output_enable(CLK_BFO_RX, 1);
		si5351_bfo.output_enable(CLK_BFO_TX, 0);
		si5351.output_enable(CLK_VFO_RX, 1);
		si5351.output_enable(CLK_VFO_TX, 0);
		Serial.println("RX output enable");
	}

}


int latchPin = RCLK;
int clockPin = SRCLK;
int dataPin = SER;

void filter_init(void) {
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
}

void shiftOut(byte bpf, byte lpf, int rxtx) {

	byte myDataOut;
	int i = 0, ii = 0;
	int pinState;

	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);

	digitalWrite(dataPin, 0);
	digitalWrite(clockPin, 0);
	digitalWrite(latchPin, 0);


	for (ii = 0; ii < 3; ii++)
	{
		switch (ii)
		{
		case 0:
			myDataOut = lpf;
			if (rxtx)
				myDataOut |= LP_TX;
			break;

		case 1:
			if (rxtx)
				myDataOut = SW_TX;
			else
				myDataOut = SW_RX;
			break;

		case 2:
			myDataOut = bpf;
			break;
		}
		for (i = 7; i >= 0; i--) {
			digitalWrite(clockPin, 0);
			if (myDataOut & (1 << i)) {
				pinState = 1;
			}
			else {
				pinState = 0;
			}
			digitalWrite(dataPin, pinState);
			digitalWrite(clockPin, 1);
			digitalWrite(dataPin, 0);
			delay(1);
		}
	}
	digitalWrite(clockPin, 0);
	digitalWrite(latchPin, 1);
}

#define CAL_FREQ 10000000L

void start_cal()
{
	long frq = CAL_FREQ;

	updateBottomStatus(LV_COLOR_RED, "Calibration frequency 10 Mhz",0);

	lv_spinbox_set_value(si_spinbox,  R.correction_si5351_no1);
	lv_spinbox_set_value(si_spinbox2, R.correction_si5351_no2);

	setbfo(frq);
	setvfo(frq,0);
}

void cal_vfo()
{
	long frq = CAL_FREQ;

	si5351.set_correction(R.correction_si5351_no1, SI5351_PLL_INPUT_XO);
	si5351.set_freq(frq * SI5351_FREQ_MULT, CLK_VFO_RX);
}

void cal_bfo()
{
	long frq = CAL_FREQ;

	si5351_bfo.set_correction(R.correction_si5351_no1, SI5351_PLL_INPUT_XO);
	si5351_bfo.set_freq(frq * SI5351_FREQ_MULT, CLK_VFO_RX);
}