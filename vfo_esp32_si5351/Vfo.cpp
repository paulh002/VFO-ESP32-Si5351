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
#include "io.h"
#include "FT891_CAT.h"
#include "si5351.h"

uint8_t			c_rxtx = -1; // detect rx_tx change
uint8_t			f_rxtx = 0;  // initial rx

/*-------------------------------------------------------
   Frequency configuration
--------------------------------------------------------*/
uint8_t f_band[2] = { 1, 1 };
uint8_t f_mode[2] = { 0, 0 };
const unsigned long bandswitch[] = { 160,80,40,20,15,10 };
const  uint8_t bandconf[] = { 0,1,1,1,0,1 };
const unsigned long freqswitch_low[] = { 1800000,3500000,7000000,14000000,21000000,28000000 };
const unsigned long freqswitch_high[] = { 1880000,3800000,7200000,14350000,21450000,29000000 };
const uint8_t mode[] = {0,0,0,1,1,1};  //default modes per band 0 LSB, 1 USB
long  current_frq1[] = { 1800000,3500000,7000000,14000000,21000000,28000000 };
long  current_frq2[] = { 1800000,3500000,7000000,14000000,21000000,28000000 };
const uint8_t	bpf_band[] = { BP_160M ,BP_80M, BP_40M, BP_20M, BP_15M, BP_10M };
const uint8_t	lpf_band[] = { LP_160M ,LP_80M, LP_40M, LP_20M, LP_15M, LP_10M };
const uint8_t freq_step = 10;           // step[Hz]
const uint8_t bmax = 5;

#define USB_FREQUENCY 9001000 //9000500 //8998000 
#define LSB_FREQUENCY 8998000 //8998000 //8995000 


#define BFO_STEP 100  // BFO adjust 10Hz
#define BFO_MIN 8900000 
#define BFO_MAX 9008450 

/*-------------------------------------------------------
   Frequency offsets
--------------------------------------------------------*/

uint32_t   bfo_frq[2] = { LSB_FREQUENCY, LSB_FREQUENCY };     // BFO Frequency[Hz]

void start_vfo()
{
	uint32_t bfo_frq;
	uint32_t frq;
	
	memcpy(current_frq1, R.current_frq1, sizeof(current_frq1));
	memcpy(current_frq2, R.current_frq2, sizeof(current_frq2));
	f_band[0] = R.band[0];
	f_band[1] = R.band[1];
	active_vfo = R.active_vfo;
	R.scale_watt = 50;

	uint8_t band = f_band[active_vfo];
	if (active_vfo == 0)
		frq = current_frq1[band];
	else
		frq = current_frq2[band];
	
	if (mode[band] == 0)
	{
		bfo_frq = LSB_FREQUENCY;
		f_mode[active_vfo] = MODE_LSB;
	}
	else
	{
		bfo_frq = USB_FREQUENCY;
		f_mode[active_vfo] = MODE_USB;
	}
	
	Serial.println("Bfo : " + String(bfo_frq));
	Serial.println("vfo : " + String(frq));
	setvfo_rxtx(f_rxtx);
	setbfo(bfo_frq);
	setvfo(frq, bfo_frq);
	CAT.SetFA(current_frq1[f_band[0]]);
	CAT.SetFB(current_frq1[f_band[1]]);
	CAT.SetMDA(f_mode[0]+1);
	CAT.SetMDB(f_mode[1]+1);
/*
	R.cal_AD[0].db10m = CAL1_NOR_VALUE;
	R.cal_AD[0].Fwd = CALFWD1_DEFAULT;
	R.cal_AD[0].Rev = CALREV1_DEFAULT;
	R.cal_AD[1].db10m = CAL2_NOR_VALUE;
	R.cal_AD[1].Fwd = CALFWD2_DEFAULT;
	R.cal_AD[1].Rev = CALREV2_DEFAULT;
*/
}

void save_vfo()
{
	uint32_t bfo_frq;
	uint32_t frq;

	memcpy(R.current_frq1, current_frq1, sizeof(current_frq1));
	memcpy(R.current_frq2, current_frq2, sizeof(current_frq2));
	R.band[0] = f_band[0] ;
	R.band[1] = f_band[1];
	R.active_vfo = active_vfo;
}

void init_vfo()
{
	long  current_frq1[] = { 1800000,3500000,7000000,14000000,21000000,28000000 };
	long  current_frq2[] = { 1800000,3500000,7000000,14000000,21000000,28000000 };

	// default settings
	memset(&R, 0, sizeof(var_t));	
	memcpy(R.current_frq1, current_frq1, sizeof(current_frq1));
	memcpy(R.current_frq2, current_frq2, sizeof(current_frq2));
	R.band[0] = 1;
	R.band[1] = 1;
	R.active_vfo = 0;
	R.cal_AD[0].db10m = CAL1_NOR_VALUE;
	R.cal_AD[0].Fwd = CALFWD1_DEFAULT;
	R.cal_AD[0].Rev = CALREV1_DEFAULT;
	R.cal_AD[1].db10m = CAL2_NOR_VALUE;
	R.cal_AD[1].Fwd = CALFWD2_DEFAULT;
	R.cal_AD[1].Rev = CALREV2_DEFAULT;
	R.PEP_period = PEP_PERIOD;
	R.AVG_period = 0;
	R.low_power_floor = FLOOR_TEN_mW;
	R.correction_si5351_no1 = 0;
	R.correction_si5351_no2 = 0;
	R.xtal_si5351_no1 = SI5351_XTAL_FREQ;
	R.xtal_si5351_no2 = SI5351_XTAL_FREQ;
	R.wifi_onoff = 1;
	R.scale_watt = CAL_SCALE_WATT;
}

