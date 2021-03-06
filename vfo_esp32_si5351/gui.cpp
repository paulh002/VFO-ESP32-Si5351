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
#include "wifigui.h"
#include "io.h"
#include "smeter.h"

#define LVGL_TICK_PERIOD 60


LV_FONT_DECLARE(FreeSansOblique42);
LV_FONT_DECLARE(FreeSansOblique32);

/*-------------------------------------------------------
   Optical Rotary encoder settings (used for frequency)
--------------------------------------------------------*/
#define PULSE_INPUT_PIN 34  // Rotaty Encoder A
#define PULSE_CTRL_PIN  35  // Rotaty Encoder B
#define ROTARY_A      14      
#define ROTARY_B      19
#define ROTARY_PRESS  26

ESP32Encoder    Enc_vfo;
AceButton		rotary_button(ROTARY_PRESS);

/*-------------------------------------------------------
   Rotary hardware connection (used for bandswitching, and mode switching)
--------------------------------------------------------*/
static	ESP32Encoder	GuiEncoder;
volatile int			enc_button_state = LV_INDEV_STATE_REL;
void					rotary_button_eventhandler(AceButton*, uint8_t, uint8_t);

int active_vfo = 0;  // holds which vfo is active selected using radio buttons

//Ticker tick; /* timer for interrupt handler */
TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

lv_obj_t* slider_label;
const int screenWidth = 320;
const int screenHeight = 240;
const int bottomHeight = 40;
const int topHeight = 25;
const int nobuttons = 6;
const int bottombutton_width = (screenWidth / nobuttons) - 2;
const int bottombutton_width1 = (screenWidth / nobuttons);

lv_obj_t* ddlist;
lv_obj_t* bg_top;
lv_obj_t* bg_middle;
lv_obj_t* mbox_connect;
lv_obj_t* gauge;
lv_obj_t* label_status;
lv_obj_t* s_meter;
lv_obj_t* usb_button;
lv_obj_t* lsb_button;
lv_obj_t* vfo_frequency;
lv_obj_t* vfo2_frequency;
lv_obj_t* label_wifi;
lv_obj_t* vfo_unit;
lv_obj_t* vfo2_unit;
lv_obj_t* vfo1_button;
lv_obj_t* vfo2_button;
lv_obj_t* band_button;
lv_obj_t* save_button;
lv_obj_t* Band_btn_label;
lv_obj_t* Tx_led1;
lv_obj_t* bfo_label;
lv_obj_t* s_canvas;
lv_obj_t* bg_middle2;
lv_obj_t* bg_middle3;
lv_obj_t* Swr_gauge;
lv_obj_t* pwr_gauge;
lv_obj_t* swr_vlabel;
lv_obj_t* pwr_vlabel;
lv_obj_t* bg_calgui;
lv_obj_t* ad_spinbox1;
lv_obj_t* ad_spinbox2;
lv_obj_t* ad_spinbox3;
lv_obj_t* ad_spinbox4;
lv_obj_t* fwd_label1;
lv_obj_t* bg_top_cal;
lv_obj_t* btn_fwd;
lv_obj_t* btn_rev;
lv_obj_t* label_cal;
lv_obj_t* watt_cal;
lv_obj_t* btn_matrix;
lv_obj_t* bfo_button;

lv_obj_t* setting_button;
lv_obj_t* wifi1_button;
lv_obj_t* si5351_button;
lv_obj_t* ad8307_button;
lv_obj_t* reset_button;

lv_obj_t* si_vfo_button;
lv_obj_t* si_bfo_button;
lv_obj_t* si_save_button;
lv_obj_t* si_frequency;
lv_obj_t* si_unit;
lv_obj_t* si_label1;
lv_obj_t* si_cancel_button;

lv_group_t* vfo_group;
lv_group_t* si_5351_group;
lv_group_t* setup_group;
lv_group_t* cal_group;
lv_indev_t* encoder_indev_t;

CSmeter* smeter;

static lv_color_t* cbuf;
static lv_style_t cal_led_style;
static lv_style_t style_btn;

// VFO States
// The VFO can be in different states:
// f_rxtx determines if the VFO is in receive or transmit mode
// f_cal_ad8307 determines if the VFO is calibrating the ad8307 for this the system need to measure the input of the ADC to a fixed dbm value
// active_vfo determines if VFO 0 or VFO 1 is active

static bool f_cal_ad8307 = false;
static bool f_cal_si5351 = false;
static bool f_cal_si5351_vfo = true;
static bool show_setting = false;
static bool f_bfo_adjust = false;

#if USE_LV_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char* file, uint32_t line, const char* dsc)
{

	Serial.printf("%s@%d->%s\r\n", file, line, dsc);
	delay(100);
}
#endif

static void rotary_button_eventhandler(AceButton*, uint8_t eventType, uint8_t buttonState)
{
	switch (eventType) {
	case AceButton::kEventLongPressed:
		ToggleSetup(true);
		//enc_button_state = LV_INDEV_STATE_REL;
		Serial.println("Long press");
		break;
	case AceButton::kEventPressed:
		enc_button_state = LV_INDEV_STATE_PR;
		Serial.println("Pressed");
		break;

	case AceButton::kEventReleased:
		enc_button_state = LV_INDEV_STATE_REL;
		Serial.println("Released");
		break;
	}
}

bool read_encoder(lv_indev_drv_t* indev, lv_indev_data_t* data)
{
	data->enc_diff = GuiEncoder.getCount();
	GuiEncoder.clearCount();
	data->state = enc_button_state;	
	if (data->enc_diff > 0)
		data->enc_diff = 1;
	if (data->enc_diff < 0)
		data->enc_diff = -1;
	return false;
}

static void lv_gauge_format_swr(lv_obj_t* gauge, char* buf, int bufsize, int32_t value)
{
	// 1:1
	snprintf(buf, bufsize, "%d.%d", (value / 10) % 10, (value) % 10);
}

static void lv_gauge_format_pwr(lv_obj_t* gauge, char* buf, int bufsize, int32_t value)
{
	snprintf(buf, bufsize, "%d.%d", value / 10, (value) % 10);
}

void guisetup() {
	xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	pinMode(ROTARY_PRESS, INPUT);
	rotary_button.setEventHandler(rotary_button_eventhandler);
	//rotary_button.getButtonConfig()->setFeature(ButtonConfig::kFeatureLongPress);
	//rotary_button.getButtonConfig()->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
	ESP32Encoder::useInternalWeakPullResistors = NONE;
	GuiEncoder.attachHalfQuad(ROTARY_B, ROTARY_A, 1024);
	Enc_vfo.attachHalfQuad(PULSE_INPUT_PIN, PULSE_CTRL_PIN);

	xTaskCreate(guiTask,
		"gui",
		4096 * 2,
		NULL,
		2,
		NULL);
}

