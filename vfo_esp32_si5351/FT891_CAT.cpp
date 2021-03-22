/*
 *	"FT_891_CAT.cpp" contains everything needed to recognize and process a limited sub-set
 *	of FT-891 formatted	CAT messages.
 *
 *	This file along with the corresponding header file can be installed in a directory named
 *	"FT_891_CAT" in your Arduino library directory and used just like any other Arduino
 *	library. In other words, it could also be used by other programs as-is.
 *
 *	Modified 09/26/2019 - Version 1.1
 *
 *		Added a second version of the "begin" function which does not take a GPIO pin
 *		number that would be used to key the transmitter. If the pin number is specified,
 *		the library will manipulate the specified pin whenever a command to switch from
 *		transmit to receive or vice-versa is received.
 *
 *		If the GPIO pin number is not specified, it will be up to the using application
 *		to handle the transmit/receive functionality.
 */

#include <Arduino.h>							// Basic Arduino definitions
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESP32Encoder.h>
#include <AceButton.h>
using namespace ace_button;

#include "setup.h"
#include "gui.h"
#include "network.h"
#include "vfo.h"
#include "FT891_CAT.h"							// Our header


 /*-------------------------------------------------------
	CAT Interface
 --------------------------------------------------------*/
FT891_CAT CAT;

/*
 *	The "radioStatus" structure contains all of the current values for all the parameters
 *	we deal with. It is updated by "ProcessCmd()" and used as the source of current status
 *	in "ProcessStatus()".
 *
 *	The values to which things are initialized are the settings I typically use when I am
 *	operating MSK144 on 6 meters. If I change parameters using DxCommander and WSJT-X, the
 *	values will be updated by the "ProcessCmd()" function.
 *
 *	The program using this module is expected to initialize the contents as required by
 *	that application using the public functions.
 */

struct	status {
	uint32_t		FA       = 50260000UL;		// VFO-A current frequency
	uint32_t		FB       = 50260000UL;		// VFO-B current frequency
	int				IS_VALUE = 0;				// IF Shift amount
	uint8_t			IS_STAT  = 0;				// IF shift off
	const char*		ID 		 = "0650";			// ID for an FT-891
	uint8_t			AI		 = 0;				// Auto Information off		
	uint8_t			MDA		 = 2;				// Mode for VFO-A (USB)
	uint8_t			MDB		 = 2;				// Mode for VFO-B (USB)
	uint8_t			NA		 = 0;				// Narrow filter mode on or off
	uint8_t			RM		 = 0;				// Meter reading
	uint8_t			SH_STAT	 = 1;				// Adjustable filter width on
	uint8_t			SH_VALUE = 20;				// Adjustable filter width value = 3,000
	uint8_t			SM		 = 0;				// S-meter reading
	uint8_t			ST		 = 0;				// Split mode off
	uint8_t			TX		 = 0;				// Transmitter off
  uint8_t     BS     = 0;       // Band select
} radioStatus;


