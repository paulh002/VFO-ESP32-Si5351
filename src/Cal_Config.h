/*
 * 	"Cal_Config.h" defines some symbols needed by the rest of the program.
 */

#ifndef _CONFIG_H_			// Prevent double inclusion
#define _CONFIG_H_

/*
 *	You might need to change some of the following items.
 *
 *	Although I haven't seen one that does, the Si5351 documentation indicates
 *	that it can be clocked by a 27MHz rather than the more usual 25MHz crystal.
 *	If you happen to have a clock source other than 25MHz, change the following
 *	definition accordingly
 */

#define	SI_XTAL		25000000UL				// Could also be 27000000UL
#define CLK_BFO_RX           SI5351_CLK2
#define CLK_VFO_RX           SI5351_CLK2
#define CLK_BFO_TX           SI5351_CLK1
#define CLK_VFO_TX           SI5351_CLK1
#define CLK_NA               SI5351_CLK0

/*
 *	This defines the default test frequency. If someone simply hits 'Enter' when
 *	asked for the frequency at which to test, this value will be used.
 */

#define	TEST_FREQ	10000000UL			// 10MHz

/*
 *	This is the range limit for the correction factor. It is currently set to
 *	+/- 1,000,000. I don't know for sure what the mazimum range should be, but one
 *	unit that we've seen required a correction factor in the 204K range.
 */

#define CAL_LIMIT 1000000L					// 1MHz for now


/*
 *	If you mess with any of the following, bad things might happen! These are not
 *	used in the calibration program, but are referenced in the Si5351 code.
 */

#define CAL_SET0_MENU 0
#define CAL_SET1_MENU 1
#define CAL_SET2_MENU 2

bool  GetNewFreq (void);
void  SaveCorrection ( );
void  GetTestFreq ();
void  ShowHelp ();
void  LoadCorrection ();
void  loop_si5351 ( uint8_t ui_vfo_bfo );
void FmtFreq ( int32_t freq, char* buff, bool addHz );
void ad8307_calibration(int16_t menu_level);


extern  Si5351 si5351;
extern  Si5351 si5351_bfo;

#endif