void guiTask(void* arg) {

	
	lv_init();

#if USE_LV_LOG != 0
	lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

	tft.begin(); /* TFT init */
	tft.setRotation(1);

	uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
	tft.setTouch(calData);

	lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

	/*Initialize the display*/
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res = screenWidth;
	disp_drv.ver_res = screenHeight;
	disp_drv.flush_cb = my_disp_flush;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);

	/*Initialize the input device driver*/
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);             /*Descriptor of a input device driver*/
	indev_drv.type = LV_INDEV_TYPE_POINTER;    /*Touch pad is a pointer-like device*/
	indev_drv.read_cb = my_touchpad_read;      /*Set your driver function*/
	lv_indev_drv_register(&indev_drv);         /*Finally register the driver*/

	lv_indev_drv_t indev_drv_enc; 
	lv_indev_drv_init(&indev_drv_enc);
	indev_drv_enc.type = LV_INDEV_TYPE_ENCODER;
	indev_drv_enc.read_cb = read_encoder;
	encoder_indev_t = lv_indev_drv_register(&indev_drv_enc);

	//Set the theme..
	lv_theme_t* th = lv_theme_material_init(LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY, LV_THEME_MATERIAL_FLAG_DARK, LV_THEME_DEFAULT_FONT_SMALL, LV_THEME_DEFAULT_FONT_NORMAL, LV_THEME_DEFAULT_FONT_SUBTITLE, LV_THEME_DEFAULT_FONT_TITLE);
	lv_theme_set_act(th);

	lv_obj_t* scr = lv_cont_create(NULL, NULL);
	lv_disp_load_scr(scr);

	bg_top = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_top, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_top, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_top, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_NAVY);
	lv_obj_set_size(bg_top, LV_HOR_RES, topHeight);

	label_status = lv_label_create(bg_top, NULL);
	lv_label_set_long_mode(label_status, LV_LABEL_LONG_SROLL_CIRC);
	lv_obj_set_width(label_status, LV_HOR_RES - 20);
	lv_label_set_text(label_status, "");
	lv_obj_align(label_status, NULL, LV_ALIGN_CENTER, 20, 0);

	bg_middle = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_middle, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_middle, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_middle, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(bg_middle, 0, topHeight);
	lv_obj_set_size(bg_middle, LV_HOR_RES, screenHeight - topHeight);

	smeter = new CSmeter(bg_middle, LV_ALIGN_CENTER, 255, 50, 0, 30);

	// Create groups for encoder support for the different screens
	vfo_group = lv_group_create();
	lv_indev_set_group(encoder_indev_t, vfo_group);
	
	lv_style_init(&style_btn);

	lv_style_set_radius(&style_btn, LV_STATE_DEFAULT, 10);
	lv_style_set_bg_color(&style_btn, LV_STATE_DEFAULT, lv_color_make(0x60, 0x60, 0x60));
	lv_style_set_bg_grad_color(&style_btn, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_dir(&style_btn, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_btn, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_btn, LV_STATE_DEFAULT, lv_color_make(0x9b, 0x36, 0x36) ); // lv_color_make(0x2e, 0x44, 0xb2)
	lv_style_set_border_width(&style_btn, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_btn, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_btn, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_outline_opa(&style_btn, LV_STATE_DEFAULT, 255);
	
	lv_style_set_border_width(&style_btn, LV_STATE_FOCUSED, 2);
	lv_style_set_outline_color(&style_btn, LV_STATE_FOCUSED, LV_COLOR_RED);
	//lv_style_set_border_color(&style_btn, LV_STATE_FOCUSED, LV_COLOR_WHITE);
	//lv_style_set_outline_color(&style_btn, LV_STATE_FOCUSED, LV_COLOR_WHITE);
	//lv_style_set_bg_color(&style_btn, LV_STATE_FOCUSED, LV_COLOR_OLIVE);

	//lv_style_set_border_color(&style_btn, LV_STATE_CHECKED, lv_color_make(0x9b, 0x36, 0x36)); // lv_color_make(0x2e, 0x44, 0xb2)
	//lv_style_set_border_color(&style_btn, LV_STATE_CHECKED, LV_COLOR_BLACK); // lv_color_make(0x2e, 0x44, 0xb2)
	//lv_style_set_border_width(&style_btn, LV_STATE_CHECKED, 2);

	/*Set the text style*/
	lv_style_set_text_color(&style_btn, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	lsb_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(lsb_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(lsb_button, mode_button_eh);
	lv_obj_align(lsb_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_btn_set_checkable(lsb_button, true);
	lv_btn_toggle(lsb_button);
	lv_obj_set_size(lsb_button, bottombutton_width, bottomHeight);
	lv_obj_t* label = lv_label_create(lsb_button, NULL);
	lv_label_set_text(label, "Lsb");
	
	usb_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(usb_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(usb_button, mode_button_eh);
	lv_obj_align(usb_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, bottombutton_width1, 0);
	lv_btn_set_checkable(usb_button, true);
	lv_obj_set_size(usb_button, bottombutton_width, bottomHeight);
	label = lv_label_create(usb_button, NULL);
	lv_label_set_text(label, "Usb");
	
	vfo1_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(vfo1_button, LV_BTN_PART_MAIN, &style_btn); 
	lv_obj_set_event_cb(vfo1_button, mode_button_vfo);
	lv_obj_align(vfo1_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 2 * bottombutton_width1, 0);
	lv_btn_set_checkable(vfo1_button, true);
	lv_btn_toggle(vfo1_button);
	lv_obj_set_size(vfo1_button, bottombutton_width, bottomHeight);
	label = lv_label_create(vfo1_button, NULL);
	lv_label_set_text(label, "Vfo 1");
	
	vfo2_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(vfo2_button, LV_BTN_PART_MAIN, &style_btn); 
	lv_obj_set_event_cb(vfo2_button, mode_button_vfo);
	lv_obj_align(vfo2_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 3 * bottombutton_width1, 0);
	lv_btn_set_checkable(vfo2_button, true);
	lv_obj_set_size(vfo2_button, bottombutton_width, bottomHeight);
	label = lv_label_create(vfo2_button, NULL);
	lv_label_set_text(label, "Vfo 2");
	
	band_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(band_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(band_button, event_band_button);
	lv_obj_align(band_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 4 * bottombutton_width1, 0);
	lv_obj_set_size(band_button, bottombutton_width, bottomHeight);
	Band_btn_label = lv_label_create(band_button, NULL);
	lv_label_set_text(Band_btn_label, "80m");

	setting_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(setting_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(setting_button, event_button_setting_cb);
	lv_btn_set_checkable(setting_button, true);
	lv_obj_align(setting_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 5 * bottombutton_width1, 0);
	lv_obj_set_size(setting_button, bottombutton_width, bottomHeight);
	label = lv_label_create(setting_button, NULL);
	lv_label_set_text(label, LV_SYMBOL_SETTINGS);

	wifi1_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(wifi1_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(wifi1_button, event_button_wifi_cb);
	lv_obj_align(wifi1_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, -1*(bottomHeight+2));
	lv_obj_set_size(wifi1_button, bottombutton_width, bottomHeight);
	label = lv_label_create(wifi1_button, NULL);
	lv_label_set_text(label, LV_SYMBOL_WIFI);
	lv_obj_set_hidden(wifi1_button, true);
	
	si5351_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(si5351_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(si5351_button, event_button_si5351_cb);
	lv_obj_align(si5351_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, bottombutton_width1, -1 * (bottomHeight + 2));
	lv_obj_set_size(si5351_button, bottombutton_width, bottomHeight);
	label = lv_label_create(si5351_button, NULL);
	lv_label_set_text(label, "5351");
	lv_obj_set_hidden(si5351_button, true);

	ad8307_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(ad8307_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(ad8307_button, event_button_ad8307_cb);
	lv_obj_align(ad8307_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 2 * bottombutton_width1, -1 * (bottomHeight + 2));
	lv_obj_set_size(ad8307_button, bottombutton_width, bottomHeight);
	label = lv_label_create(ad8307_button, NULL);
	lv_label_set_text(label, "8307");
	lv_obj_set_hidden(ad8307_button, true);
	
	reset_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(reset_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(reset_button, event_button_reset_cb);
	lv_obj_align(reset_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 3 *  bottombutton_width1, -1 * (bottomHeight + 2));
	lv_obj_set_size(reset_button, bottombutton_width, bottomHeight);
	label = lv_label_create(reset_button, NULL);
	lv_label_set_text(label, "Reset");
	lv_obj_set_hidden(reset_button, true);
	
	bfo_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(bfo_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(bfo_button, event_button_bfo_cb);
	lv_btn_set_checkable(bfo_button, true);
	lv_obj_align(bfo_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 4 * bottombutton_width1, -1 * (bottomHeight + 2));
	lv_obj_set_size(bfo_button, bottombutton_width, bottomHeight);
	label = lv_label_create(bfo_button, NULL);
	lv_label_set_text(label, "Bfo");
	lv_obj_set_hidden(bfo_button, true);
	
	save_button = lv_btn_create(bg_middle, NULL);
	lv_obj_add_style(save_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(save_button, event_button_save);
	lv_obj_align(save_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 5 * bottombutton_width1, -1 * (bottomHeight + 2));
	lv_obj_set_size(save_button, bottombutton_width, bottomHeight);
	label = lv_label_create(save_button, NULL);
	lv_label_set_text(label, "Save");
	lv_obj_set_hidden(save_button, true);

	static lv_style_t text_style;
	lv_style_init(&text_style);

	/*Set a background color and a radius*/
	lv_style_set_radius(&text_style, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_opa(&text_style, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&text_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_value_align(&text_style, LV_STATE_DEFAULT, LV_ALIGN_CENTER);
	lv_style_set_text_font(&text_style, LV_STATE_DEFAULT, &FreeSansOblique42);

	vfo_frequency = lv_label_create(bg_middle, NULL);
	lv_label_set_long_mode(vfo_frequency, LV_LABEL_LONG_CROP);
	lv_obj_add_style(vfo_frequency, LV_OBJ_PART_MAIN, &text_style);
	lv_obj_set_width(vfo_frequency, LV_HOR_RES - 20);
	lv_label_set_text(vfo_frequency, "");
	lv_obj_set_height(vfo_frequency, 40);
	lv_obj_align(vfo_frequency, bg_middle, LV_ALIGN_CENTER, -30, -84);
	lv_label_set_align(vfo_frequency, LV_LABEL_ALIGN_CENTER);

	vfo_unit = lv_label_create(bg_middle, NULL);
	lv_obj_add_style(vfo_unit, LV_OBJ_PART_MAIN, &text_style);
	lv_label_set_text(vfo_unit, "Khz");
	lv_obj_set_height(vfo_unit, 40);
	lv_obj_align(vfo_unit, bg_middle, LV_ALIGN_CENTER, 110, -87);
	lv_label_set_align(vfo_unit, LV_LABEL_ALIGN_CENTER);

	static lv_style_t text_style12;
	lv_style_init(&text_style12);

	/*Set a background color and a radius*/
	lv_style_set_radius(&text_style12, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_opa(&text_style12, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&text_style12, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_value_align(&text_style12, LV_STATE_DEFAULT, LV_ALIGN_CENTER);
	lv_style_set_text_font(&text_style12, LV_STATE_DEFAULT, &FreeSansOblique32);
	lv_style_set_text_color(&text_style12, LV_STATE_DEFAULT, LV_COLOR_OLIVE);

	vfo2_frequency = lv_label_create(bg_middle, NULL);
	lv_label_set_long_mode(vfo2_frequency, LV_LABEL_LONG_CROP);
	lv_obj_add_style(vfo2_frequency, LV_OBJ_PART_MAIN, &text_style12);
	lv_obj_set_width(vfo2_frequency, LV_HOR_RES - 20);
	lv_label_set_text(vfo2_frequency, "28.000.00");
	lv_obj_set_height(vfo2_frequency, 40);
	lv_obj_align(vfo2_frequency, bg_middle, LV_ALIGN_CENTER, -35, -44);
	lv_label_set_align(vfo2_frequency, LV_LABEL_ALIGN_CENTER);
	
	vfo2_unit = lv_label_create(bg_middle, NULL);
	lv_obj_add_style(vfo2_unit, LV_OBJ_PART_MAIN, &text_style12);
	lv_label_set_text(vfo2_unit, "Khz");
	lv_obj_set_height(vfo2_unit, 40);
	lv_obj_align(vfo2_unit, bg_middle, LV_ALIGN_CENTER, 120, -49);
	lv_label_set_align(vfo2_unit, LV_LABEL_ALIGN_CENTER);

	bfo_label = lv_label_create(bg_middle, NULL);
	lv_label_set_long_mode(bfo_label, LV_LABEL_LONG_CROP);
	lv_obj_set_width(bfo_label, 120);
	lv_label_set_text(bfo_label, "8998.0");
	lv_obj_align(bfo_label, NULL, LV_ALIGN_IN_LEFT_MID, 10, -30);

	label_wifi = lv_label_create(bg_top, NULL);
	lv_label_set_long_mode(label_wifi, LV_LABEL_LONG_SROLL_CIRC);
	lv_obj_set_width(label_wifi, LV_HOR_RES - 20);
	lv_label_set_text(label_wifi, LV_SYMBOL_WIFI);
	lv_obj_align(label_wifi, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 5);
	lv_obj_set_hidden(label_wifi, true);

	Tx_led1 = lv_led_create(bg_middle, NULL);
	lv_obj_align(Tx_led1, NULL, LV_ALIGN_CENTER, -125, -70);
	lv_led_off(Tx_led1);
	lv_obj_set_size(Tx_led1, 20, 20);
	
	lv_obj_t* Tx_ledlabel = lv_label_create(bg_middle, NULL);
	lv_obj_align(Tx_ledlabel, Tx_led1, LV_ALIGN_OUT_TOP_MID, 7, 0);
	lv_label_set_text(Tx_ledlabel, "TX");

	// Keep de order of the focus correct
	lv_group_add_obj(vfo_group, lsb_button);
	lv_group_add_obj(vfo_group, usb_button);
	lv_group_add_obj(vfo_group, vfo1_button);
	lv_group_add_obj(vfo_group, vfo2_button);
	lv_group_add_obj(vfo_group, band_button);
	lv_group_add_obj(vfo_group, setting_button);

	lv_group_add_obj(vfo_group, wifi1_button);
	lv_group_add_obj(vfo_group, si5351_button);
	lv_group_add_obj(vfo_group, ad8307_button);
	lv_group_add_obj(vfo_group, reset_button);
	lv_group_add_obj(vfo_group, bfo_button);
	lv_group_add_obj(vfo_group, save_button);

	// TX objects shown when VFO is in TX mode
	static lv_color_t needle_colors[1];
	needle_colors[0] = LV_COLOR_ORANGE;
	
	static lv_style_t gauge_style;
	lv_style_init(&gauge_style);
	lv_style_set_bg_color(&gauge_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_line_color(&gauge_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_scale_end_color(&gauge_style, LV_STATE_DEFAULT, LV_COLOR_RED);
	lv_style_set_text_color(&gauge_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_line_width(&gauge_style, LV_STATE_DEFAULT, 2);
	lv_style_set_line_width(&gauge_style, LV_STATE_DEFAULT, 2);
	lv_style_set_border_side(&gauge_style, LV_STATE_DEFAULT, LV_BORDER_SIDE_NONE);
	lv_style_set_scale_end_line_width(&gauge_style, LV_STATE_DEFAULT, 2);
	lv_style_set_scale_end_border_width(&gauge_style, LV_STATE_DEFAULT, 2);

	Swr_gauge = lv_gauge_create(bg_middle, NULL);
	lv_obj_add_style(Swr_gauge, LV_GAUGE_PART_MAJOR, &gauge_style);
	lv_obj_add_style(Swr_gauge, LV_GAUGE_PART_MAIN, &gauge_style);
	lv_obj_add_style(Swr_gauge, LV_GAUGE_PART_NEEDLE, &gauge_style);
	lv_obj_align(Swr_gauge, NULL, LV_ALIGN_CENTER, 100, 45);
	lv_gauge_set_needle_count(Swr_gauge, 1, needle_colors);
	lv_obj_set_size(Swr_gauge, 160, 160);
	lv_gauge_set_range(Swr_gauge, 0, 100);
	int line_cnt, label_cnt = 5, sub_div = 3;
	line_cnt = (sub_div + 1) * (label_cnt - 1) + 1;
	lv_gauge_set_scale(Swr_gauge, 270, line_cnt, label_cnt);
	lv_gauge_set_critical_value(Swr_gauge, 40);
	lv_gauge_set_angle_offset(Swr_gauge,0);
	/*Allow recoloring of the images according to the needles' color*/
	lv_gauge_set_formatter_cb(Swr_gauge, lv_gauge_format_swr);
	lv_obj_set_style_local_image_recolor_opa(Swr_gauge, LV_GAUGE_PART_NEEDLE, LV_STATE_DEFAULT, LV_OPA_COVER);
	
	lv_obj_t* swr_label = lv_label_create(Swr_gauge, NULL);
	lv_obj_align(swr_label, Swr_gauge, LV_ALIGN_IN_BOTTOM_MID, 0, -15);
	lv_label_set_text(swr_label, "Swr");
	lv_obj_set_hidden(Swr_gauge, true);
	swr_vlabel = lv_label_create(Swr_gauge, NULL);
	lv_obj_align(swr_vlabel, Swr_gauge, LV_ALIGN_IN_BOTTOM_MID, 0, -30);
	lv_label_set_text(swr_vlabel, "0.0");
	lv_obj_set_hidden(Swr_gauge, true);

	pwr_gauge = lv_gauge_create(bg_middle, NULL);
	lv_obj_add_style(pwr_gauge, LV_GAUGE_PART_MAJOR, &gauge_style);
	lv_obj_add_style(pwr_gauge, LV_GAUGE_PART_MAIN, &gauge_style);
	lv_obj_add_style(pwr_gauge, LV_GAUGE_PART_NEEDLE, &gauge_style);
	lv_obj_align(pwr_gauge, NULL, LV_ALIGN_CENTER, -65, 45);
	lv_gauge_set_needle_count(pwr_gauge, 1, needle_colors);
	lv_obj_set_size(pwr_gauge, 160, 160);
	lv_gauge_set_range(pwr_gauge, 0, R.scale_watt *10);
	label_cnt = 5; sub_div = 3;
	line_cnt = (sub_div + 1) * (label_cnt - 1) + 1;
	lv_gauge_set_scale(pwr_gauge, 270, line_cnt, label_cnt);
	lv_gauge_set_critical_value(pwr_gauge, (R.scale_watt * 10));
	lv_gauge_set_angle_offset(pwr_gauge, 0);
	lv_gauge_set_formatter_cb(pwr_gauge, lv_gauge_format_pwr);
	/*Allow recoloring of the images according to the needles' color*/
	lv_obj_set_style_local_image_recolor_opa(pwr_gauge, LV_GAUGE_PART_NEEDLE, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_t* pwr_label = lv_label_create(pwr_gauge, NULL);
	lv_obj_align(pwr_label, pwr_gauge, LV_ALIGN_IN_BOTTOM_MID, 0, -15);
	lv_label_set_text(pwr_label, "Watt");
	pwr_vlabel = lv_label_create(pwr_gauge, NULL);
	lv_obj_align(pwr_vlabel, pwr_gauge, LV_ALIGN_IN_BOTTOM_MID, 0, -30);
	lv_label_set_text(pwr_vlabel, "0.0");
	lv_obj_set_hidden(pwr_gauge, true);

	gui_setup(scr);
	gui_si5351(scr);
	init_wifi_gui(scr);
	init_cal_ad8307_gui(scr);

	BfoLabel(get_bfo(active_vfo) / 100, 0);
	setfrequencylabel(get_vfo_frequency(active_vfo), get_vfo_frequency(1 - active_vfo));
	ToggleVfo(active_vfo, 0);

	// Set default focus to band selector
	lv_indev_set_group(encoder_indev_t, vfo_group); 
	lv_group_focus_obj(usb_button);

	xSemaphoreGive(GuiBinarySemaphore);
	int tt = 0;
	while (1) {
/*
* Call the lv_task_handler to process GUI changes
* Semaphores are there for thread save GUI changes from other threads
* lv_task_handler should not be executed if other tasks change the GUI
*/
		xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
		lv_task_handler();
		xSemaphoreGive(GuiBinarySemaphore);
		
		if ((f_cal_si5351 == false) && (f_bfo_adjust == false))  // Both si5351 and bfo adjust use optical encoder, so don't change vfo freq at the same time
		{
			int count = Enc_vfo.getCount();
			if (count != 0)
			{
				/*
				* Translate the encoder movement to a new frequency
				* If the band is changed also update the band roller
				*/
				char str[32];
				long frq = set_encoder_count_to_vfo_frequency(count, active_vfo);
				Enc_vfo.clearCount();
				setfrequencylabel(frq, get_vfo_frequency(1 - active_vfo));
				uint16_t i = get_band(str);
				lv_label_set_text(Band_btn_label, str);
			}
		}

/*		
* Process rotary press button messages
*/
		smeter->Draw(get_smeter());
		rotary_button.check();

		static volatile int lastPowerEncoding;
		static	bool	hide_rx = true;
		if ((f_rxtx) && (f_cal_ad8307 == false) && (f_cal_si5351 == false) && (f_bfo_adjust == false))
		{
			if (hide_rx == false)
			{
				hide_rx = true;		
				smeter->hide(true);
				lv_obj_set_hidden(Swr_gauge, false);
				lv_obj_set_hidden(pwr_gauge, false);
				hide_buttons(true);
			}
			pswr_sync_from_interrupt();
			calc_SWR_and_power();
			int currMillis = millis();
			if (currMillis - lastPowerEncoding > POLL_TIMER)
			{
				uint16_t swr, pep;


				swr = print_swr();
				pep = (uint16_t) (power_mw_pep / 100.0);
				lv_gauge_set_value(Swr_gauge, 0, swr/10);
				lv_gauge_set_value(pwr_gauge, 0, pep);
				char buf[32];
				sprintf(buf,"1:%d.%d", swr / 100, (swr / 10) % 10);
				lv_label_set_text(swr_vlabel, buf);
				print_p_mw(power_mw_pep);
				lv_label_set_text(pwr_vlabel, lcd_buf);
				//lv_label_set_text(pwr_vlabel, String((double)power_mw_pep / 1000.0).c_str());
				//Serial.println("SWR: " + String(swr) + " Power: " + String(power_mw_pep));
				lastPowerEncoding = currMillis;
			}

		}
		else
		{
			if (hide_rx == true)
			{
				hide_rx = false;
				smeter->hide(false);
				lv_obj_set_hidden(Swr_gauge, true);
				lv_obj_set_hidden(pwr_gauge, true);
				hide_buttons(false);	
				hide_setting(true);
				lv_obj_clear_state(setting_button, LV_STATE_CHECKED);
			}
		}
		// Capture optical encoder to update BFO frequency
		if (f_bfo_adjust)
		{
			int count = Enc_vfo.getCount();
			if (count != 0)
			{
				/*
				* Translate the encoder movement to a new bfo frequency
				* If the band is changed also update the band roller
				*/
				char str[32];
				long freq = 0L;
				Enc_vfo.clearCount();
				freq = set_encoder_count_to_bfo_frequency(count);
				setbfolabel(freq);
			}
		}
		
		
		if (f_cal_ad8307)
		{
			uint8_t inputq = check_input_cal();
			lv_color_t color_t = LV_COLOR_RED;
			String	sStr;

			switch (inputq)
			{
			case CAL_FWD:
				color_t = LV_COLOR_GREEN;
				sStr = String("Power Ok:");
				break;
			case CAL_REV:
				color_t = LV_COLOR_ORANGE;
				sStr = String("Power rev:");
				break;
			case CAL_BAD:
				color_t = LV_COLOR_RED;
				sStr = String("Power Low:");
				break;
			}
			uint16_t fwd, rev;

			fwd = analogRead(FWD_METER);
			rev = analogRead(REV_METER);
			updateBottomStatus(color_t, String(sStr + " Fwd: " + String(adc_ref * ((double)fwd / 4096.0)) + "V Rev: " + String(adc_ref * ((double)rev / 4096.0))).c_str(),0);
		}

		
		// Calibrate si5351
		
		if (f_cal_si5351 == true)
		{
			int count = Enc_vfo.getCount();
			if (count != 0)
			{
				/*
				* Translate the encoder movement to a new frequency
				* If the band is changed also update the band roller
				*/
				char str[32];
				long freq = 0L;
				Enc_vfo.clearCount();
				if (f_cal_si5351_vfo)
				{
					R.correction_si5351_no1 += count;
					if (R.correction_si5351_no1 < -100000L) R.correction_si5351_no1 = -100000L;
					if (R.correction_si5351_no1 > 100000L) R.correction_si5351_no1 = 0;
					freq = R.correction_si5351_no1;
					cal_vfo();
				}
				else
				{
					R.correction_si5351_no2 += count; 
					if (R.correction_si5351_no2 < -100000L) R.correction_si5351_no2 = -100000L;
					if (R.correction_si5351_no2 > 100000L) R.correction_si5351_no2 = 0;
					freq = R.correction_si5351_no2;
					cal_bfo();
				}
				si_setcalfrequency(freq);
			}
		}		
		vTaskDelay(1);
	}
}

void si_setcalfrequency(long freq)
{
	char	str[80];

	if (freq < 0)
		sprintf(str, "-%ld,%03ld", abs(freq) / 1000L, (abs(freq)) % 1000L);
	else
		sprintf(str, "%ld,%03ld", abs(freq) / 1000L, (abs(freq)) % 1000L);
	lv_label_set_text(si_frequency, str);
}

// Callbacks

/* Display flushing */
void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p)
{
	uint16_t c;

	tft.startWrite(); /* Start new TFT transaction */
	tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
	for (int y = area->y1; y <= area->y2; y++) {
		for (int x = area->x1; x <= area->x2; x++) {
			c = color_p->full;
			tft.writeColor(c, 1);
			color_p++;
		}
	}
	tft.endWrite(); /* terminate TFT transaction */
	lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

bool my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data)
{
	uint16_t touchX, touchY;

	bool touched = tft.getTouch(&touchX, &touchY, 600);

	if (tft.getRotation() == 1)
	{
		touchX = screenWidth - touchX;
	}

	if (!touched)
	{
		return false;
	}

	if (touchX > screenWidth || touchY > screenHeight)
	{
		Serial.println("Y or y outside of expected parameters..");
		Serial.print("y:");
		Serial.print(touchX);
		Serial.print(" x:");
		Serial.print(touchY);
	}
	else
	{

		data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

		/*Save the state and save the pressed coordinate*/
		//if(data->state == LV_INDEV_STATE_PR) touchpad_get_xy(&last_x, &last_y);

		/*Set the coordinates (if released use the last pressed coordinates)*/
		data->point.x = touchX;
		data->point.y = touchY;

	/*	Serial.print("Data x");
		Serial.println(touchX);

		Serial.print("Data y");
		Serial.println(touchY);
	*/
	}

	return false; /*Return `false` because we are not buffering and no more data to read*/
}

static void mode_button_eh(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_PRESSED)
	{	
		bool bchecked = lv_btn_get_state(obj) & LV_STATE_CHECKED;
		//Serial.println(String("state checked:") + String(bchecked));

		if (obj == usb_button)
		{
			if (bchecked)
			{
				lv_obj_clear_state(lsb_button, LV_STATE_CHECKED);
				switch_mode(MODE_USB, active_vfo);
			}
			else
			{
				lv_obj_add_state(lsb_button, LV_STATE_CHECKED);
				switch_mode(MODE_LSB, active_vfo);
			}
		}
		if (obj == lsb_button)
		{
			if (bchecked)
			{
				lv_obj_clear_state(usb_button, LV_STATE_CHECKED);
				switch_mode(MODE_LSB, active_vfo);
			}
			else
			{
				lv_obj_add_state(usb_button, LV_STATE_CHECKED);
				switch_mode(MODE_USB, active_vfo);
			}
		}
	}
}

static void hide_buttons(bool hide)
{
	lv_obj_set_hidden(usb_button, hide);
	lv_obj_set_hidden(lsb_button, hide);
	lv_obj_set_hidden(vfo1_button, hide);
	lv_obj_set_hidden(vfo2_button, hide);
	lv_obj_set_hidden(band_button, hide);
	lv_obj_set_hidden(setting_button, hide);
	lv_obj_set_hidden(bfo_label, hide);
	lv_obj_set_hidden(vfo2_frequency, hide);
	lv_obj_set_hidden(vfo2_unit, hide);
}

static void hide_setting(bool hide)
{
	smeter->hide(!hide);
	lv_obj_set_hidden(save_button, hide);
	lv_obj_set_hidden(wifi1_button, hide);
	lv_obj_set_hidden(si5351_button, hide);
	lv_obj_set_hidden(ad8307_button, hide);
	lv_obj_set_hidden(reset_button, hide);
	lv_obj_set_hidden(bfo_button, hide);
	lv_obj_clear_state(bfo_button, LV_STATE_CHECKED);
	//show_setting = !hide;
	//f_bfo_adjust = !f_bfo_adjust;
}



static void event_button_setting_cb(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_PRESSED)
	{
		bool bchecked = lv_btn_get_state(obj) & LV_STATE_CHECKED;
		f_bfo_adjust = false;
		hide_setting(!bchecked);
	}
}

static void event_button_bfo_cb(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_PRESSED)
	{
		bool bchecked = lv_btn_get_state(obj) & LV_STATE_CHECKED;
		f_bfo_adjust = bchecked;
	}
}

static void event_button_wifi_cb(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_PRESSED) {
		show_wifi_gui(true);
	}
}

static void event_button_si5351_cb(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_PRESSED) {
		vTaskDelay(100);
		f_cal_si5351 = true;
		lv_obj_move_foreground(bg_middle3);
		lv_indev_set_group(encoder_indev_t, si_5351_group);
		si_setcalfrequency(R.correction_si5351_no1);
		lv_obj_add_state(si_vfo_button, LV_STATE_CHECKED);
		start_cal();
	}
}

static void event_button_reset_cb(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_PRESSED) {
		memset(&R, 0, sizeof(R));
		init_vfo();
		SaveEEPROM();
		ESP.restart();
	}
}

static void event_button_ad8307_cb(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_PRESSED) {
		lv_obj_move_foreground(bg_calgui);
		lv_obj_move_foreground(bg_top_cal);
		lv_indev_set_group(encoder_indev_t, cal_group);
		start_ad8307_cal();
	}
}

static void mode_button_vfo(lv_obj_t* obj, lv_event_t event)
{
	/*if (event == LV_EVENT_CLICKED) {
		//printf("Clicked\n");
	}
	else */  
	if (event == LV_EVENT_PRESSED /*LV_EVENT_VALUE_CHANGED*/) {

		bool bchecked = lv_btn_get_state(obj) & LV_STATE_CHECKED;
		//Serial.println(String("state vfo checked:") + String(bchecked));

		if (obj == vfo1_button)
		{
			if (bchecked)
			{
				lv_obj_clear_state(vfo2_button, LV_STATE_CHECKED);
				active_vfo = 0;
			}
			else
			{
				lv_obj_add_state(vfo2_button, LV_STATE_CHECKED);
				active_vfo = 1;
			}
		}
		if (obj == vfo2_button)
		{
			if (bchecked)
			{
				lv_obj_clear_state(vfo1_button, LV_STATE_CHECKED);
				active_vfo = 1;
			}
			else
			{
				lv_obj_add_state(vfo1_button, LV_STATE_CHECKED);
				active_vfo = 0;
			}
		}
		long frq = switch_vfo(active_vfo);
		setfrequencylabel(frq, get_vfo_frequency(1 - active_vfo));
	}
}

void updateBottomStatus(lv_color_t color, String text, uint8_t sem) {
	if (sem) xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	lv_obj_set_style_local_bg_color(bg_top, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, color);
	lv_label_set_text(label_status, text.c_str());
	if (sem) xSemaphoreGive(GuiBinarySemaphore);
}

void showWifilabel(bool show) {
	xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	lv_obj_set_hidden(label_wifi, show);
	xSemaphoreGive(GuiBinarySemaphore);
}


// this function is used to set the display frequency, it is not using semaphore because this is done from main loop
void setfrequencylabel(long freq, long freq2, uint8_t sem) {
	if (sem) xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	char str[80];

	sprintf(str, "%3d.%03d,%02d", freq / 1000000, (freq / 1000) % 1000, (freq / 10) % 100);
	lv_label_set_text(vfo_frequency, str);
	
	sprintf(str, "%3d.%03d,%02d", freq2 / 1000000, (freq2 / 1000) % 1000, (freq2 / 10) % 100);
	lv_label_set_text(vfo2_frequency, str);
	if (sem) xSemaphoreGive(GuiBinarySemaphore);
}

void setbfolabel(long freq, uint8_t sem) {
	if (sem) xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	char str[80];

	sprintf(str, "Bfo:%2d.%03d,%02d Khz", freq / 1000000, (freq / 1000) % 1000, (freq / 10) % 100);
	lv_label_set_text(bfo_label, str);
	if (sem) xSemaphoreGive(GuiBinarySemaphore);
}

static void event_band_button(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_PRESSED) {
		char	buf[80];
		long	frq;

		strcpy(buf, lv_label_get_text(Band_btn_label));
		buf[2] = '\0';
		frq = band_select_next(buf, active_vfo);
		setfrequencylabel(frq, get_vfo_frequency(1 - active_vfo));
		set_vfo_frequency(frq, active_vfo);
		lv_label_set_text(Band_btn_label, buf);
	}
}

void ToggleTX(uint8_t show) {
	xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	if (show)
		lv_led_on(Tx_led1);
	else
		lv_led_off(Tx_led1);
	xSemaphoreGive(GuiBinarySemaphore);
}


static void event_button_save(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		save_vfo();
		SaveEEPROM();
	}
}

void ToggleSetup(bool show, uint8_t sem) 
{
	if (sem) xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	if (show)
	{
		lv_obj_move_foreground(bg_middle2);
		lv_indev_set_group(encoder_indev_t, setup_group);
		lv_group_focus_obj(btn_matrix);
	}
	else
	{
		lv_obj_move_background(bg_middle2);
		lv_indev_set_group(encoder_indev_t, vfo_group);
		lv_group_focus_obj(lsb_button);
	}
	if (sem) xSemaphoreGive(GuiBinarySemaphore);
}

void BfoLabel(uint32_t num, uint8_t sem) {
	if (sem)
		xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	char str[32];
	sprintf(str, "Bfo: %ld.%ld Khz", num / 10, num % 10);
	lv_label_set_text(bfo_label, str);
	//lv_spinbox_set_value(bfo_spinbox, num);
	if (sem)
		xSemaphoreGive(GuiBinarySemaphore);
}

void Togglemode(int mode, uint8_t sem)
{
	if (sem)
		xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY); 
	if (mode == MODE_LSB)
	{
		lv_btn_set_state(lsb_button, LV_BTN_STATE_CHECKED_RELEASED);
		lv_btn_set_state(usb_button, LV_BTN_STATE_RELEASED);
	}
	if (mode == MODE_USB)
	{
		lv_btn_set_state(usb_button, LV_BTN_STATE_CHECKED_RELEASED);
		lv_btn_set_state(lsb_button, LV_BTN_STATE_RELEASED);
	}
	if (sem)
		xSemaphoreGive(GuiBinarySemaphore);
}

void ToggleVfo(int active_vfo, uint8_t sem)
{
	if (sem) xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	if (active_vfo)
	{
		lv_obj_add_state(vfo2_button, LV_STATE_CHECKED);
		lv_obj_clear_state(vfo1_button, LV_STATE_CHECKED);
	}
	else
	{
		lv_obj_add_state(vfo1_button, LV_STATE_CHECKED);
		lv_obj_clear_state(vfo2_button, LV_STATE_CHECKED);
	}
	if (sem) xSemaphoreGive(GuiBinarySemaphore);
}

static void gui_setup_event_handler(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_VALUE_CHANGED) {
		uint16_t abutton = lv_btnmatrix_get_active_btn(obj);
		switch (abutton)
		{
		case 0:
			show_wifi_gui(true);
			break;
		case 1:
			//Si5351 calibration
			vTaskDelay(100);
			f_cal_si5351 = true;
			lv_obj_move_foreground(bg_middle3);
			lv_indev_set_group(encoder_indev_t, si_5351_group);
			si_setcalfrequency(R.correction_si5351_no1);
			lv_obj_add_state(si_vfo_button, LV_STATE_CHECKED); 
			start_cal();
			break;
		case 2:
			lv_obj_move_foreground(bg_calgui);
			lv_obj_move_foreground(bg_top_cal);
			lv_indev_set_group(encoder_indev_t, cal_group);
			start_ad8307_cal();
			break;
		case 3:
			memset(&R, 0, sizeof(R));
			init_vfo();
			SaveEEPROM();
			ESP.restart();
			break;
		}
		
	}
}

static void gui_setup(lv_obj_t* scr)
{
	static const char* setup_btnm_map[] = { LV_SYMBOL_WIFI, "Si5351", "Calibrate\n AD8307", "Reset", "" };

	bg_middle2 = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_middle2, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_middle2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_middle2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(bg_middle2, 0, topHeight);
	lv_obj_set_size(bg_middle2, LV_HOR_RES, screenHeight - topHeight);
	lv_obj_move_background(bg_middle2);

	btn_matrix = lv_btnmatrix_create(bg_middle2, NULL);
	lv_btnmatrix_set_map(btn_matrix, setup_btnm_map);
	lv_obj_align(btn_matrix, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_btnmatrix_set_btn_ctrl(btn_matrix, 6, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_obj_set_event_cb(btn_matrix, gui_setup_event_handler);
	setup_group = lv_group_create();
	lv_group_add_obj(setup_group, btn_matrix);

	lv_obj_t* pwd_label1 = lv_label_create(bg_middle2, NULL);
	lv_label_set_text(pwd_label1, "Setup");
	lv_obj_align(pwd_label1, btn_matrix, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
}


static void si_vfo_button_cb(lv_obj_t* obj, lv_event_t e)
{
	if (e == LV_EVENT_PRESSED )
	{
		bool bchecked = lv_btn_get_state(obj) & LV_STATE_CHECKED;
		if (obj == si_vfo_button)
		{
			if (bchecked)
			{
				lv_obj_clear_state(si_bfo_button, LV_STATE_CHECKED);
				f_cal_si5351_vfo = true;
				lv_label_set_text(si_label1, "Calibrating si5351 VFO");
				si_setcalfrequency(R.correction_si5351_no1);
			}
			else
			{
				lv_obj_add_state(si_bfo_button, LV_STATE_CHECKED);
				f_cal_si5351_vfo = false;
				lv_label_set_text(si_label1, "Calibrating si5351 BFO");
				si_setcalfrequency(R.correction_si5351_no2);
			}
		}
	}
}

static void si_bfo_button_cb(lv_obj_t* obj, lv_event_t e)
{
	if (e == LV_EVENT_PRESSED) 
	{
		bool bchecked = lv_btn_get_state(obj) & LV_STATE_CHECKED;
		if (obj == si_bfo_button)
		{
			if (bchecked)
			{
				lv_obj_clear_state(si_vfo_button, LV_STATE_CHECKED);
				f_cal_si5351_vfo = false;
				lv_label_set_text(si_label1, "Calibrating si5351 BFO");
				si_setcalfrequency(R.correction_si5351_no2);
			}
			else
			{
				lv_obj_add_state(si_vfo_button, LV_STATE_CHECKED);
				f_cal_si5351_vfo = true;
				lv_label_set_text(si_label1, "Calibrating si5351 VFO");
				si_setcalfrequency(R.correction_si5351_no1);
			}
		}
	}
}

static void si_save_button_cb(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_PRESSED) {
		SaveEEPROM();
		updateBottomStatus(LV_COLOR_GREEN, "Si5351 calibration saved", 0);
		lv_obj_move_foreground(bg_middle);
		lv_obj_move_background(bg_middle3);
		lv_indev_set_group(encoder_indev_t, vfo_group);
		f_cal_si5351 = false;
	}
}

static void si_cancel_button_cb(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_PRESSED) {
		lv_obj_move_foreground(bg_middle);
		lv_obj_move_background(bg_middle3);
		lv_indev_set_group(encoder_indev_t, vfo_group);
		f_cal_si5351 = false;
	}
}

static void gui_si5351(lv_obj_t* scr)
{
	bg_middle3 = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_middle3, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_middle3, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_middle3, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(bg_middle3, 0, topHeight);
	lv_obj_set_size(bg_middle3, LV_HOR_RES, screenHeight - topHeight);
	lv_obj_move_background(bg_middle3);

	si_vfo_button = lv_btn_create(bg_middle3, NULL);
	lv_obj_add_style(si_vfo_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(si_vfo_button, si_vfo_button_cb);
	lv_obj_align(si_vfo_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_btn_set_checkable(si_vfo_button, true);
	lv_obj_set_size(si_vfo_button, bottombutton_width, bottomHeight);
	lv_obj_t* label = lv_label_create(si_vfo_button, NULL);
	lv_label_set_text(label, "VFO");
	lv_obj_add_state(si_vfo_button, LV_STATE_CHECKED);

	si_bfo_button = lv_btn_create(bg_middle3, NULL);
	lv_obj_add_style(si_bfo_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(si_bfo_button, si_bfo_button_cb);
	lv_obj_align(si_bfo_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, bottombutton_width1, 0);
	lv_btn_set_checkable(si_bfo_button, true);
	lv_btn_toggle(vfo1_button); 
	lv_obj_set_size(si_bfo_button, bottombutton_width, bottomHeight);
	label = lv_label_create(si_bfo_button, NULL);
	lv_label_set_text(label, "BFO");

	si_save_button = lv_btn_create(bg_middle3, NULL);
	lv_obj_add_style(si_save_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(si_save_button, si_save_button_cb);
	lv_obj_align(si_save_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 2 * bottombutton_width1, 0);
	lv_obj_set_size(si_save_button, bottombutton_width, bottomHeight);
	label = lv_label_create(si_save_button, NULL);
	lv_label_set_text(label, "Save");

	si_cancel_button = lv_btn_create(bg_middle3, NULL);
	lv_obj_add_style(si_cancel_button, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(si_cancel_button, si_cancel_button_cb);
	lv_obj_align(si_cancel_button, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 3 * bottombutton_width1, 0);
	lv_obj_set_size(si_cancel_button, bottombutton_width, bottomHeight);
	label = lv_label_create(si_cancel_button, NULL);
	lv_label_set_text(label, "Cancel");

	static lv_style_t text_style;
	lv_style_init(&text_style);

	/*Set a background color and a radius*/
	lv_style_set_radius(&text_style, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_opa(&text_style, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&text_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_value_align(&text_style, LV_STATE_DEFAULT, LV_ALIGN_CENTER);
	lv_style_set_text_font(&text_style, LV_STATE_DEFAULT, &FreeSansOblique42);

	si_frequency = lv_label_create(bg_middle3, NULL);
	lv_label_set_long_mode(si_frequency, LV_LABEL_LONG_CROP);
	lv_obj_add_style(si_frequency, LV_OBJ_PART_MAIN, &text_style);
	lv_obj_set_width(si_frequency, LV_HOR_RES - 20);
	lv_label_set_text(si_frequency, "");
	lv_obj_set_height(si_frequency, 40);
	lv_obj_align(si_frequency, bg_middle, LV_ALIGN_CENTER, -30, -84);
	lv_label_set_align(si_frequency, LV_LABEL_ALIGN_CENTER);

	si_unit = lv_label_create(bg_middle3, NULL);
	lv_obj_add_style(si_unit, LV_OBJ_PART_MAIN, &text_style);
	lv_label_set_text(si_unit, "Khz");
	lv_obj_set_height(si_unit, 40);
	lv_obj_align(si_unit, bg_middle, LV_ALIGN_CENTER, 110, -87);
	lv_label_set_align(si_unit, LV_LABEL_ALIGN_CENTER);


	si_label1 = lv_label_create(bg_middle3, NULL);
	lv_label_set_text(si_label1, "Calibrating si5351 VFO");
	lv_obj_align(si_label1, NULL, LV_ALIGN_CENTER, 0, 0);

	si_5351_group = lv_group_create(); 
	lv_group_add_obj(si_5351_group, si_vfo_button);
	lv_group_add_obj(si_5351_group, si_bfo_button);
	lv_group_add_obj(si_5351_group, si_save_button);
	
}

static void mode_button_cal_cancel(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		lv_obj_move_background(bg_calgui);
		lv_obj_move_foreground(bg_middle);
		lv_obj_move_background(bg_top_cal);
		lv_obj_move_foreground(bg_top);
		f_cal_ad8307 = false;
		start_measurement();
		lv_indev_set_group(encoder_indev_t, vfo_group);
	}
}

static void mode_button_cal_save(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		lv_obj_move_background(bg_calgui);
		lv_obj_move_foreground(bg_middle);
		lv_obj_move_background(bg_top_cal);
		lv_obj_move_foreground(bg_top);
		f_cal_ad8307 = false;
		Serial.println("db10m[0]: " + String(R.cal_AD[0].db10m) + " Fwd[0]: " + String(R.cal_AD[0].Fwd) + " Rev[0]: " + String(R.cal_AD[0].Rev));
		Serial.println("db10m[1]: " + String(R.cal_AD[1].db10m) + " Fwd[1]: " + String(R.cal_AD[1].Fwd) + " Rev[1]: " + String(R.cal_AD[1].Rev));
		start_measurement();
		SaveEEPROM();
		lv_indev_set_group(encoder_indev_t, vfo_group);
	}
}

static void lv_spinbox_increment_event_ad1(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_increment(ad_spinbox1);
		R.cal_AD[0].db10m = lv_spinbox_get_value(ad_spinbox1);
	}
}

static void lv_spinbox_decrement_event_ad1(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_decrement(ad_spinbox1);
		R.cal_AD[0].db10m = lv_spinbox_get_value(ad_spinbox1);
	}
}

static void lv_spinbox_increment_event_ad2(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_increment(ad_spinbox2);
		R.cal_AD[1].db10m = lv_spinbox_get_value(ad_spinbox2);
	}
}

static void lv_spinbox_increment_event_ad4(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_increment(ad_spinbox4);
		R.cal_AD[1].Fwd = (double)lv_spinbox_get_value(ad_spinbox4) / 1000.0;
	}
}

static void lv_spinbox_decrement_event_ad3(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_decrement(ad_spinbox3);
		R.scale_watt = lv_spinbox_get_value(ad_spinbox3);
	}
}

static void lv_spinbox_increment_event_ad3(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_increment(ad_spinbox3);
		R.scale_watt = lv_spinbox_get_value(ad_spinbox3);
	}
}

static void lv_spinbox_decrement_event_ad2(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_decrement(ad_spinbox2);
		R.cal_AD[1].db10m = lv_spinbox_get_value(ad_spinbox2);
	}
}

static void lv_spinbox_decrement_event_ad4(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_decrement(ad_spinbox4);
		R.cal_AD[1].Fwd = (double)lv_spinbox_get_value(ad_spinbox4) / 1000.0;
	}
}

static void lv_spinbox_event_save(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		
		R.cal_AD[0].db10m = lv_spinbox_get_value(ad_spinbox1);
		R.cal_AD[0].Rev = R.cal_AD[0].Fwd = adc_ref * ((double)analogRead(FWD_METER) / 4096.0);
		lv_spinbox_set_value(ad_spinbox4, (uint32_t)(R.cal_AD[0].Fwd * 1000.0));
	}
}

static void lv_spinbox_event_save1(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {

		R.cal_AD[1].db10m = lv_spinbox_get_value(ad_spinbox2);
		R.cal_AD[1].Rev = R.cal_AD[1].Fwd = adc_ref * ((double)analogRead(FWD_METER) / 4096.0);
		lv_spinbox_set_value(ad_spinbox4, (uint32_t)(R.cal_AD[0].Fwd * 1000.0));
	}
}

static void lv_spinbox_event_copy(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) 
	{
		// Set second calibration point at 30 dB less
		R.cal_AD[1].db10m = R.cal_AD[0].db10m - 300;
		R.cal_AD[1].Fwd = R.cal_AD[0].Fwd - LOGAMP1_SLOPE * .001 * 30;
		R.cal_AD[1].Rev = R.cal_AD[0].Fwd - LOGAMP2_SLOPE * .001 * 30;
		lv_spinbox_set_value(ad_spinbox2, R.cal_AD[1].db10m);
	}
}

static void lv_spinbox_event_save4(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		uint32_t value;

		value = lv_spinbox_get_value(ad_spinbox4);
		R.cal_AD[0].Rev = R.cal_AD[0].Fwd = (double) value / 1000.0;
	}
}

static void lv_spinbox_event_save5(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		uint32_t value;

		value = lv_spinbox_get_value(ad_spinbox4);
		R.cal_AD[1].Rev = R.cal_AD[1].Fwd = (double)value / 1000.0;
	}
}

static void init_cal_ad8307_gui(lv_obj_t* scr)
{
	bg_top_cal = lv_obj_create(scr, bg_top);
	lv_obj_move_background(bg_top_cal);

	bg_calgui = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_calgui, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_calgui, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_calgui, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(bg_calgui, 0, topHeight);
	lv_obj_set_size(bg_calgui, LV_HOR_RES, screenHeight - topHeight);
	lv_obj_move_background(bg_calgui);

	label_cal = lv_label_create(bg_top_cal, NULL);
	lv_label_set_text(label_cal, "AD8307 Calibration");
	lv_obj_align(label_cal, NULL, LV_ALIGN_IN_TOP_MID, 50, 5);

#define XPOS_SPIN		-50
#define POS_SPIN		-80
#define POS_SPIN_DELTA	45

	cal_group = lv_group_create();

	ad_spinbox1 = lv_spinbox_create(bg_calgui, NULL);
	lv_spinbox_set_range(ad_spinbox1, 0, 999);
	lv_spinbox_set_digit_format(ad_spinbox1, 3, 2);
	lv_spinbox_step_prev(ad_spinbox1);
	lv_obj_set_width(ad_spinbox1, 50);
	lv_obj_align(ad_spinbox1, NULL, LV_ALIGN_CENTER, XPOS_SPIN, POS_SPIN);
	lv_spinbox_set_value(ad_spinbox1, R.cal_AD[0].db10m);

	ad_spinbox2 = lv_spinbox_create(bg_calgui, NULL);
	lv_spinbox_set_range(ad_spinbox2, 0, 999);
	lv_spinbox_set_digit_format(ad_spinbox2, 3, 2);
	lv_spinbox_step_prev(ad_spinbox2);
	lv_obj_set_width(ad_spinbox2, 50);
	lv_obj_align(ad_spinbox2, NULL, LV_ALIGN_CENTER, XPOS_SPIN, POS_SPIN + POS_SPIN_DELTA);
	lv_spinbox_set_value(ad_spinbox2, R.cal_AD[1].db10m);

	ad_spinbox3 = lv_spinbox_create(bg_calgui, NULL);
	lv_spinbox_set_range(ad_spinbox3, 0, 999);
	lv_spinbox_set_digit_format(ad_spinbox3, 3, 0);
	lv_spinbox_step_prev(ad_spinbox3);
	lv_obj_set_width(ad_spinbox3, 50);
	lv_obj_align(ad_spinbox3, NULL, LV_ALIGN_CENTER, XPOS_SPIN, POS_SPIN + 2* POS_SPIN_DELTA);
	lv_spinbox_set_value(ad_spinbox3, R.scale_watt);

	ad_spinbox4 = lv_spinbox_create(bg_calgui, NULL);
	lv_spinbox_set_range(ad_spinbox4, 0, 9999);
	lv_spinbox_set_digit_format(ad_spinbox4, 4, 1);
	lv_spinbox_step_prev(ad_spinbox4);
	lv_obj_set_width(ad_spinbox4, 50);
	lv_obj_align(ad_spinbox4, NULL, LV_ALIGN_CENTER, XPOS_SPIN, POS_SPIN + 3 * POS_SPIN_DELTA);
	lv_spinbox_set_value(ad_spinbox4, (int32_t)(R.cal_AD[0].Fwd * 1000.0));

	lv_obj_t* pwd_label1 = lv_label_create(bg_calgui, NULL);
	lv_label_set_text(pwd_label1, "Ref1:");
	lv_obj_align(pwd_label1, ad_spinbox1, LV_ALIGN_OUT_LEFT_MID, -45, 0); 

	pwd_label1 = lv_label_create(bg_calgui, NULL);
	lv_label_set_text(pwd_label1, "dbm");
	lv_obj_align(pwd_label1, ad_spinbox1, LV_ALIGN_OUT_LEFT_MID, -45, 14);

	pwd_label1 = lv_label_create(bg_calgui, NULL);
	lv_label_set_text(pwd_label1, "Ref2:");
	lv_obj_align(pwd_label1, ad_spinbox2, LV_ALIGN_OUT_LEFT_MID, -45, 0);

	pwd_label1 = lv_label_create(bg_calgui, NULL);
	lv_label_set_text(pwd_label1, "dbm");
	lv_obj_align(pwd_label1, ad_spinbox2, LV_ALIGN_OUT_LEFT_MID, -45, 14); 
	
	pwd_label1 = lv_label_create(bg_calgui, NULL);
	lv_label_set_text(pwd_label1, "Watt:");
	lv_obj_align(pwd_label1, ad_spinbox3, LV_ALIGN_OUT_LEFT_MID, -45, 0);
	
	pwd_label1 = lv_label_create(bg_calgui, NULL);
	lv_label_set_text(pwd_label1, "Volt:");
	lv_obj_align(pwd_label1, ad_spinbox4, LV_ALIGN_OUT_LEFT_MID, -45, 0);

	lv_coord_t h = lv_obj_get_height(ad_spinbox1);
	lv_obj_t* btn_plus1 = lv_btn_create(bg_calgui, NULL);
	lv_obj_set_size(btn_plus1, h, h);
	lv_obj_align(btn_plus1, ad_spinbox1, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_theme_apply(btn_plus1, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn_plus1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
	lv_obj_set_event_cb(btn_plus1, lv_spinbox_increment_event_ad1);

	lv_obj_t* btn_min1 = lv_btn_create(bg_calgui, btn_plus1);
	lv_obj_align(btn_min1, ad_spinbox1, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_event_cb(btn_min1, lv_spinbox_decrement_event_ad1);
	lv_obj_set_style_local_value_str(btn_min1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

	lv_obj_t* btn_ok1 = lv_btn_create(bg_calgui, NULL);
	lv_obj_set_size(btn_ok1, h, h);
	lv_obj_align(btn_ok1, ad_spinbox1, LV_ALIGN_OUT_RIGHT_MID, 40, 0);
	lv_theme_apply(btn_ok1, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn_ok1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_OK);
	lv_obj_set_event_cb(btn_ok1, lv_spinbox_event_save);

	lv_obj_t* btn_plus2 = lv_btn_create(bg_calgui, NULL);
	lv_obj_set_size(btn_plus2, h, h);
	lv_obj_align(btn_plus2, ad_spinbox2, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_theme_apply(btn_plus2, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn_plus2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
	lv_obj_set_event_cb(btn_plus2, lv_spinbox_increment_event_ad2);

	lv_obj_t* btn_min2 = lv_btn_create(bg_calgui, btn_plus2);
	lv_obj_align(btn_min2, ad_spinbox2, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_event_cb(btn_min2, lv_spinbox_decrement_event_ad2);
	lv_obj_set_style_local_value_str(btn_min2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

	lv_obj_t* btn_ok2 = lv_btn_create(bg_calgui, NULL);
	lv_obj_set_size(btn_ok2, h, h);
	lv_obj_align(btn_ok2, ad_spinbox2, LV_ALIGN_OUT_RIGHT_MID, 40, 0);
	lv_theme_apply(btn_ok2, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn_ok2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_OK);
	lv_obj_set_event_cb(btn_ok2, lv_spinbox_event_save1);

	lv_obj_t* btn_refr = lv_btn_create(bg_calgui, NULL);
	lv_obj_set_size(btn_refr, h, h);
	lv_obj_align(btn_refr, ad_spinbox2, LV_ALIGN_OUT_RIGHT_MID, 80, 0);
	lv_theme_apply(btn_refr, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn_refr, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_REFRESH);
	lv_obj_set_event_cb(btn_refr, lv_spinbox_event_copy);
	

	lv_obj_t* btn_save = lv_btn_create(bg_calgui, NULL);
	lv_obj_add_style(btn_save, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(btn_save, mode_button_cal_save);
	lv_obj_align(btn_save, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_obj_set_size(btn_save, bottombutton_width, bottomHeight);
	lv_obj_set_style_local_value_str(btn_save, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "save");
	
	lv_obj_t* btn_cancel = lv_btn_create(bg_calgui, NULL);
	lv_obj_add_style(btn_cancel, LV_BTN_PART_MAIN, &style_btn);
	lv_obj_set_event_cb(btn_cancel, mode_button_cal_cancel);
	lv_obj_align(btn_cancel, NULL, LV_ALIGN_IN_BOTTOM_LEFT, LV_HOR_RES - bottombutton_width, 0);
	lv_obj_set_size(btn_cancel, bottombutton_width, bottomHeight);
	lv_obj_set_style_local_value_str(btn_cancel, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Cancel");
	
	lv_obj_t* btn_plus3 = lv_btn_create(bg_calgui, NULL);
	lv_obj_set_size(btn_plus3, h, h);
	lv_obj_align(btn_plus3, ad_spinbox3, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_theme_apply(btn_plus3, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn_plus3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
	lv_obj_set_event_cb(btn_plus3, lv_spinbox_increment_event_ad3);

	lv_obj_t* btn_min3 = lv_btn_create(bg_calgui, btn_plus3);
	lv_obj_align(btn_min3, ad_spinbox3, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_event_cb(btn_min3, lv_spinbox_decrement_event_ad3);
	lv_obj_set_style_local_value_str(btn_min3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

	lv_obj_t* btn_plus4 = lv_btn_create(bg_calgui, NULL);
	lv_obj_set_size(btn_plus4, h, h);
	lv_obj_align(btn_plus4, ad_spinbox4, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_theme_apply(btn_plus4, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn_plus4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
	lv_obj_set_event_cb(btn_plus4, lv_spinbox_increment_event_ad4);

	lv_obj_t* btn_min4 = lv_btn_create(bg_calgui, btn_plus4);
	lv_obj_align(btn_min4, ad_spinbox4, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_event_cb(btn_min4, lv_spinbox_decrement_event_ad4);
	lv_obj_set_style_local_value_str(btn_min4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

	lv_obj_t* btn_ok4 = lv_btn_create(bg_calgui, NULL);
	lv_obj_set_size(btn_ok4, h, h);
	lv_obj_align(btn_ok4, ad_spinbox4, LV_ALIGN_OUT_RIGHT_MID, 40, 0);
	lv_theme_apply(btn_ok4, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn_ok4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "R1");
	lv_obj_set_event_cb(btn_ok4, lv_spinbox_event_save4);

	lv_obj_t* btn_ok5 = lv_btn_create(bg_calgui, NULL);
	lv_obj_set_size(btn_ok5, h, h);
	lv_obj_align(btn_ok5, ad_spinbox4, LV_ALIGN_OUT_RIGHT_MID, 80, 0);
	lv_theme_apply(btn_ok5, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn_ok5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "R2");
	lv_obj_set_event_cb(btn_ok5, lv_spinbox_event_save5);

//	pwd_label1 = lv_label_create(bg_calgui, NULL);
//	lv_label_set_text(pwd_label1, "Watt");
//	lv_obj_align(pwd_label1, btn_min3, LV_ALIGN_OUT_LEFT_MID, -10,0);

	lv_group_add_obj(cal_group, btn_min1);
	lv_group_add_obj(cal_group, ad_spinbox1);
	lv_group_add_obj(cal_group, btn_plus1);
	lv_group_add_obj(cal_group, btn_ok1);

	lv_group_add_obj(cal_group, btn_min2);
	lv_group_add_obj(cal_group, ad_spinbox2);
	lv_group_add_obj(cal_group, btn_plus2);
	lv_group_add_obj(cal_group, btn_ok2);
	lv_group_add_obj(cal_group, btn_refr);

	lv_group_add_obj(cal_group, btn_min3);
	lv_group_add_obj(cal_group, ad_spinbox3);
	lv_group_add_obj(cal_group, btn_plus3);

	lv_group_add_obj(cal_group, btn_min4);
	lv_group_add_obj(cal_group, ad_spinbox4);
	lv_group_add_obj(cal_group, btn_plus4);
	lv_group_add_obj(cal_group, btn_ok4);
	lv_group_add_obj(cal_group, btn_ok5);

	lv_group_add_obj(cal_group, btn_save);
	lv_group_add_obj(cal_group, btn_cancel);
}

static void start_ad8307_cal()
{
	stop_measurement();
	f_cal_ad8307 = true;
}

