// VFO.h

#ifndef _Vfo_h_
#define _Vfo_h_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#define MODE_LSB 0
#define MODE_USB 1

extern uint8_t			f_rxtx;  // initial rx

void next_band(uint8_t dir, uint8_t& band);
void check_rx_tx();
void band_roller_str(char* str);
long set_encoder_count_to_vfo_frequency(int count, int active_vfo);
long band_select(char* buf, int active_vfo);
uint16_t  get_band();
void set_vfo_frequency(long frq, int active_vfo);
void init_vfo();
void start_vfo();
void save_vfo();
long switch_vfo(int active_vfo);
void switch_band(int active_vfo);
long get_vfo_frequency(int active_vfo);
uint32_t get_bfo(int active_vfo);
uint16_t get_smeter();
void switch_mode(uint8_t mode, int active_vfo);
bool CheckCAT();
#endif

