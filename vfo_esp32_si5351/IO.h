// ArduinoC.h

#ifndef _IO_h
#define _IO_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

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

void _setbfo(uint32_t bfo_frq);
void setvfo(uint32_t frq, uint32_t offset_frq);
void init_io();
void start_cal();
void cal_vfo();
void cal_bfo();
void shiftOut(byte bpf, byte lpf, int rxtx);
void filter_init(void);
void setvfo_rxtx(uint8_t rxtx);
#endif

