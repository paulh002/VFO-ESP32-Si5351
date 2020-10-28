/*
 *  The source code is original created by W2FZW and adapted for use in VFO of PA0PHH
 *
 *    Written: John M. Price (WA2FZW) 08/03/2019
 *
 *    This program is designed to provide a way of calibrating the crystal frequency
 *    for an Adafruit Si5351 clock generator module. Nominally, the crystal frequency
 *    is either 25MHz or 27MHz, but rarely is it exactly one of those frequencies.
 *
 *    The program was originally written as a support program for another project,
 *    but as it is generally useful for other projects, I generalized it. The original
 *    program was written specifically to run on the ESP32. This version will also
 *    work on an Arduino. The primary difference is how the EEPROM is handled as 
 *    different versions of the EEPROM library are used.
 *
 *    In the "Cal_Config.h" file, you must set the symbol "USE_EEPROM" to one of these
 *    three values:
 *
 *      DONT_USE
 *      ESP32
 *      ARDNO
 *
 *    If the "USE_EEPROM" symbol is set to either "ESP32" or "ARDNO", the correction
 *    factor can be saved as a long integer (int32_t) in the first 4 bytes of the
 *    EEPROM for use by other programs. Although the correction only needs to be a
 *    normal integer (int16_t), the original program was saving a frequency (uint32_t)
 *    in the loccation, so we stuck with the 4 byte length.
 *
 */

#include <arduino.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_ILI9341.h>
#include "i2c.h"
#include "si5351.h"

#include "measurement.h"
#include "ringmeter.h"
#include "setup.h"
#include "Cal_config.h"

#include "Fonts/DroidSansMono8pt7b.h"
#include "Fonts/DroidSansMono12pt7b.h"
#include "Fonts/DroidSansMono24pt7b.h"
#include "Fonts/DroidSansMono48pt7b.h"

extern  Adafruit_ILI9341 tft;

int       corrAddr   = 0;           // EEPROM address of correction factor
int32_t   correction = 0;           // Correction amount

uint32_t  readFreq   = 0;           // Correction factor read from EEPROM

uint32_t  testFreq   = TEST_FREQ;       // Default test frequency (10MHz)
char      freqString[25];           // Used to format frequency output strings

typedef struct                  // These are set by "GetNewFreq()"
{
  uint32_t  value;              // Binary frequency
  char    sign;             // '+', '-', '=', 'Q', 'S' or 'N'
} new_freq;

new_freq    newFreq;            // Create the structure




