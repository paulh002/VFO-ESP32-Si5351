

#ifndef _RINGMETER_H_
#define _RINGMETER_H_

#define RED2RED 0
#define GREEN2GREEN 1
#define BLUE2BLUE 2
#define BLUE2RED 3
#define GREEN2RED 4
#define RED2GREEN 5
#define GREEN3RED 6

#define ILI9341_GREY 0x2104 // Dark grey 16 bit colour

extern int ringMeter(int value, int vmin, int vmax, int x, int y, int r, char *units, byte scheme);


#endif
