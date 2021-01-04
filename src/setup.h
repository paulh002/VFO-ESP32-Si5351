
#ifndef _SETUP_H_
#define _SETUP_H_

#define COLDSTART_REF          0x07 // When started, the firmware examines this "Serial Number
                                    // and enforces factory reset if there is a mismatch.
                                    // Rolling this value is useful if the EEPROM structure has been modified


extern var_t  R;
extern uint8_t c_button, f_button, c_mode, c_rxtx, c_band, c_mhz;
extern uint8_t f_dchange, f_bchange;
extern uint8_t setup_menu_item, setup_select;


void setup_menu(void);
void setup_display(void);
void calibrate_vfo_si5351(void);
void calibrate_bfo_si5351(void);
void calibrate_ad8307(void);
void LoadEEPROM (void);
void SaveEEPROM ();
void otawifi();
#endif
