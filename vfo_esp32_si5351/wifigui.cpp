// 
// 
// 
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include <ESP32Encoder.h>
#include <AceButton.h>
using namespace ace_button;

#include "setup.h"
#include "gui.h"
#include "network.h"
#include "wifigui.h"

lv_obj_t* bg_wifigui;
lv_obj_t* wifi_scan_list;
lv_obj_t* ta_password;
lv_obj_t* kb;
lv_obj_t* btnm1;
lv_obj_t* pwd_label1;
lv_obj_t* pwd_label;
lv_obj_t* cancel_btn;
lv_obj_t* wifi_button;
lv_obj_t* bg_wifiota;
lv_obj_t* ota_label;
lv_obj_t* ota_label1;

TaskHandle_t hWifiTask;
String	ssidName, password;

void wifi_scan_network(void* pvParameters);

static void wifilist_event_handler(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		Save_wifi((char *)lv_list_get_btn_text(obj));
	}
}

static void close_event_handler(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		show_wifi_gui(false);
	}
}

static void ota_event_handler(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		show_ota_gui(false);
	}
}

static void makeKeyboard() {
	kb = lv_keyboard_create(lv_scr_act(), NULL);
	lv_obj_set_size(kb, LV_HOR_RES, LV_VER_RES / 2);
	lv_keyboard_set_cursor_manage(kb, true);

	lv_keyboard_set_textarea(kb, ta_password);
	lv_obj_set_event_cb(kb, keyboard_event_cb);
	lv_obj_move_background(kb);
}

static void makePW(void)
{
	/* Create the password box */
	ta_password = lv_textarea_create(bg_wifigui, NULL);
	lv_textarea_set_text(ta_password, "");
	lv_textarea_set_pwd_mode(ta_password, true);
	lv_textarea_set_one_line(ta_password, true);
	lv_textarea_set_cursor_hidden(ta_password, true);
	lv_obj_set_width(ta_password, LV_HOR_RES / 2 - 20);
	lv_obj_set_pos(ta_password, 5, 40);
	//	lv_obj_set_event_cb(pwd_ta, ta_event_cb);

		/* Create a label and position it above the text box */
	pwd_label = lv_label_create(bg_wifigui, NULL);
	lv_label_set_text(pwd_label, "Password:");
	lv_obj_align(pwd_label, ta_password, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

	lv_obj_set_hidden(ta_password, true);
	lv_obj_set_hidden(pwd_label, true);
}

static void keyboard_event_cb(lv_obj_t* kb, lv_event_t event) {
	lv_keyboard_def_event_cb(kb, event);

	if (event == LV_EVENT_APPLY) {
		{
			lv_obj_move_background(kb);
			password = String(lv_textarea_get_text(ta_password));
			lv_obj_set_hidden(ta_password, true);
			lv_obj_set_hidden(pwd_label, true);
			lv_obj_set_hidden(btnm1, false);
			lv_obj_set_hidden(pwd_label1, false);
		}

	}
	else if (event == LV_EVENT_CANCEL) {
		lv_obj_move_background(kb);
	}
}


static void wifi_button_eh(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		//printf("Clicked\n");
	}
	else if (event == LV_EVENT_VALUE_CHANGED) {
		lv_btn_state_t state = lv_btn_get_state(obj);
		if (state == LV_STATE_CHECKED || state == LV_BTN_STATE_CHECKED_RELEASED)
		{
			uint8_t	message = 1;

			R.wifi_onoff = 1;
			xQueueSend(net_queue, &message, 0);
		}
		else
		{
			uint8_t	message = 0;

			R.wifi_onoff = 0;
			xQueueSend(net_queue, &message, 0);
		}
	}
}