void loop_si5351 ( uint8_t ui_vfo_bfo )
{
uint32_t	oldCorrection;							// Saved value if illegal change


/*
 *	Instruct the user to either accept the current correction factor by hitting 'Enter' or
 *	to input a new correction factor (or adjustment).
 *
 *	Again, all entries are in Hertz. Here the user can enter a completely new correction
 *	factor by entering the number (except '0'), or the number preceeded by an equal sign
 *	('='). Again, commas can be used for clarity (they are ignored).
 *
 *	To reset the correction factor to zero, one has to use the 'R' (or 'r') command.
 *
 *	If the number is preceeded with a plus ('+') or minus ('-') sign, it is interpreted
 *	as an adjustment to the current correction factor.
 *
 *	Entering 'Q' or 'q' will cause the current correction factor to be stored in the
 *	EEPROM and the program will terminate (actually just go into a never ending loop).
 *
 *	Entering 'S' or 's' will cause the current correction factor to be stored in the
 *	EEPROM and the program will ask for a new frequency at which to perform the 
 *	calibration. This makes it easier for one to test at several frequencies without
 *	having to press the reboot button!
 *
 *	Entering 'N' or 'n' causes program to ask for a new frequency at which to perform
 *	the calibration without saving the correction factor. This makes it easier for one
 *	to test at several frequencies without having to press the reboot button!
 */

	while(1)
  {
  // Save current correction factor
  if (ui_vfo_bfo = 0)
    oldCorrection = R.correction_si5351_no1;
  else
    oldCorrection = R.correction_si5351_no2 ;


	Serial.println ( "\nEnter a new correction factor or offset or" );
	Serial.println ( "hit 'Enter' to use the current correction factor" );
  ShowHelp ();

	while ( ! (int32_t) GetNewFreq() ) {}				// Wait for an answer

	switch ( newFreq.sign )
	{
		case '+':										// Increment old frequency?
			if (ui_vfo_bfo = 0)
			  R.correction_si5351_no1 += (int32_t) newFreq.value;		// Add new value
			else
			  R.correction_si5351_no2 += (int32_t) newFreq.value;    // Add new value
			break;

 		case '-':										// Decrement old frequency?
 	    if (ui_vfo_bfo = 0)
        R.correction_si5351_no1 -= (int32_t) newFreq.value;   // Add new value
      else
        R.correction_si5351_no2 -= (int32_t) newFreq.value;    // Add new value
  		break;

		case '=':										// Completly new value?
			if ( newFreq.value != 0 )					// '0' means don't change the frequency
 				{
 				  if (ui_vfo_bfo = 0)
     				R.correction_si5351_no1 = (int32_t) newFreq.value;   // Add new value
          else
            R.correction_si5351_no2 = (int32_t) newFreq.value;    // Add new value
 				}
			break;

		case 'R':										// Reset to zero?
       if (ui_vfo_bfo = 0)
          R.correction_si5351_no1 = 0;   // Add new value
        else
          R.correction_si5351_no2 = 0;    // Add new value
			break;

		case 'S':										// Save and restart
			SaveCorrection (  );				// Save correction factor
														// Drops through to "case 'N'"

		case 'N':										// Get new test frequency
			GetTestFreq ();								// Get the frequency
			ShowHelp ();								// Display instructions again
			break;

		case 'Q':										// Save and quit?
			SaveCorrection ();				// Save correction factor
			Serial.println ( "Execution terminated! ");
			return;
	}


/*
 *	Range check the new correction factor:
 */

	if (ui_vfo_bfo = 0)
  {
    	if (( R.correction_si5351_no1 < -CAL_LIMIT ) || ( R.correction_si5351_no1 > CAL_LIMIT ))	// Invalid correction factor?
    	{
    		Serial.print ( "Correction factor: " );
    		FmtFreq ( R.correction_si5351_no1, freqString, false );
    		Serial.println ( freqString );
    		Serial.println ( "Is out of the legal range; not changed." );
    
    		R.correction_si5351_no1 = oldCorrection;						// Restore old value
    	}
  }
  else
  {
      if (( R.correction_si5351_no2 < -CAL_LIMIT ) || ( R.correction_si5351_no2 > CAL_LIMIT ))  // Invalid correction factor?
      {
        Serial.print ( "Correction factor: " );
        FmtFreq ( R.correction_si5351_no2, freqString, false );
        Serial.println ( freqString );
        Serial.println ( "Is out of the legal range; not changed." );
    
        R.correction_si5351_no2 = oldCorrection;            // Restore old value
      }    
  }
	Serial.print ( "\nCorrection factor set to: " );
  if (ui_vfo_bfo = 0)
  {
    si5351.set_correction(R.correction_si5351_no1, SI5351_PLL_INPUT_XO);
    FmtFreq ( R.correction_si5351_no1, freqString, false );
    
    si5351.set_freq(testFreq * SI5351_FREQ_MULT, CLK_VFO_RX);
    si5351.set_freq(testFreq * SI5351_FREQ_MULT, CLK_VFO_TX);
  }
  else
  {
    si5351_bfo.set_correction(R.correction_si5351_no2, SI5351_PLL_INPUT_XO);    
    FmtFreq ( R.correction_si5351_no2, freqString, false );
    
    si5351_bfo.set_freq(testFreq * SI5351_FREQ_MULT, CLK_BFO_RX);
    si5351_bfo.set_freq(testFreq * SI5351_FREQ_MULT, CLK_BFO_TX);
  }
	Serial.println ( freqString );

	delay ( 1000 );
  }
}


