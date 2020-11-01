#include <arduino.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>

#include "ringmeter.h"

#include <Adafruit_ILI9341.h>
#include "Fonts/FreeSans18pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSans7pt7b.h"
#include "Fonts/FreeSans24pt7b.h"

extern  Adafruit_ILI9341 tft;

extern unsigned int rainbow(byte value);
extern int drawCentreString(char *string, int dX, int poY, int size, int erase);

int drawCentreString(char *string, int dX, int poY, int size, int erase)
{
  
    int16_t   x1,y1;
    uint16_t  w,h;
    char      buf[10];

    switch (size)
    {
      case 1:
        tft.setFont(&FreeSans7pt7b);
        break;
      case 2:
        tft.setFont(&FreeSans9pt7b);
        break;
      case 3:
        tft.setFont(&FreeSans12pt7b);
        break;
      case 4:
        tft.setFont(&FreeSans18pt7b);
        break;
      case 5:
        tft.setFont(&FreeSans24pt7b);
        break;
    }
    
    if (erase)
    {
      char ch = '9', str[10];
      erase = min(erase,9);
      memset(str,0,10*sizeof(char));
      
      for (int i=0; i < erase; i++)
        {
        strncat(str, &ch, 1); 
        }
      tft.getTextBounds(str, (int16_t)dX, (int16_t) poY, &x1, &y1, &w, &h);
      int poX = dX - w/2;
      if (poX < 0) poX = 0;
      tft.fillRoundRect(poX,y1-2,w+6,h+4,5,ILI9341_BLACK);  
    }
    
    
    tft.getTextBounds(string, (int16_t)dX, (int16_t) poY, &x1, &y1, &w, &h);
    int poX = dX - w/2;
    if (poX < 0) poX = 0;
    tft.setCursor(poX,poY);
    tft.print(string);
    return w;  
}

// #########################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// #########################################################################
int ringMeter(int value, int vmin, int vmax, int x, int y, int r, char *units, byte scheme, int digits)
{
  // Minimum value of r is about 52 before value text intrudes on ring
  // drawing the text first is an option
  
  value = min(value,vmax);
  value = max(value,vmin);
  
  x += r; y += r;   // Calculate coords of centre of ring

  int w = r / 4;    // Width of outer ring is 1/4 of radius
  
  int angle = 150;  // Half the sweep angle of meter (300 degrees)

  int text_colour = 0; // To hold the text colour

  int v = map(value, vmin, vmax, -angle, angle); // Map the value to an angle v

  byte seg = 5; // Segments are 5 degrees wide = 60 segments for 300 degrees
  byte inc = 5; // Draw segments every 5 degrees, increase to 10 for segmented ring

  // Draw colour blocks every inc degrees
  for (int i = -angle; i < angle; i += inc) {

    // Choose colour from scheme
    int colour = 0;
    switch (scheme) {
      case RED2RED:     colour = ILI9341_RED; break; // Fixed colour
      case GREEN2GREEN: colour = ILI9341_GREEN; break; // Fixed colour
      case BLUE2BLUE:   colour = ILI9341_BLUE; break; // Fixed colour
      case BLUE2RED:    colour = rainbow(map(i, -angle, angle, 0, 127)); break; // Full spectrum blue to red
      case GREEN2RED:   colour = rainbow(map(i, -angle, angle, 63, 127)); break; // Green to red (high temperature etc)
      case RED2GREEN:   colour = rainbow(map(i, -angle, angle, 127, 63)); break; // Red to green (low battery etc)
      case GREEN3RED:   colour = rainbow(map(i, -angle, angle, 100, 127)); break; // Green to red (high temperature etc)
      default: colour = ILI9341_BLUE; break; // Fixed colour
    }

    // Calculate pair of coordinates for segment start
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (r - w) + x;
    uint16_t y0 = sy * (r - w) + y;
    uint16_t x1 = sx * r + x;
    uint16_t y1 = sy * r + y;

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * 0.0174532925);
    float sy2 = sin((i + seg - 90) * 0.0174532925);
    int x2 = sx2 * (r - w) + x;
    int y2 = sy2 * (r - w) + y;
    int x3 = sx2 * r + x;
    int y3 = sy2 * r + y;

    if (i < v) { // Fill in coloured segments with 2 triangles
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
      text_colour = colour; // Save the last colour drawn
    }
    else // Fill in blank segments
    {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, ILI9341_GREY);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, ILI9341_GREY);
    }
  }

  // Convert value to a string
  char buf[10], buf1[10];
  byte len = 3; if (value > 999) len = 4;
  dtostrf(value, len, 0, buf);
  if (digits == 2)
  {
    memset(buf1,0,10);
    strcpy(buf1,buf);
    buf1[1] = '.';
    strcpy(&buf1[2], &buf[1]);
    strcpy(buf, buf1);
  }
  
  // Set the text colour to default
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  // Uncomment next line to set the text colour to the last segment value!
  // tft.setTextColor(text_colour, ILI9341_BLACK);
  
  // Print value, if the meter is large then use big font 6, othewise use 4
  if (r > 84) 
    drawCentreString(buf, x - 5, y - 20, 4,2); // Value in middle
  else if (r > 40) 
          drawCentreString(buf, x - 5, y - 10, 3,3); // Value in middle
       else
          drawCentreString(buf, x - 5, y - 10, 2,3); // Value in middle

  // Print units, if the meter is large then use big font 4, othewise use 2
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  if (r > 84) drawCentreString(units, x, y + 30, 2,0); // Units display
  else if (r > 40) 
          drawCentreString(units, x, y + 5, 1,0); // Units display
       else
          drawCentreString(units, x, y + 5, 1,0); // Units display

  // Calculate and return right hand side x coordinate
  return x + r;
}

// #########################################################################
// Return a 16 bit rainbow colour
// #########################################################################
unsigned int rainbow(byte value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red

  byte red = 0; // Red is the top 5 bits of a 16 bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits

  byte quadrant = value / 32;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}
