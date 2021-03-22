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
#define ROTARY_A      14      
#define ROTARY_B      19
#define ROTARY_PRESS  26

LV_FONT_DECLARE(FreeSansOblique);

/*-------------------------------------------------------
   Optical Rotary encoder settings (used for frequency)
--------------------------------------------------------*/
#define PULSE_INPUT_PIN 34  // Rotaty Encoder A
#define PULSE_CTRL_PIN  35  // Rotaty Encoder B

ESP32Encoder    Enc_vfo;

/*-------------------------------------------------------
   Rotary hardware connection (used for bandswitching, and mode switching)
--------------------------------------------------------*/
static	ESP32Encoder	GuiEncoder;
static	int				enc_button_state = LV_INDEV_STATE_REL;
AceButton				rotary_button(ROTARY_PRESS);
void					rotary_button_eventhandler(AceButton*, uint8_t, uint8_t);

int active_vfo = 0;  // holds which vfo is active selected using radio buttons

//Ticker tick; /* timer for interrupt handler */
TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

lv_obj_t* slider_label;
int screenWidth = 320;
int screenHeight = 240;
int	bottomHeight = 20;
int	topHeight = 30;

lv_obj_t* ddlist;
lv_obj_t* bg_top;
lv_obj_t* bg_middle;
lv_obj_t* bg_bottom;
lv_obj_t* mbox_connect;
lv_obj_t* gauge;
lv_obj_t* label_status;
lv_obj_t* s_meter;
lv_obj_t* usb_button;
lv_obj_t* lsb_button;
lv_obj_t* vfo_frequency;
lv_obj_t* label_wifi;
lv_obj_t* vfo_unit;
lv_obj_t* vfo1_button;
lv_obj_t* vfo2_button;
lv_obj_t* Band_roller;
lv_obj_t* Tx_led1;
lv_obj_t* bfo_spinbox;
lv_obj_t* s_canvas;
lv_obj_t* bg_middle2;
lv_obj_t* bg_middle3;
lv_obj_t* si_spinbox;
lv_obj_t* si_spinbox2;

CSmeter* smeter;

static lv_color_t* cbuf;

#if USE_LV_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char* file, uint32_t line, const char* dsc)
{

	Serial.printf("%s@%d->%s\r\n", file, line, dsc);
	delay(100);
}
#endif

bool read_encoder(lv_indev_drv_t* indev, lv_indev_data_t* data)
{
	data->enc_diff = GuiEncoder.getCount();
	data->state = enc_button_state;
	if (data->enc_diff > 0)
		data->enc_diff = 1;
	if (data->enc_diff < 0)
		data->enc_diff = -1;
	GuiEncoder.clearCount();
	return false;
}