/*
 *	GetNewFreq() reads both the calibration test frequency and the correction factor.
 *
 *	The numeric frequency can be preceeded by:
 *
 *		'+'				Add the value to the existing frequency
 *		'-'				Subtract the value to the existing frequency
 *		'=' or no sign	This is an absolute value
 *
 *	If the first character is 'q' or 'Q', we set the frequency in the EEPROM and we're done.
 *
 *	If the first character is 'r' or'R' we reset the value to zero.
 *
 *	If the first character is 's' or'S' we save the correction factor and ask for a new
 *	test frequency.
 *
 *	If the first character is 'n' or'N' we ask for a new test frequency without saving
 *	the correction factor.
 *
 *	The frequency is assumed to be in Hertz and may optionally contain commas
 *	for clarity.
 *
 *	It returns 'true' if a new value was received' 'false' if not.
 */
 
bool GetNewFreq ()
{
char 		sign;									// Sign in front of the string
char		nextChar;								// Next character from the input
uint32_t	number;									// Binary value of frequency
int			count = 0;								// Characters read from input


	if ( !Serial.available () )						// If nothing to read
		return false;								// Return 'false'

	memset ( freqString, 0, sizeof ( freqString ));	// Clear buffer

	sign   = '=';									// Assume absolute number
	number = 0;										// Set value to '0'
	count  = 0;										// Clear character count

	Serial.readBytesUntil ( '\n', freqString, sizeof ( freqString ));

	while ( count < strlen ( freqString ))					// While data to read
	{
		nextChar = toupper ( freqString[count] );			// Read a character

		if ( count == 0 )									// Sign or command can only be 1st character
			if (( nextChar == '+' ) || ( nextChar == '-' )	// Sign character?
								    || ( nextChar == '=' )
								    || ( nextChar == 'Q' )	// Quit character?
								    || ( nextChar == 'S' )	// Save character?
								    || ( nextChar == 'N' )	// New test frequency  character?
								    || ( nextChar == 'R' ))	// Reset character?
		{
				sign = nextChar;							// Set the sign
				count++;									// Bump the counter
				continue;
		}

		if (( nextChar >= '0' ) && ( nextChar <= '9' ))		// A digit?
		{
			number = ( number * 10 ) + ( nextChar - '0' );	// Add to the number
			count++;										// Bump the counter

//			Serial.print ( "Number: " );	Serial.println ( number );
//			Serial.print ( "Count:  " );	Serial.println ( count );

			continue;
		}

		else								// Ignore anything else
		{
			count++;
			continue;						// We're finished reading
		}
	}

	newFreq.value = number;					// Set frequency in global structure
	newFreq.sign  = sign;					// And the sign

	return true;
}


/*
 *	"FmtFreq" formats frequencies nicely with commas inserted in the appropriate
 *	places.
 */

void FmtFreq ( int32_t freq, char* buff, bool addHz )
{
	int	KHz;						// Low order 3 digits
	int	MHz2;						// Middle 3 digits
	int	MHz1;						// High order digits
	int	sign = 1;					// Assume a positive number

	if ( freq < 0 )					// Wrong assumption?
	{
		sign = -1;					// Yep!
		freq = abs ( freq );		// Positive now!
	}

	KHz   = freq % 1000;
	freq  = freq / 1000;

	MHz2  = freq % 1000;
	freq  = freq / 1000;

	MHz1  = freq;

	if ( MHz1 != 0 )
	{
		MHz1 *= sign;						// Make negative maybe
		sprintf ( buff, "%2d,%03d,%03d", MHz1, MHz2, KHz );
		if ( addHz )
			strcat ( buff, " Hz" );
		return;
	}
	
	else if ( MHz2 != 0 )
	{
		MHz2 *= sign;						// Make negative maybe
		sprintf ( buff, "%d,%03d", MHz2, KHz );
		if ( addHz )
			strcat ( buff, " Hz" );
		return;
	}

	else
	{
		KHz *= sign;
		sprintf ( buff, "%3d", KHz );
		if ( addHz )
			strcat ( buff, " Hz" );
		return;
	}
}


/*
 *	"GetTestFreq" asks or the frequency at which the user wants to perform the
 *	calibration. Any frequency may be used if one has an accurate frequency counter.
 *	If no counter is available, then the output of the Si5351 can be zero beat
 *	against a known accurate signal such as WWV.
 *
 *	The value is entered in Hertz. Commas may be used for clarity, for example:
 *	'10,000,000'.
 */