void next_band(uint8_t dir, uint8_t& band, long &frq)
{
	if (dir > 0)
	{
		for (int i = 0; i < bmax; i++)
		{
			if (bandconf[band] == 0) { band++; if (band > bmax) { band = 0; } frq = freqswitch_low[band]; }
			if (bandconf[band] != 0)
				break;
		}
	}
	else
	{
		for (int i = 0; i < bmax; i++)
		{
			if (bandconf[band] == 0) { if (band == 0) { band = bmax; } else { band--; }  frq = freqswitch_high[band]; }
			if (bandconf[band] != 0)
				break;
		}
	}
}

// mode select select the preset bfo mode for the selected band
void mode_select(int new_band, int active_vfo)
{
	if (new_band != f_band[active_vfo])
	{
		// we have to check the bfo setting
		if (mode[new_band])
		{
			// USB
			uint32_t bfo_frq = USB_FREQUENCY;
			f_mode[active_vfo] = MODE_USB;
			BfoLabel(bfo_frq / 1000,0);
			setbfo(bfo_frq);
			CAT.SetMDB(MODE_USB+1);
			CAT.SetMDA(MODE_USB+1);
		}
		else
		{
			// LSB
			uint32_t bfo_frq = LSB_FREQUENCY;
			f_mode[active_vfo] = MODE_LSB;
			BfoLabel(bfo_frq / 1000,0);
			setbfo(bfo_frq);
			CAT.SetMDB(MODE_LSB+1);
			CAT.SetMDA(MODE_LSB+1);
		}
		f_band[active_vfo] = new_band;
		switch_band(active_vfo);
	}
}


//translate gui string to frequency stored in current_freq variable
long band_select(char *buf, int active_vfo)
{
	for (int i = 0; i <= bmax; i++)
	{
		if (bandswitch[i] == atol(buf))
		{
			mode_select(i, active_vfo);
			if (active_vfo == 0)
				return current_frq1[i];
			else
				return current_frq2[i];
		}
	}
	if (active_vfo == 0)
		return current_frq1[f_band[0]];
	else
		return current_frq2[f_band[1]];
}


void band_roller_str(char* str)
{
	char str1[32], * ptr;
	*str = '\0';
	for (int i = 0; i < bmax + 1; i++)
	{
		if (bandconf[i] == 1)
		{
			sprintf(str1, "%ld m\n", bandswitch[i]);
			strcat(str, str1);
		}
	}
	ptr = strrchr(str, '\n');
	if (ptr != NULL)
		*ptr = '\0';
}

uint16_t get_band(void)
{
	uint16_t ii = 0;

	for (int i = 0; i < bmax; i++)
	{
		if (f_band[active_vfo] == i)
			break;

		if (bandconf[i] > 0)
		{
			ii++;
		}
	}
	return (uint16_t)ii;
}

void check_rx_tx()
{
	int16_t x1, y1;
	uint16_t w, h;

	if (digitalRead(TXRX_SWITCH))
	{
		if (CAT.GetTX() == TX_CAT)
		{
			f_rxtx = 1;
		}
		else
		{
			f_rxtx = 0;
			CAT.SetTX((uint8_t)TX_OFF);
		}
	}
	else
	{
		//		CAT.SetTX((uint8_t)TX_MAN);
		f_rxtx = 1;
	}
	if (c_rxtx != f_rxtx)
	{
		setvfo_rxtx(f_rxtx); 
		ToggleTX(f_rxtx);
		c_rxtx = f_rxtx;
		switch_band(active_vfo);
	}
}


long  set_encoder_count_to_vfo_frequency(int count, int active_vfo)
{
	long frq;
	uint8_t band = f_band[active_vfo];

	if (active_vfo == 0)
		frq = current_frq1[band];
	else
		frq = current_frq2[band];

	frq += count * freq_step;
	if (frq > freqswitch_high[band]) { band++; if (band > bmax) { band = 0; } frq = freqswitch_low[band]; next_band(1, band, frq); }
	if (frq < freqswitch_low[band]) { if (band == 0) { band = bmax; } else { band--; }  frq = freqswitch_high[band]; next_band(0, band, frq); }
	if (active_vfo == 0)
	{
		current_frq1[band] = frq;
	}
	else
	{
		current_frq2[band] = frq;
	}
	// mode select will update f_band[active_vfo]
	mode_select(band, active_vfo);
	Togglemode(f_mode[active_vfo], 0); 
	if (active_vfo)
		CAT.SetFB(frq);
	else
		CAT.SetFA(frq);
	setvfo(frq, bfo_frq[active_vfo]);
	return frq;
}


