// ArduinoC.h

#ifndef _GUI_H
#define _GUI_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <lvgl.h>
#include <TFT_eSPI.h>

extern const int screenWidth;
extern const int screenHeight;
extern const int bottomHeight;
extern const int topHeight;

extern TFT_eSPI tft;

void guisetup();
void guiTask(void* arg);
bool my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);
void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p);
void slider_event_cb(lv_obj_t* slider, lv_event_t event);
void printEvent(String Event, lv_event_t event);
void updateBottomStatus(lv_color_t color, String text, uint8_t sem = 1);
void showWifidropdown(void);
void showWifilabel(bool show);
void setfrequencylabel(long freq, long freq2, uint8_t sem = 0);
void ToggleTX(uint8_t show);
void ToggleSetup(bool show, uint8_t sem = 1);
void BfoLabel(uint32_t num, uint8_t sem);
void Togglemode(int mode, uint8_t sem = 1);
void ToggleVfo(int active_vfo, uint8_t sem = 1);

static void mode_button_eh(lv_obj_t* obj, lv_event_t event);
static void popupPW();
static void mbox_event_handler(lv_obj_t* obj, lv_event_t event);
static void lv_ex_btnmatrix_1(void);
static void mode_button_vfo(lv_obj_t* obj, lv_event_t event);
static void band_event_handler(lv_obj_t* obj, lv_event_t event);
static void lv_spinbox_decrement_event_cb(lv_obj_t* btn, lv_event_t e);
static void lv_spinbox_increment_event_cb(lv_obj_t* btn, lv_event_t e);
static void gui_si5351(lv_obj_t* scr);
static void gui_setup(lv_obj_t* scr);
static void rotary_button_eventhandler(AceButton*, uint8_t eventType, uint8_t buttonState);
static void event_button_save(lv_obj_t* btn, lv_event_t e);
static void start_ad8307_cal();
static void init_cal_ad8307_gui(lv_obj_t* scr);
static void gui_setup_event_handler(lv_obj_t* obj, lv_event_t event);
static void lv_bfo_spinbox_event_cb(lv_obj_t* btn, lv_event_t e);
static void event_band_button(lv_obj_t* obj, lv_event_t event);
static void hide_buttons(bool show);

extern lv_obj_t* ddlist;
extern lv_obj_t* bg_top;
extern lv_obj_t* bg_middle;
extern lv_obj_t* bg_bottom;
extern lv_obj_t* kb;
extern lv_obj_t* mbox_connect;
extern lv_obj_t* gauge;
extern lv_obj_t* label_status;
extern lv_obj_t* s_meter;
extern lv_obj_t* usb_button;
extern lv_obj_t* lsb_button;
extern lv_obj_t* si_spinbox;
extern lv_obj_t* si_spinbox2;
extern lv_indev_t* encoder_indev_t;
extern lv_group_t* vfo_group;

extern int active_vfo;
#endif