/*
 *	The "msgTable" provides a way of translating the ASCII message to the internal
 *	number. Again, this is a hook to allow other command languages to be implemented
 *	without major changes to the rest of the software.
 *
 *	The order of the table is based on how frequently status messages are received
 *	from the DxCommander program, but other than making the search run a tad faster,
 *	the order really doesn't matter.
 *
 *	Although the messages are all identified by the first 2 characters of the message,
 *	some entries in the table have the 2 alphabetical characters followed by a zero.
 *	In these cases, the 3rd character is always a zero which is then followed by any
 *	data that is included in the message. By including the zero in the name, it makes
 *	it easier to separate the data in "ParseMsg()".
 */

	msg	msgTable[] =
	{
		{ "IF",  MSG_IF, MSG_BOTH },		// Information request/answer
		{ "OI",  MSG_OI, MSG_BOTH },		// Opposite Band Information request/answer
		{ "RIC", MSG_RI, MSG_STS  },		// Alternate way of asking for split status
		{ "IS0", MSG_IS, MSG_STS  },		// Set or request IF shift
		{ "SH0", MSG_SH, MSG_STS  },		// Set or request IF bandwidth
		{ "SM0", MSG_SM, MSG_STS  },		// Read S-meter
		{ "NA0", MSG_NA, MSG_STS  },		// Request narrow IF shift
		{ "TX",  MSG_TX, MSG_BOTH },		// Set or request transmit/receive status
		{ "FA",  MSG_FA, MSG_BOTH },		// Set or request VFO-A frequency
		{ "FB",  MSG_FB, MSG_BOTH },		// Set or request VFO-B frequency
		{ "MD0", MSG_MD, MSG_BOTH },		// Set or request mode (USB, LSB, CW, etc.)
		{ "EX",  MSG_EX, MSG_BOTH },		// Menu commands (ignored)
		{ "RM",  MSG_RM, MSG_STS  },		// Read meter
		{ "ST",  MSG_ST, MSG_BOTH },		// ( 0 - 2) Split mode off, on or on +5KHz up
		{ "SV",  MSG_SV, MSG_CMD  },		// Swap VFOs
		{ "AB",  MSG_AB, MSG_CMD  },		// Copy VFO-A to VFO-B
		{ "BA",  MSG_BA, MSG_CMD  },		// Copy VFO-B to VFO-A
		{ "BS",  MSG_BS, MSG_BOTH },		// Band select
		{ "AI",  MSG_AI, MSG_BOTH },		// ( 0 or 1) Turn auto-information on or off
		{ "ID",  MSG_ID, MSG_STS  },		// Request radio's ID (0650 for the FT-891)
		{ "CL",  MSG_CL, MSG_STS },    // Request call sign
		{ "",    MSG_NONE, 0 }				// Command not found in the list
	};


	int	 pttPin;							// GPIO pin to key the transmitter
	bool addNewline  = false;				// Add a newline to the output if 'true'
	bool keyTransmit = true;				// SetTX() will operate the transmitter if 'true'

	
/*
 *	Only one constructor; nothing to do really.
 */

FT891_CAT::FT891_CAT () {}


/*
 *	"begin()" does a few things to setup the CAT control functionality. The
 *	optional "debug" controls whether or not to add a new line character to
 *	the serial output for debugging.
 *
 *	There are two versions. If the PTT pin is specified, the "SetTX()" function
 *	will turn the transmitter on and off. If the PTT pin is not specified,
 *	operating the transmitter is up to the using program.
 *
 *	The "Init()" function handles the common logic for both.
 */

void FT891_CAT::begin ( int txPin, bool debug )
{
	keyTransmit = true;							// Pin specified, so we operate the transmitter
	
	pttPin     = txPin;							// Remember GPIO pin to key the transmitter

  callsign = "PA0PHH";


/*
 *	Set up the digital pins used:
 */

	pinMode ( pttPin, OUTPUT );					// Turns the transmitter on or off
	digitalWrite ( pttPin, XMIT_OFF );			// Turn the transmitter off

	Init ( debug );								// Do common stuff
}


void FT891_CAT::begin ( bool debug )			// No transmitter PTT pin specified
{
	keyTransmit = false;						// Pin not specified, don't operate the transmitter
	
	Init ( debug );								// Do common stuff
}



void FT891_CAT::Init ( bool debug )
{
	MSG_COUNT = ELEMENTS ( msgTable );			// Number of messages in the list
	addNewline = debug;							// Debug mode or not

	
/*
 *	Clear all the buffers
 */

	memset ( rxBuff,   0, BUF_LEN );			// DxCommander receive buffer
	memset ( dataBuff, 0, BUF_LEN );			// Data part of an incoming message

	for ( int ix = 0; ix < BUF_COUNT; ix++ )	// Multiple transmit buffers
		memset ( txBuff[ix], 0, BUF_LEN );		// Need to be cleared
}