void guisetup() {
	xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	pinMode(ROTARY_PRESS, INPUT);
	rotary_button.setEventHandler(rotary_button_eventhandler);
	rotary_button.getButtonConfig()->setFeature(ButtonConfig::kFeatureLongPress);
	rotary_button.getButtonConfig()->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
	ESP32Encoder::useInternalWeakPullResistors = NONE;
	GuiEncoder.attachHalfQuad(ROTARY_A, ROTARY_B);
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
	lv_indev_t *encoder_indev_t = lv_indev_drv_register(&indev_drv_enc);

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

	bg_bottom = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_bottom, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_bottom, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_bottom, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
	lv_obj_set_pos(bg_bottom, 0, screenHeight - bottomHeight);
	lv_obj_set_size(bg_bottom, LV_HOR_RES, bottomHeight);
	
	label_status = lv_label_create(bg_bottom, NULL);
	lv_label_set_long_mode(label_status, LV_LABEL_LONG_SROLL_CIRC);
	lv_obj_set_width(label_status, LV_HOR_RES - 20);
	lv_label_set_text(label_status, "");
	lv_obj_align(label_status, NULL, LV_ALIGN_CENTER, 0, 0);

	bg_middle = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_middle, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_middle, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_middle, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(bg_middle, 0, topHeight);
	lv_obj_set_size(bg_middle, LV_HOR_RES, screenHeight - topHeight - bottomHeight);

	smeter = new CSmeter(bg_middle, LV_ALIGN_CENTER, 240, 50, 0, 60);

	// Create groups for encoder support for the different screens
	lv_group_t* vfo_group = lv_group_create();
	lv_indev_set_group(encoder_indev_t, vfo_group);

	usb_button = lv_btn_create(bg_top, NULL);
	lv_obj_set_event_cb(usb_button, mode_button_eh);
	lv_obj_align(usb_button, NULL, LV_ALIGN_CENTER, 170, 10);
	lv_btn_set_checkable(usb_button, true);
	lv_obj_set_size(usb_button, 40, 20);
	lv_obj_t *label = lv_label_create(usb_button, NULL);
	lv_label_set_text(label, "Usb");
	lv_group_add_obj(vfo_group, usb_button);

	lsb_button = lv_btn_create(bg_top, NULL);
	lv_obj_set_event_cb(lsb_button, mode_button_eh);
	lv_obj_align(lsb_button, NULL, LV_ALIGN_CENTER, 120, 10);
	lv_btn_set_checkable(lsb_button, true);
	lv_btn_toggle(lsb_button);
	lv_obj_set_size(lsb_button, 40, 20);
	label = lv_label_create(lsb_button, NULL);
	lv_label_set_text(label, "Lsb");
	lv_group_add_obj(vfo_group, lsb_button);

	vfo1_button = lv_btn_create(bg_top, NULL);
	lv_obj_set_event_cb(vfo1_button, mode_button_vfo);
	lv_obj_align(vfo1_button, NULL, LV_ALIGN_CENTER, 65, 10);
	lv_btn_set_checkable(vfo1_button, true);
	lv_btn_toggle(vfo1_button);
	lv_obj_set_size(vfo1_button, 50, 20);
	label = lv_label_create(vfo1_button, NULL);
	lv_label_set_text(label, "Vfo 1");
	lv_group_add_obj(vfo_group, vfo1_button);
	
	vfo2_button = lv_btn_create(bg_top, NULL);
	lv_obj_set_event_cb(vfo2_button, mode_button_vfo);
	lv_obj_align(vfo2_button, NULL, LV_ALIGN_CENTER, 10, 10);
	lv_btn_set_checkable(vfo2_button, true);
	//lv_btn_toggle(vfo2_button);
	lv_obj_set_size(vfo2_button, 50, 20);
	label = lv_label_create(vfo2_button, NULL);
	lv_label_set_text(label, "Vfo 2");
	lv_group_add_obj(vfo_group, vfo2_button);

	lv_obj_t* save_button = lv_btn_create(bg_top, NULL);
	lv_obj_set_event_cb(save_button, event_button_save);
	lv_obj_align(save_button, NULL, LV_ALIGN_CENTER, -45, 10);
	lv_obj_set_size(save_button, 50, 20);
	label = lv_label_create(save_button, NULL);
	lv_label_set_text(label, "Save");
	lv_group_add_obj(vfo_group, save_button);

	static lv_style_t text_style;
	lv_style_init(&text_style);

	/*Set a background color and a radius*/
	lv_style_set_radius(&text_style, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_opa(&text_style, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&text_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_value_align(&text_style, LV_STATE_DEFAULT, LV_ALIGN_CENTER);
	lv_style_set_text_font(&text_style, LV_STATE_DEFAULT, &FreeSansOblique);

	vfo_frequency = lv_label_create(bg_middle, NULL);
	lv_label_set_long_mode(vfo_frequency, LV_LABEL_LONG_CROP);
	lv_obj_add_style(vfo_frequency, LV_OBJ_PART_MAIN, &text_style);
	lv_obj_set_width(vfo_frequency, LV_HOR_RES - 20);
	lv_label_set_text(vfo_frequency, "");
	lv_obj_set_height(vfo_frequency, 40);
	lv_obj_align(vfo_frequency, NULL, LV_ALIGN_CENTER, -30, -65);
	lv_label_set_align(vfo_frequency, LV_LABEL_ALIGN_CENTER);

	vfo_unit = lv_label_create(bg_middle, NULL);
	lv_obj_add_style(vfo_unit, LV_OBJ_PART_MAIN, &text_style);
	lv_label_set_text(vfo_unit, "Mhz");
	lv_obj_set_height(vfo_unit, 40);
	lv_obj_align(vfo_unit, NULL, LV_ALIGN_CENTER, 110, -68);
	lv_label_set_align(vfo_unit, LV_LABEL_ALIGN_CENTER);

	label_wifi = lv_label_create(bg_top, NULL);
	lv_label_set_long_mode(label_wifi, LV_LABEL_LONG_SROLL_CIRC);
	lv_obj_set_width(label_wifi, LV_HOR_RES - 20);
	lv_label_set_text(label_wifi, LV_SYMBOL_WIFI);
	lv_obj_align(label_wifi, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 5);
	lv_obj_set_hidden(label_wifi, true);

	char str[80];
	
	band_roller_str(str);
	/*Band_roller = lv_dropdown_create(bg_middle, NULL);
	lv_dropdown_set_options(Band_roller,str);
	lv_obj_align(Band_roller,NULL, LV_ALIGN_CENTER, 130, -20);
	lv_obj_set_width(Band_roller, 80);
	lv_obj_set_event_cb(Band_roller, band_event_handler);
	lv_group_add_obj(vfo_group, Band_roller);
	*/
	
	Band_roller = lv_roller_create(bg_middle, NULL);
	lv_roller_set_options(Band_roller,str, LV_ROLLER_MODE_INFINITE);
	lv_obj_align(Band_roller,NULL, LV_ALIGN_CENTER, 120, 10);
	lv_obj_set_width(Band_roller, 80);
	lv_obj_set_event_cb(Band_roller, band_event_handler);
	lv_roller_set_visible_row_count(Band_roller, 2);
	lv_group_add_obj(vfo_group, Band_roller);


	Tx_led1 = lv_led_create(bg_middle, NULL);
	lv_obj_align(Tx_led1, NULL, LV_ALIGN_CENTER, -125, -50);
	lv_led_off(Tx_led1);
	lv_obj_set_size(Tx_led1, 20, 20);
	
	lv_obj_t* Tx_ledlabel = lv_label_create(bg_middle, NULL);
	lv_obj_align(Tx_ledlabel, Tx_led1, LV_ALIGN_OUT_TOP_MID, 7, 0);
	lv_label_set_text(Tx_ledlabel, "TX");

	bfo_spinbox = lv_spinbox_create(bg_middle, NULL);
	lv_spinbox_set_range(bfo_spinbox, 8900, 9900);
	lv_spinbox_set_digit_format(bfo_spinbox, 4, 0);
	lv_spinbox_step_prev(bfo_spinbox);
	lv_obj_set_width(bfo_spinbox, 60);
	lv_obj_align(bfo_spinbox, NULL, LV_ALIGN_CENTER, -80, -20);
	lv_textarea_set_cursor_hidden(bfo_spinbox, true);

	lv_coord_t h = lv_obj_get_height(bfo_spinbox);
	lv_obj_t* btn = lv_btn_create(bg_middle, NULL);
	lv_obj_set_size(btn, h, h);
	lv_obj_align(btn, bfo_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_theme_apply(btn, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
	lv_obj_set_event_cb(btn, lv_spinbox_increment_event_cb);
	lv_group_add_obj(vfo_group, btn);
	
	// Keep de order of the focus correct
	lv_group_add_obj(vfo_group, bfo_spinbox);

	btn = lv_btn_create(bg_middle, btn);
	lv_obj_align(btn, bfo_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_event_cb(btn, lv_spinbox_decrement_event_cb);
	lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
	lv_group_add_obj(vfo_group, btn);

	gui_setup(scr);
	gui_si5351(scr);
	init_wifi_gui(scr);

	long frq = get_vfo_frequency(active_vfo);
	setfrequencylabel(frq);
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
		int count = Enc_vfo.getCount();
		if (count != 0)
		{
/*
* Translate the encoder movement to a new frequency
* If the band is changed also update the band roller
*/

			long frq = set_encoder_count_to_vfo_frequency(count, active_vfo);
			Enc_vfo.clearCount();
			setfrequencylabel(frq);
			uint16_t i = get_band();
			uint16_t ii = lv_roller_get_selected(Band_roller);
			if (i != ii)
				lv_roller_set_selected(Band_roller, i , LV_ANIM_ON);
		}
/*
* Process rotary press button messages
*/
		rotary_button.check();
		smeter->Draw(get_smeter());
		/*
		if (tt > 10)
		{
			int r = random(0,4096);
			smeter->Draw(r);
			tt = 0;
		}
		else
			tt++;
		*/
		vTaskDelay(5);
	}
}

static void rotary_button_eventhandler(AceButton*, uint8_t eventType, uint8_t buttonState)
{
	switch (eventType) {
	case AceButton::kEventLongPressed:
		ToggleSetup(true);
		break;
	case AceButton::kEventPressed:
		enc_button_state = LV_INDEV_STATE_PR;
		break;

	case AceButton::kEventReleased:
		enc_button_state = LV_INDEV_STATE_REL;
		break;
	}
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

		Serial.print("Data x");
		Serial.println(touchX);

		Serial.print("Data y");
		Serial.println(touchY);

	}

	return false; /*Return `false` because we are not buffering and no more data to read*/
}

static void mode_button_eh(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		//printf("Clicked\n");
	}
	else if (event == LV_EVENT_VALUE_CHANGED) {
		if (obj == usb_button)
		{
			lv_btn_toggle(lsb_button);
			switch_mode(MODE_USB, active_vfo);
		}
		if (obj == lsb_button)
		{
			lv_btn_toggle(usb_button);
			switch_mode(MODE_LSB, active_vfo);
		}
	}
}

static void mode_button_vfo(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		//printf("Clicked\n");
	}
	else if (event == LV_EVENT_VALUE_CHANGED) {
		if (obj == vfo1_button)
		{
			lv_btn_toggle(vfo2_button);
			active_vfo = 0;
		}
		if (obj == vfo2_button)
		{
			lv_btn_toggle(vfo1_button);
			active_vfo = 1;
		}
		long frq = switch_vfo(active_vfo);
		setfrequencylabel(frq);
		
	}
}

void updateBottomStatus(lv_color_t color, String text, uint8_t sem) {
	if (sem) xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	lv_obj_set_style_local_bg_color(bg_bottom, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, color);
	lv_label_set_text(label_status, text.c_str());
	if (sem) xSemaphoreGive(GuiBinarySemaphore);
}

void showWifilabel(bool show) {
	xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	lv_obj_set_hidden(label_wifi, show);
	xSemaphoreGive(GuiBinarySemaphore);
}


// this function is used to set the display frequency, it is not using semaphore because this is done from main loop
void setfrequencylabel(long freq, uint8_t sem) {
	if (sem) xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	char str[80];

	sprintf(str, "%3d.%03d,%02d", freq / 1000000, (freq / 1000) % 1000, (freq / 10) % 100);
	lv_label_set_text(vfo_frequency, str);
	if (sem) xSemaphoreGive(GuiBinarySemaphore);
}

static void band_event_handler(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_VALUE_CHANGED) {
		char	buf[80];
		long	frq;

		lv_roller_get_selected_str(obj, buf, sizeof(buf));
		buf[2] = '\0';
		frq = band_select(buf, active_vfo);
		setfrequencylabel(frq);
		set_vfo_frequency(frq, active_vfo);
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


static void lv_spinbox_increment_event_cb(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_increment(bfo_spinbox);
		lv_textarea_set_cursor_hidden(bfo_spinbox, false);
	}
}

static void lv_spinbox_decrement_event_cb(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_decrement(bfo_spinbox);
		lv_textarea_set_cursor_hidden(bfo_spinbox, false);
	}
}