void init_wifi_gui(lv_obj_t* scr)
{
	bg_wifigui = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_wifigui, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_wifigui, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_wifigui, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(bg_wifigui, 0, topHeight);
	lv_obj_set_size(bg_wifigui, LV_HOR_RES, screenHeight - topHeight - bottomHeight);
	lv_obj_move_background(bg_wifigui);
	lv_coord_t h = lv_obj_get_height(bg_wifigui);
	lv_coord_t w = lv_obj_get_width(bg_wifigui);

	wifi_scan_list = lv_list_create(bg_wifigui, NULL);
	lv_obj_set_size(wifi_scan_list, w, h - 60);
	lv_obj_align(wifi_scan_list, NULL, LV_ALIGN_CENTER, 0, -20);

	cancel_btn = lv_btn_create(bg_wifigui, NULL);
	lv_obj_align(cancel_btn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
	lv_obj_set_event_cb(cancel_btn, close_event_handler);
	lv_obj_set_style_local_value_str(cancel_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Cancel");
	
	wifi_button = lv_btn_create(bg_bottom, NULL);
	lv_obj_set_event_cb(wifi_button, wifi_button_eh);
	lv_obj_align(wifi_button, NULL, LV_ALIGN_CENTER, 170, 10);
	lv_btn_set_checkable(wifi_button, true);
	lv_btn_toggle(wifi_button);
	lv_obj_set_size(wifi_button, 40, 20);
	lv_obj_t* label = lv_label_create(wifi_button, NULL);
	lv_label_set_text(label, "Wifi");

	makeKeyboard();
	makePW();
	lv_ex_btnmatrix_1();

	bg_wifiota = lv_obj_create(scr, NULL);
	lv_obj_clean_style_list(bg_wifiota, LV_OBJ_PART_MAIN);
	lv_obj_set_style_local_bg_opa(bg_wifiota, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(bg_wifiota, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(bg_wifiota, 0, topHeight);
	lv_obj_set_size(bg_wifiota, LV_HOR_RES, screenHeight - topHeight - bottomHeight);
	lv_obj_move_background(bg_wifigui);

	static lv_style_t text_style;
	lv_style_init(&text_style);

	/*Set a background color and a radius*/
	lv_style_set_radius(&text_style, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_opa(&text_style, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&text_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_value_align(&text_style, LV_STATE_DEFAULT, LV_ALIGN_CENTER);
	lv_style_set_text_font(&text_style, LV_STATE_DEFAULT, &lv_font_montserrat_18);

	ota_label = lv_label_create(bg_wifiota, NULL);
	lv_label_set_text(ota_label, "");
	lv_obj_add_style(ota_label, LV_OBJ_PART_MAIN, &text_style);
	lv_obj_align(ota_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_move_background(bg_wifiota);

	ota_label1 = lv_label_create(bg_wifiota, NULL);
	lv_obj_add_style(ota_label1, LV_OBJ_PART_MAIN, &text_style);
	lv_label_set_text(ota_label1, "");
	lv_obj_align(ota_label1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 30);
	lv_obj_move_background(bg_wifiota);

	//ota_cancel_btn = lv_btn_create(bg_wifiota, NULL);
	//lv_obj_align(ota_cancel_btn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
	//lv_obj_set_event_cb(ota_cancel_btn, ota_event_handler);
	//lv_obj_set_style_local_value_str(ota_cancel_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Cancel");
}

void show_wifi_gui(int show)
{
	if (show)
	{
		lv_obj_move_foreground(bg_wifigui);
		lv_obj_set_hidden(wifi_scan_list, false);
		lv_obj_set_hidden(cancel_btn, false);
		xTaskCreate(wifi_scan_network,"wifi_scan",4096,NULL,1,&hWifiTask); 
	}
	else
	{
		vTaskDelete(hWifiTask);
		lv_obj_move_background(bg_wifigui);
		lv_obj_move_foreground(bg_middle);
	}
}

void wifi_scan_network(void* pvParameters)
{
	vTaskDelay(1000); 
	while (1)
	{
		updateBottomStatus(LV_COLOR_ORANGE, "Searching Available WIFI ...");
		int n = WiFi.scanNetworks();
		if (n <= 0) {
			updateBottomStatus(LV_COLOR_RED, "Sorry no networks found!");
			vTaskDelay(5);
		}
		else
		{
			xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
			lv_list_clean(wifi_scan_list);
			xSemaphoreGive(GuiBinarySemaphore);
			vTaskDelay(10);
			for (int i = 0; i < n; ++i) 
			{
				String item = WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ") ";
				switch (WiFi.encryptionType(i))
				{
				case WIFI_AUTH_OPEN:
					break;
				case WIFI_AUTH_WEP:
					item = item + String(" WEP");
					break;
				case WIFI_AUTH_WPA_WPA2_PSK:
				case WIFI_AUTH_WPA_PSK: 
					item = item + String(" WPA");
					break;
				case WIFI_AUTH_WPA2_PSK:
				case WIFI_AUTH_WPA2_ENTERPRISE:
					item = item + String(" WPA2");
					break;
				default:
					item = item + String(" ***");
					break;
				}				
				xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
				lv_obj_t* list_btn = lv_list_add_btn(wifi_scan_list, LV_SYMBOL_WIFI, item.c_str());
				lv_obj_set_event_cb(list_btn, wifilist_event_handler);
				xSemaphoreGive(GuiBinarySemaphore);
				vTaskDelay(10);
			}
			updateBottomStatus(LV_COLOR_GREEN, String(n) + " networks found!");
		}
		vTaskDelay(30000);
	}
}

static void popupPW(String ssidName) {
	if (ssidName == NULL || ssidName.length() == 0) {
		return;
	}
	char str[80];

	lv_textarea_set_text(ta_password, "");
	// lv_msgbox_set_text(mbox_connect, ssidName.c_str());
	strcpy(str, "Password for ");
	strcat(str, ssidName.c_str());
	lv_label_set_text(pwd_label, str);
	lv_obj_set_hidden(ta_password, false);
	lv_obj_set_hidden(pwd_label, false);

	lv_obj_move_foreground(kb);
	lv_keyboard_set_textarea(kb, ta_password);
}

static void Save_wifi(char* buf)
{
	char *ptr = strchr(buf, ' ');
	if (ptr) *ptr = '\0';
	vTaskDelete(hWifiTask);
	lv_obj_set_hidden(wifi_scan_list, true);
	lv_obj_set_hidden(cancel_btn, true);
	ssidName = String(buf);
	popupPW(String(buf));
}

static void event_handler(lv_obj_t* obj, lv_event_t event)
{
	if (event == LV_EVENT_VALUE_CHANGED) {
		const char* txt = lv_btnmatrix_get_active_btn_text(obj);

		uint8_t i = atoi(txt) - 1;
		if (i > 5) i = 0;

		ssidName.toCharArray(R.ssid[i], 30);
		password.toCharArray(R.password[i], 60);
		lv_obj_set_hidden(btnm1, true);
		lv_obj_set_hidden(pwd_label1, true);
		
		lv_obj_move_background(bg_wifigui);
		lv_obj_move_foreground(bg_middle);
		
		// Start Wifi Network
		uint8_t	message = 1;

		R.wifi_onoff = 1;
		xQueueSend(net_queue, &message, 0);
	}
}

static const char* btnm_map[] = { "1", "2", "3", "4", "5", "" };

static void lv_ex_btnmatrix_1(void)
{
	btnm1 = lv_btnmatrix_create(bg_wifigui, NULL);
	lv_btnmatrix_set_map(btnm1, btnm_map);
	lv_obj_align(btnm1, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_btnmatrix_set_btn_ctrl(btnm1, 6, LV_BTNMATRIX_CTRL_CHECKABLE);
	lv_obj_set_event_cb(btnm1, event_handler);

	pwd_label1 = lv_label_create(bg_wifigui, NULL);
	lv_label_set_text(pwd_label1, "ssid slot");
	lv_obj_align(pwd_label1, btnm1, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

	lv_obj_set_hidden(btnm1, true);
	lv_obj_set_hidden(pwd_label1, true);
}

void show_ota_gui(int show, uint8_t sem)
{
	if (sem)
		xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY); 
	if (show)
	{
		lv_obj_move_foreground(bg_wifiota);
		//xTaskCreate(wifi_scan_network, "wifi_scan", 4096, NULL, 1, &hWifiTask);
	}
	else
	{
		//vTaskDelete(hWifiTask);
		lv_obj_move_background(bg_wifiota);
		lv_obj_move_foreground(bg_middle);
	}
	if (sem)
		xSemaphoreGive(GuiBinarySemaphore);
}

void set_ota_label(String str, uint8_t sem)
{
	if (sem)
		xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	lv_label_set_text(ota_label, str.c_str()); 
	if (sem)
		xSemaphoreGive(GuiBinarySemaphore);
}

void set_ota_label1(String str, uint8_t sem)
{
	if (sem)
		xSemaphoreTake(GuiBinarySemaphore, portMAX_DELAY);
	lv_label_set_text(ota_label1, str.c_str());
	if (sem)
		xSemaphoreGive(GuiBinarySemaphore);
}