/*
 *	"FT891_CAT::CheckCAT ()" first looks for an incoming message and it there is one
 *	to be processed, it does so.
 *
 *	If there is no new message or if the new message is a status request (which
 *	doesn't change the status of anything, it returns "false". If a command type
 *	message was processed, it returns true.
 *
 *	It is up to the calling program to figure out what changed and make any
 *	determinations about the validity of the new data.
 */

bool FT891_CAT::CheckCAT ()
{
bool newCmd = false;						// True if command message processed

	if ( GetMessage ())						// See if new message
	{	
		newMessage = FindMsg ();				// If so, look it up in the table
		if ( newMessage.ID == MSG_NONE )		// Not found?
		{
			return false;						// We're done
		}
		ParseMsg ();							// Separate any data in the message
		if ( newMessage.Type == MSG_CMD )		// Command?
		{
			if ( ProcessCmd ())					// Yes, process it
				newCmd = true;					// Indicate command received & processed
		}
		else									// It's a status request
		{
			ProcessStatus ();					// Process it
		}
	}
	memset(rxBuff, 0, BUF_LEN);					// Clear the receive buffer
	return newCmd;								// Done!
}


/*
 *	"GetMessage()" reads a command from the Serial port if there is anything to read.
 *
 *	When using the serial monitor for testing, the command letters may be entered in
 *	either upper or lower case.
 */