static void event_button_save(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		init_vfo_save(active_vfo);
		SaveEEPROM();
	}
}

void ToggleSetup(bool show) {
	xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	if (show)
		lv_obj_move_foreground(bg_middle2);
	else
		lv_obj_move_background(bg_middle2);
	xSemaphoreGive(GuiBinarySemaphore);
}

void BfoLabel(uint32_t num, uint8_t sem) {
	if (sem)
		xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	lv_spinbox_set_value(bfo_spinbox, num);
	if (sem)
		xSemaphoreGive(GuiBinarySemaphore);
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
			lv_obj_move_foreground(bg_middle3);
			start_cal();
			break;
		case 2:
			
			break;
		case 3:
			memset(&R, 0, sizeof(R));
			SaveEEPROM();
			break;
		}
		
	}
}

static void gui_setup(lv_obj_t *scr)
{
	static const char* setup_btnm_map[] = { "Wifi", "Si5351", "Calibrate\n AD8307", "Reset", "" };

	bg_middle2 = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_middle2, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_middle2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_middle2, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(bg_middle2, 0, topHeight);
	lv_obj_set_size(bg_middle2, LV_HOR_RES, screenHeight - topHeight - bottomHeight);
	lv_obj_move_background(bg_middle2);

	lv_obj_t* btnm1 = lv_btnmatrix_create(bg_middle2, NULL);
	lv_btnmatrix_set_map(btnm1, setup_btnm_map);
	lv_obj_align(btnm1, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_btnmatrix_set_btn_ctrl(btnm1, 6, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_obj_set_event_cb(btnm1, gui_setup_event_handler);

	lv_obj_t* pwd_label1 = lv_label_create(bg_middle2, NULL);
	lv_label_set_text(pwd_label1, "Setup");
	lv_obj_align(pwd_label1, btnm1, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

}

static void lv_spinbox_increment_event_si(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_increment(si_spinbox);
		R.correction_si5351_no1 = lv_spinbox_get_value(si_spinbox);
		cal_vfo();
	}
}

static void lv_spinbox_decrement_event_si(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_decrement(si_spinbox);
		R.correction_si5351_no1 = lv_spinbox_get_value(si_spinbox);
		cal_vfo();
	}
}

static void lv_spinbox_increment_event_si2(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_increment(si_spinbox2);
		R.correction_si5351_no2 = lv_spinbox_get_value(si_spinbox);
		cal_bfo();
	}
}