void GetTestFreq ()
{
	Serial.println ( "\nEnter calibration test frequency in Hertz" );
	while ( !GetNewFreq() ) {}								// Wait for an answer

	if ( newFreq.value == 0 )								// User didn't enter anything
	{
		Serial.println ( "Using default test frequency." );
		testFreq = TEST_FREQ;								// 10 MHz
	}

	else
		testFreq = newFreq.value;							// Set value entered

	FmtFreq ( testFreq, freqString, true );
	Serial.print ( "Calibration will be performed at: " );
	Serial.println ( freqString );
}


/*
 *	"ShowHelp" displays the instructions. Note if the "USE_EEPROM" value is set to
 *	"DONT_USE", the 'S' option is not available.
 */

void ShowHelp ()
{
	Serial.println ( "\nWhen asked to enter a new correction factor, you may enter:" );
	Serial.println ( "\n    nnnnn (or =nnnnn) to set a new value" );
	Serial.println ( "    +nnnn to add an offset to the current value" );
	Serial.println ( "    -nnnn to subtract an offset from the current value" );
	Serial.println ( "    The enter key to use the current value" );
	Serial.println ( "    or 'R' or 'r' to reset the correction factor to zero" );
	Serial.println ( "    or 'N' or 'n' to get a new test frequency without saving" );
	Serial.println ( "    or 'Q' or 'q' to quit and save the value in the EEPROM" );
}


/*
 *	"SaveCorrection" saves the specified correction factor in the EEPROM if the
 *	"USE_EEPROM" symbol is set to either "ESP32" or "ARDNO", but note, how it
 *	is written to the EEPROM is different.
 */

void SaveCorrection ( int32_t corr )
{
	Serial.println ( "\nCorrection factor is saved in the EEPROM" );
  SaveEEPROM ();
	delay ( 1000 );
}

/*
 *	"LoadCorrection" retrieves the current correction factor in the EEPROM if the
 *	"USE_EEPROM" symbol is set to either "ESP32" or "ARDNO", but note, how it
 *	is read from the EEPROM is different.
 */

void LoadCorrection ()
{
/*
 *	Verify that what we read is valid. I'm not sure what the actual legitimate range
 *	is for the correction factor, so for the time being, the value of "CAL_LIMIT" is
 *	set to 1,000,000 in the "Cal_Config.h" file. One Si5351 we tested with required
 *	a correction factor in the -204K range; most were about -40K.
 *
 *	'-1' is a special case. On all the ESP32s we tested on, the EEPROM contents were
 *	all ones. If that is the case, the correction factor will be read as '-1', which
 *	most likely isn't a valid answer.
 */

	if (( R.correction_si5351_no1 < -CAL_LIMIT ) || ( R.correction_si5351_no1 > CAL_LIMIT )
								|| ( R.correction_si5351_no1 == -1 ))	// Invalid correction factor?
	{
		R.correction_si5351_no1 = 0;										// Set default

		Serial.println ( "\nEEPROM does not contain a valid correction factor." );
		Serial.print   ( "Correction factor set to zero! " );
	}

	else
	{
		Serial.print ( "\nEEPROM contains correction factor: " );
		FmtFreq ( R.correction_si5351_no1, freqString, false );
		Serial.println ( freqString );
		Serial.println ( "We will start with that value." );
	}
  
  if (( R.correction_si5351_no2 < -CAL_LIMIT ) || ( R.correction_si5351_no2 > CAL_LIMIT )
                || ( R.correction_si5351_no2 == -1 ))  // Invalid correction factor?
  {
    R.correction_si5351_no2 = 0;                    // Set default

    Serial.println ( "\nEEPROM does not contain a valid correction factor." );
    Serial.print   ( "Correction factor set to zero! " );
  }

  else
  {
    Serial.print ( "\nEEPROM contains correction factor: " );
    FmtFreq ( R.correction_si5351_no2, freqString, false );
    Serial.println ( freqString );
    Serial.println ( "We will start with that value." );
  }
  
	delay ( 1000 );
}

void SaveCorrection(void)
{
  SaveEEPROM ();
}