bool FT891_CAT::GetMessage ()
{
	int	i;										// Loop counter

if (CatServer.Anyconnection())
  {
	memset(rxBuff, 0, sizeof(rxBuff));
	if (CatServer.read((const uint8_t*)rxBuff, sizeof(rxBuff)))
	{
		char* ptr = strchr(rxBuff, ';'); // TERM_CH
		if (ptr)
		{
			*ptr = '\0';
		
			for (i = 0; i < strlen(rxBuff); i++)	// Translate incoming message
				rxBuff[i] = toupper(rxBuff[i]);		// to all upper case
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
  }
  else
  {
	if ( !Serial.available ())					// Anything in the input buffer?
		return false;							// If not, no message yet

	Serial.readBytesUntil ( TERM_CH, rxBuff, sizeof ( rxBuff ));
  }
  
	for ( i = 0; i < strlen ( rxBuff ); i++ )	// Translate incoming message
		rxBuff[i] = toupper ( rxBuff[i] );		// to all upper case

	return true;								// There is a new message
}


/*
 *	"FindMsg" searches the msgTable for the first 2 characters of the message that is now
 *	in the "rxBuff" and returns a copy of the entry from the table if found. Even though
 *	some of the entries in the table have 3 character identifiers, we are only concerned
 *	with the first 2 to identify the incoming message.
 *
 *	Once we find a match on the first 2 characters we test for an incomplete message. This
 *	applies to all the ones that have the built-in zero as part of the message identifier.
 *	If the message is missing the expected "0", we treat it as "MSG_NONE".
 */

msg FT891_CAT::FindMsg ()
{
	int index;											// Loop index

	for ( index = 0; index < MSG_COUNT; index++ )		//Search the table
	{
		if ( strncmp ( rxBuff, msgTable[index].Name, 2 ) == 0 )					// Find a match?
			{
			  if ( strlen ( rxBuff ) >= strlen ( msgTable[index].Name ))	// Complete message?
				{
				  //Serial.println ("MSG: Index " + String(index) + " " + String(msgTable[index].Name));
				  return msgTable[index];									// Yes, return the entry
				}
			else										// If not,
				return msgTable[MSG_COUNT - 1] ;		// Return MSG_NONE entry
			}
	}

	return msgTable[MSG_COUNT - 1] ;					// Return MSG_NONE entry - nothing found
}


/*
 *	"ParseMsg" checks to see if the message has data attached. If so, the data section of
 *	the message (in "rxBuff") is copied to "dataBuff", the "msg.Type" indicator is set to
 *	"MSG_CMD" and we're done (any message that includes data is automatically a command).
 *
 *	If there is no data, we need to look at the "msg.Type" to see if it is one of those
 *	messages that can only be a command (eg. "AB"). if that is the case, we just return.
 *
 *	If The message has no data and is one that could be either a command or a status
 *	request, we set the "msg.Type" indicator to "MSG_STS" and we're done.
 *
 *	Note, when we change things in "entry", we are making changes to a copy of the entry
 *	from the "msgTable", not to the master array itself.
 *
 *	We return the "hasData" or not indicator.
 */

bool FT891_CAT::ParseMsg ()
{
	int		prefixLength;							// The length of the command string itself
	int 	messageLength;							// Length of entire message
	int		i;										// Loop index

	hasData = false;								// True when message has data

	memset ( dataBuff, 0, sizeof ( dataBuff ));		// Clear the data buffer

	prefixLength  = strlen ( newMessage.Name );		// Get command ID length from table entry
	messageLength = strlen ( rxBuff );				// Length of entire message


/*
 *	Case #1: See if message has any data. If so, copy the data to "dataBuff" amd 
 *	set the "hasData" indicator to true and set the "msg.Type" to "MSG_CMD".
 */

	if ( messageLength > ( prefixLength ))			// If true, there is data attached
	{
		hasData = true;
		newMessage.Type = MSG_CMD;					// Anything with data is a command

		for ( i = 0; i < messageLength - prefixLength; i++ )		// Put the data in
			dataBuff[i] = rxBuff[prefixLength + i];					// "dataBuff"

		dataBuff[i] = '\0';							// Make sure terminated

		return hasData;								// No more to do
	}


/*
 *	Case #2: Message has no data.
 */

	if ( newMessage.Type == MSG_CMD )				// It can only be a command
		return hasData;								// Which is "false"


/*
 *	Case #3: It must be a status request.
 */

	newMessage.Type = MSG_STS;						// Indicate so
		return hasData;								// Which is "false"
}


/*
 *	"ProcessCmd()" processes the specified command. The list is currently in alphabetical
 *	order. If desired, the list can be re-ordered to allow the most often messages to be
 *	found faster.
 *
 *	The structure "newMessage" contains the "ID" of the command. If "hasData" is "true",
 *	the data part of the command will be in "dataBuff".
 *
 *	We convert the  portion of the message to the appropriate type (unsigned long,
 *	uint8_t, etc.) and store it in the "radioStatus" structure and do anything else that
 *	might be required (like turn the transmitter on or off).
 *
 *	Note that there are no checks for illegal  values. We assume that the programs
 *	talking to this one do not send illegal values; it is up to the program using
 *	this library to determine the validity of the data.
 *
 *	You'll notice that the code for processing some of the commands is commented out.
 *	This is because in the application that this was developed for, those commands
 *	are not used. I left the code in here should you want to enable them for some
 *	other application.
 */


bool FT891_CAT::ProcessCmd ()
{
	char 		tempBuff[12];							// Needed to disect "IF" & "OI" messages
	uint32_t	tempFreq;								// Needed to swap frequencies
	uint8_t		tempMode;								// Needed to swap modes
	uint8_t		catMode;								// Used to look for CAT mode
	uint8_t		oldMode;								// Ditto

	bool		cmdProcessed = false;					// True if we actually did something

	memset ( tempBuff, 0, sizeof ( tempBuff ));			// Clear the temporary buffer

	switch ( newMessage.ID )							// Make decisions based on message ID
	{    
		case MSG_AB:									// Copy VFO-A to VFO-B (and mode)
			radioStatus.FB  = radioStatus.FA;			// VFO-A frequency now in VFO-B
			radioStatus.MDB = radioStatus.MDA;			// And mode
			cmdProcessed = true;						// Command was processed
			break;

		case MSG_BA:									// Copy VFO-B to VFO-A (and mode)
			radioStatus.FA  = radioStatus.FB;			// VFO-B frequency now in VFO-A
			radioStatus.MDA = radioStatus.MDB;			// And mode
			cmdProcessed = true;						// Command was processed
			break;

		case MSG_EX:									// FT-891 "Menu" commands (ignored)
			cmdProcessed = true;						// Command was processed
			break;

		case MSG_FA:									// Set VFO-A frequency
			tempFreq = atol ( dataBuff );				// Convert into temporary place
			SetFA ( tempFreq );							// Update radioStatus.FA
			cmdProcessed = true;
			break;

		case MSG_FB:									// Set VFO-B frequency
			tempFreq = atol ( dataBuff );				// Convert into temporary place
			SetFB ( tempFreq );							// Update radioStatus.FB
			cmdProcessed = true;
			break;

		case MSG_IF:									// Can be used to set VFO-A frequency & mode
//			strncpy ( tempBuff, &dataBuff[3], 9 );		// Extract the VFO-A frequency

//			radioStatus.FA = atol ( tempBuff );			// Convert & save
//			tempBuff[0] = dataBuff[19];					// Extract the mode character
//			tempBuff[1] = '\0';							// Add a null to terminate
//			tempMode = xtoi ( tempBuff );				// Convert

//			if ( tempMode != 0 )						// Zero is invalid
//				radioStatus.MDA = tempMode;
			cmdProcessed = true;						// Command was processed
			break;

		case MSG_IS:									// Set IF shift & status
//			strncpy ( tempBuff, &dataBuff[1], 5 );		// Extract the offset amount
//			radioStatus.IS_VALUE = atoi ( tempBuff );	// Set value in the structure
//			radioStatus.IS_STAT = dataBuff[0] - '0';	// Set on/off status
			cmdProcessed = true;						// Command was processed
			break;

		case MSG_MD:									// Set mode (USB, LSB, CW, etc.)
			tempMode = xtoi ( dataBuff );				// Convert to a number

			SetMDA ( tempMode );						// Set in radioStatus
			SetMDB ( tempMode );						// Both modes for now

			cmdProcessed = true;						// Command was processed
			break;

		case MSG_OI:									// Can be used to set VFO-B frequency & mode
//			strncpy ( tempBuff, &dataBuff[3], 9 );		// Extract the VFO-B frequency
//			radioStatus.FB = atol ( tempBuff );			// Convert & save
//			tempBuff[0] = dataBuff[19];					// Extract the mode character
//			tempBuff[1] = '\0';							// Add a null to terminate
//			tempMode = xtoi ( tempBuff );				// Convert

//			if ( tempMode != 0 )						// Zero is invalid
//				radioStatus.MDB = tempMode;

			cmdProcessed = true;						// Command was processed
			break;

		case MSG_RM:									// Get TX meter reading
//			radioStatus.RM = dataBuff[0] - '3';			// Set value in the structure
//			strncpy ( tempBuff, &dataBuff[1], 3 );		// Extract the VFO-A frequency
//			tempBuff[3] = '\0';
//			radioStatus.RM = atoi ( tempBuff );

			cmdProcessed = true;						// Command was processed
			break;

		case MSG_SM:									// S-meter reading
//			radioStatus.SM = atoi ( dataBuff );			// Set value in the structure

			cmdProcessed = true;						// Command was processed
			break;

		case MSG_ST:									// Turn split mode on or off
			radioStatus.ST = dataBuff[0] - '0';			// Cheap ASCII to integer trick!
			cmdProcessed = true;						// Command was processed
			break;

		case MSG_SV:									// Swap VFOs
			tempFreq = radioStatus.FA;					// Save VFO-A frequency
			tempMode = radioStatus.MDA;					// And mode
			radioStatus.FA  = radioStatus.FB;			// Copy VFO-B frequency to VFO-A
			radioStatus.MDA = radioStatus.MDB;			// And mode
			radioStatus.FB  = tempFreq;					// Old VFO-A frequency now in VFO-B
			radioStatus.MDB = tempMode;					// And mode

			cmdProcessed = true;						// Command was processed
			break;

		case MSG_TX:									// Set transmit/receive status
			SetTX ( dataBuff[0] - '0' );				// Cheap trick ASCII -> int conversion!
			cmdProcessed = true;
			break;

		case MSG_NONE:									// Command not found in the list
			break;
	}

	return cmdProcessed;
}


/*
 *	"ProcessStatus()" formats appropriate responses to status requests from DxCommander
 *	in the "tempBuff" and sends them. The ID of the current message is in the "newMessage"
 *	structure.
 *
 *	The list is in order by frequency of use, so the most often used requests will be 
 *	found the fastest.
 */

void FT891_CAT::ProcessStatus ()
{
	uint16_t buffSize;									// Number of characters to be sent
	char	 tempBuff[BUF_LEN];							// Temporary buffer

	memset ( tempBuff, 0, BUF_LEN );					// Clear the temporary buffer
  
  
	switch ( newMessage.ID )							// Make decisions based on message ID
	{
    case MSG_CL:                      // print callsign
      callsign.toCharArray(tempBuff, BUF_LEN);   
      break;
      
	case MSG_IF:									// Information request
		sprintf ( tempBuff,							// Format response
				"IF000%09lu+000000%1u00000;",
				radioStatus.FA, radioStatus.MDA );
		Serial.println(tempBuff);
		break;

	case MSG_OI:									// Opposite Band Information request
		sprintf ( tempBuff,							// Format response
				"OI000%09lu+000000%1u00000;",
				radioStatus.FB, radioStatus.MDB );
		break;

	case MSG_RI:									// Alternate way of requesting split status
		sprintf ( tempBuff,							// Format message
				"RIC%1u;", radioStatus.ST );
		break;

	case MSG_IS:									// Request IF shift status
		sprintf ( tempBuff, "IS0%1u%+05d;",			// Format message
					radioStatus.IS_STAT,
					radioStatus.IS_VALUE );
		break;

	case MSG_SH:									// Request IF bandwidth setting
		sprintf ( tempBuff,							// Format message
				"SH0%1u%02u;",
				radioStatus.SH_STAT,
				radioStatus.SH_VALUE );
		break;

	case MSG_SM:									// Read S-meter
		sprintf ( tempBuff,							// Format message
				"SM0%03u;", radioStatus.SM );
		break;

	case MSG_NA:									// Request narrow IF shift status
		sprintf ( tempBuff,							// Format message
				"NA0%1u;", radioStatus.NA );
		break;

	case MSG_TX:									// Request transmit/receive status
		sprintf ( tempBuff,							// Format message
				"TX%1u;", radioStatus.TX );
		break;

	case MSG_FA:									// Request VFO-A frequency
		sprintf ( tempBuff,							// Format message
				"FA%09lu;", radioStatus.FA );
		break;

	case MSG_FB:									// Request VFO-B frequency
		sprintf ( tempBuff,							// Format message
				"FB%09lu;", radioStatus.FB );
		break;

	case MSG_MD:									// Request mode (USB, LSB, CW, etc.)
		sprintf ( tempBuff,							// Format message
				"MD0%1u;", radioStatus.MDA );
		break;

	case MSG_RM:									// Read meter
		break;

	case MSG_ST:									// Request split mode status
		sprintf ( tempBuff,							// Format message
				"ST%1u;", radioStatus.ST );
		break;

	case MSG_AI:									// Request for auto-information status
		sprintf ( tempBuff,							// Format message
				"AI%1u;", radioStatus.AI );
	break;

	case MSG_ID:									// Request radio's ID (0650 for the FT-891)
		strcpy ( tempBuff, "ID0650;" );				// Format message
		break;

	case MSG_NONE:									// Command not found in the list
		break;
	}


/*
 *	If the "tempBuff" has anything in it, we need to sent it to DxCommander. If there is nothing
 *	in the buffer, we just return.
 *
 *	Note there are both a "Serial.print" and a "Serial.println" function calls to send the
 *	message with a newline (for testing) or not (for DxCommander). The choice is controlled
 *	by the optional "debug" argument (aka addNewline) of the "begin" function.
 */

	buffSize = strlen ( tempBuff );

	if ( buffSize != 0 )								//  Anything to be sent?
	{
		Serial.println ("process status");
		strcpy ( txBuff[txBuffIndex], tempBuff );

    if (CatServer.Anyconnection())
    {
    if ( addNewline )
      {
        CatServer.write ( (const uint8_t *)txBuff[txBuffIndex], strlen(txBuff[txBuffIndex]) );   // Send the message (debugging mode)
        CatServer.write ( (const uint8_t *)"\r\n", strlen("\r\n") );   // Send the message (debugging mode)      
      }
    else
        CatServer.write ( (const uint8_t *)txBuff[txBuffIndex], strlen(txBuff[txBuffIndex]) );   // Send the message with no newline
    }
    else
    {

		if ( addNewline )
			Serial.println ( txBuff[txBuffIndex] );		// Send the message (debugging mode)
		else
			Serial.print ( txBuff[txBuffIndex] );		// Send the message with no newline
			}
		txBuffIndex++;
		txBuffIndex %= BUF_COUNT - 1;
	}
}

void FT891_CAT::Processautoinformation (uint8_t message)
{
  uint16_t buffSize;                    // Number of characters to be sent
  char   tempBuff[BUF_LEN];             // Temporary buffer

  memset ( tempBuff, 0, BUF_LEN );      // Clear the temporary buffer
  switch ( message )              // Make decisions based on message ID
  {
    case MSG_TX:
    if ( radioStatus.TX )
      strcpy(tempBuff,"TX1;");
    else
      strcpy(tempBuff,"TX0;");
    break;

    case MSG_FA:
    sprintf ( tempBuff,              // Format message
          "FA%09lu;", radioStatus.FA );
    break;

    case MSG_FB:
    sprintf ( tempBuff,              // Format message
          "FB%09lu;", radioStatus.FB );
    break;
  }

if (CatServer.Anyconnection() && strlen(tempBuff))
    {
    Serial.println ("Send data ");
    if ( addNewline )
      {
        CatServer.write ( (const uint8_t *)tempBuff, strlen(tempBuff) );   // Send the message (debugging mode)
        CatServer.write ( (const uint8_t *)"\r\n", strlen("\r\n") );   // Send the message (debugging mode)      
      }
    else
        CatServer.write ( (const uint8_t *)tempBuff, strlen(tempBuff) );   // Send the message with no newline
    }
}

/*
 *	"xtoi()" works similar to "atoi()" except the string is assumed to be hex numbers. It will
 *	convert characters in the string as long as the next character in the string is a valid hex
 *	digit. In other words it stops converting if it sees a null or a ';', or any thing else that
 *	is not a valid hex digit. The only exception is the character "X" (or "x"), which is ignored.
 *	This allows it to correctly convert a string in the "0xNN..." format.
 */

unsigned FT891_CAT::xtoi ( char* hexString )
{
int			ix;											// Loop index
char		c;											// Single character
uint16_t	answer = 0;									// The answer

	for ( ix = 0; ix < strlen ( hexString ); ix++ )
	{
		c = toupper ( hexString[ix] );						// Get next character

		if ( c == 'X' )
			continue;

		if ( isdigit ( c ))									// Normal base 10 digit?
			answer = ( answer * 16 ) + ( c - '0' );			// shift answer and add new number

		else if ( c >= 'A' && c <= 'F' )					// Valid hex digit?
			answer = ( answer * 16 ) + ( c - 'A' + 10 );	// Shift answer and add new number

		else
			break;
	}

	return answer;
}


/*
 *	The following functions are "public" functions that allow the using application'
 *	to set data in or retreive data from the "radioStatus" structure.
 *
 *	"SetFA()" and "SetFB()" is a way for the outside world to set "radioStatus.FA"
 *	and "radioStatus.FB". "GetFA()" and "GetFB()" provide a means for the outside
 *	world to ask for the frequencies.
 */

void FT891_CAT::SetFA ( uint32_t freq )			// Set VFO-A frequency
{
	radioStatus.FA = freq;						// Done!
  Processautoinformation (MSG_FA);
}

void FT891_CAT::SetFB ( uint32_t freq )			// Set VFO-B frequency
{
	radioStatus.FB = freq;						// Done!
  Processautoinformation (MSG_FB);
}

uint32_t FT891_CAT::GetFA ()					// Get VFO-A frequency
{
	return radioStatus.FA;						// Done!
}

uint32_t FT891_CAT::GetFB ()					// Get VFO-B frequency
{
	return radioStatus.FB;						// Done!
}


/*
 *	"SetMDA()" and "SetMDB()" is a way for the outside world to set "radioStatus.MDA"
 *	and "radioStatus.MDB". "GetMDA()" and "GetMDB()" provide a way for the outside
 *	world to ask for the modes.
 */

void FT891_CAT::SetMDA ( uint8_t mode )			// Set VFO-A mode
{
	radioStatus.MDA = mode;						// Done!
  Processautoinformation (MSG_MD);
}

void FT891_CAT::SetMDB ( uint8_t mode )			// Set VFO-B mode
{
	radioStatus.MDB = mode;						// Done!
  Processautoinformation (MSG_MD);
}

void FT891_CAT::SetBS ( uint8_t mode )      // Set band select
{
  radioStatus.BS = mode;           // Done!
  Processautoinformation (MSG_BS);
}

uint8_t FT891_CAT::GetBS ()          // Get Band select frequency
{
  return radioStatus.BS;            // Done!
}

uint8_t FT891_CAT::GetMDA ()					// Get VFO-A mode
{
	return radioStatus.MDA;						// Done!
}

uint8_t FT891_CAT::GetMDB ()					// Get VFO-B mode
{
	return radioStatus.MDB;						// Done!
}


/*
 *	Allow the main program to set or get the transmit/receive status:
 */

void FT891_CAT::SetTX ( uint8_t tx )				// Set transmit/receive status
{
	uint8_t old_tx = radioStatus.TX;
	radioStatus.TX = tx;							// Set status
  if (old_tx != tx)
    Processautoinformation (MSG_TX);

	if ( keyTransmit )								// Only operate the transmitter if 'true'
	{
		if ( radioStatus.TX )						// Transmit mode?
			digitalWrite ( pttPin, XMIT_ON );		// Turn the transmitter on

		else
			digitalWrite ( pttPin, XMIT_OFF );		// Back to receive mode
	}
}

uint8_t FT891_CAT::GetTX ()							// Get transmit/receive status
{
	return radioStatus.TX;							// Done!
}


/*
 *	Allow the main program to set or get the split mode status:
 */

void FT891_CAT::SetSM ( uint8_t sm )			// Set split mode status
{
	radioStatus.SM = sm;						// Done!
}

uint8_t FT891_CAT::GetSM ()						// Get split mode status
{
	return radioStatus.SM;						// Done!
}

/*
 *	Allow the main program to set or get the split mode status:
 */

void FT891_CAT::SetST ( uint8_t st )			// Set split mode status
{
	radioStatus.ST = st;						// Done!
}

bool FT891_CAT::GetST ()						// Get split mode status
{
	return radioStatus.ST;						// Done!
}