static void lv_spinbox_decrement_event_si2(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_spinbox_decrement(si_spinbox2);
		R.correction_si5351_no2 = lv_spinbox_get_value(si_spinbox);
		cal_bfo();
	}
}

static void lv_spinbox_save_event_si(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		SaveEEPROM();
		updateBottomStatus(LV_COLOR_GREEN, "Si5351 calibration saved", 0);
		lv_obj_move_foreground(bg_middle);
		lv_obj_move_background(bg_middle3);
	}
}

static void lv_spinbox_cancel_event_si(lv_obj_t* btn, lv_event_t e)
{
	if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT) {
		lv_obj_move_foreground(bg_middle);
		lv_obj_move_background(bg_middle3);
	}
}

static void gui_si5351(lv_obj_t* scr)
{
	bg_middle3 = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_middle3, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_middle3, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_middle3, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(bg_middle3, 0, topHeight);
	lv_obj_set_size(bg_middle3, LV_HOR_RES, screenHeight - topHeight - bottomHeight);
	lv_obj_move_background(bg_middle3);

	si_spinbox = lv_spinbox_create(bg_middle3, NULL);
	lv_spinbox_set_range(si_spinbox, -150000, 150000);
	lv_spinbox_set_digit_format(si_spinbox, 7, 0);
	lv_spinbox_step_prev(si_spinbox);
	lv_obj_set_width(si_spinbox, 120);
	lv_obj_align(si_spinbox, NULL, LV_ALIGN_CENTER, 0, -50);
	lv_spinbox_set_value(si_spinbox, 0);

	lv_obj_t* pwd_label1 = lv_label_create(bg_middle3, NULL);
	lv_label_set_text(pwd_label1, "Vfo Si5351");
	lv_obj_align(pwd_label1, si_spinbox, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

	lv_coord_t h = lv_obj_get_height(si_spinbox);
	lv_obj_t* btn = lv_btn_create(bg_middle3, NULL);
	lv_obj_set_size(btn, h, h);
	lv_obj_align(btn, si_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_theme_apply(btn, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
	lv_obj_set_event_cb(btn, lv_spinbox_increment_event_si);

	btn = lv_btn_create(bg_middle3, btn);
	lv_obj_align(btn, si_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_event_cb(btn, lv_spinbox_decrement_event_si);
	lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

	si_spinbox2 = lv_spinbox_create(bg_middle3, NULL);
	lv_spinbox_set_range(si_spinbox2, -150000, 150000);
	lv_spinbox_set_digit_format(si_spinbox2, 7, 0);
	lv_spinbox_step_prev(si_spinbox2);
	lv_obj_set_width(si_spinbox2, 120);
	lv_obj_align(si_spinbox2, NULL, LV_ALIGN_CENTER, 0, +20);
	lv_spinbox_set_value(si_spinbox2, 0);

	pwd_label1 = lv_label_create(bg_middle3, NULL);
	lv_label_set_text(pwd_label1, "Bfo Si5351");
	lv_obj_align(pwd_label1, si_spinbox2, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

	h = lv_obj_get_height(si_spinbox2);
	btn = lv_btn_create(bg_middle3, NULL);
	lv_obj_set_size(btn, h, h);
	lv_obj_align(btn, si_spinbox2, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_theme_apply(btn, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
	lv_obj_set_event_cb(btn, lv_spinbox_increment_event_si2);

	btn = lv_btn_create(bg_middle3, btn);
	lv_obj_align(btn, si_spinbox2, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_event_cb(btn, lv_spinbox_decrement_event_si2);
	lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);


	btn = lv_btn_create(bg_middle3, NULL);
	lv_obj_align(btn, NULL, LV_ALIGN_CENTER, -70, 70);
	lv_theme_apply(btn, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Save");
	lv_obj_set_event_cb(btn, lv_spinbox_save_event_si);

	btn = lv_btn_create(bg_middle3, NULL);
	lv_obj_align(btn, NULL, LV_ALIGN_CENTER, 70, 70);
	lv_theme_apply(btn, LV_THEME_SPINBOX_BTN);
	lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Cancel");
	lv_obj_set_event_cb(btn, lv_spinbox_cancel_event_si);
}