//register and set frequency of vfo
void set_vfo_frequency(long frq,int active_vfo)
{
	if (active_vfo == 0)
		current_frq1[f_band[0]] = frq;	// save current frequency;
	else
		current_frq2[f_band[1]] = frq;	// save current frequency;
	if (active_vfo)
		CAT.SetFB(frq);
	else
		CAT.SetFA(frq);
	setvfo(frq, bfo_frq[active_vfo]);
}

// This function is called from the GUI thread,
// So Semaphores
long switch_vfo(int active_vfo)
{
	long frq;

	if (active_vfo == 0)
		frq = current_frq1[f_band[0]];	// get current frequency vfo 0;
	else
		frq = current_frq2[f_band[1]];	// get current frequency vfo 1;

	if (f_band[0] != f_band[1])
	{
		//switch mode
		uint8_t band = f_band[active_vfo];
		f_band[active_vfo] = -1; //make sure mode_select will change mode
		mode_select(band, active_vfo);
		switch_band(active_vfo);
		Togglemode(f_mode[active_vfo],0);
	}
	setvfo(frq, bfo_frq[active_vfo]);
	return frq;
}

// send commands to lpf and bpf
void switch_band(int active_vfo)
{
	byte bpf, lpf;

	bpf = bpf_band[f_band[active_vfo]];
	lpf = lpf_band[f_band[active_vfo]];
	shiftOut(bpf, lpf, f_rxtx);
}

long get_vfo_frequency(int active_vfo)
{
	if (active_vfo == 0)
		return current_frq1[f_band[0]];	// get current frequency vfo 0;
	else
		return current_frq2[f_band[1]];	// get current frequency vfo 1;
}


uint16_t get_smeter() {
	uint16_t smeterval;

	if (f_rxtx == 0)
	{
		char buf[32];

		smeterval = analogRead(S_METER);
		smeterval = 4096 - smeterval;
		CAT.SetSM(smeterval / 16);
	}
	else
	{
		smeterval = 1024;
		//if (rev_power_db > 0.0)
		//	smeterval = floor(rev_power_db / 6.0);
	}
	return smeterval;
}

/*--------------------------------------------------------------------------
		Check CAT interface
---------------------------------------------------------------------------*/

uint32_t  lastCatRead = 0;

bool CheckCAT()
{
	bool      returnCode = false;  // Assume nothing happened

	/*
	 *  We only look for new messages every CAT_READ_TIME milliseconds:
	 */

	if ((millis() - lastCatRead) < CAT_READ_TIME)
		return returnCode;            // Nothing changed

	lastCatRead = millis();          // Update the last read time

  /*
   *  Ask the CAT module if anything changed. If not, we have nothing to do!
   */

	if (!CAT.CheckCAT())           // If nothing changed
		return returnCode;            // No more to do!

	 // check if mode is changed
	uint8_t mode;
	if (active_vfo)
		mode = CAT.GetMDA() -1;
	else
		mode = CAT.GetMDB() -1;
	if (mode != f_mode[active_vfo])
	{
		Serial.println("Change mode : " + String(mode));
		switch_mode(mode,active_vfo);
		Togglemode(mode);
	}

	//char  str[80];
	//sprintf(str,"freq = %ld cat freq %ld" , frq, CAT.GetFA() );
   // Serial.println(str);
	long frq, frq1;
	if (active_vfo == 0)
		frq = current_frq1[f_band[active_vfo]];
	else
		frq = current_frq2[f_band[active_vfo]];
	if (active_vfo)
		frq1 = CAT.GetFB();
	else
		frq1 = CAT.GetFA();

	if (frq1 != frq)
	{
		if (frq1 > freqswitch_high[f_band[active_vfo]])
			return  returnCode;
		if (frq1 < freqswitch_low[f_band[active_vfo]])
			return  returnCode;
		if (active_vfo == 0)
			current_frq1[f_band[active_vfo]] = frq1;
		else
			current_frq2[f_band[active_vfo]] = frq1;
		setvfo(frq1, bfo_frq[active_vfo]);
		setfrequencylabel(frq1,1);
	}
}

void switch_mode(uint8_t mode, int active_vfo)
{
	if (mode == MODE_LSB)
	{
		// LSB
		uint32_t bfo_frq = LSB_FREQUENCY;
		f_mode[active_vfo] = MODE_LSB;
		BfoLabel(bfo_frq / 1000, 0);
		setbfo(bfo_frq);
		CAT.SetMDB(MODE_LSB + 1);
		CAT.SetMDA(MODE_LSB + 1);
	}
	if (mode == MODE_USB)
	{
		// USB
		uint32_t bfo_frq = USB_FREQUENCY;
		f_mode[active_vfo] = MODE_USB;
		BfoLabel(bfo_frq / 1000, 0);
		setbfo(bfo_frq);
		CAT.SetMDB(MODE_USB + 1);
		CAT.SetMDA(MODE_USB + 1);
	}
}

uint32_t get_bfo(int active_vfo)
{
	return bfo_frq[f_mode[active_vfo]];
}