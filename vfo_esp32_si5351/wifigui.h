// wifigui.h

#ifndef _wifigui_h
#define _wifigui_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
void show_wifi_gui(int show);
void init_wifi_gui(lv_obj_t* scr);
static void Save_wifi(char* buf);
static void makePW();
static void keyboard_event_cb(lv_obj_t* kb, lv_event_t event);
static void makeKeyboard();
void show_ota_gui(int show, uint8_t sem = 1);
void set_ota_label(String str, uint8_t sem = 1);
void set_ota_label1(String str, uint8_t sem = 1);
#endif

