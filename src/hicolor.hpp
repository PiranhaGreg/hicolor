#pragma once

/*
	************************************************************************************
	* Utility to convert images from *.ppm (Portable PixMap) to *.HI (HIcolor) and back.
	************************************************************************************
	
	HIcolor format:
	
	<2B width>
	<2B height> 
	<2B type>
	[<512B palette>]
	<data>

	Where type can be
		8 - using 256 colors palette
		15 - full 32768 color image
	and one color is represented in RGB555 with first bit set to zero (0RRRRRGGGGGBBBBB).
*/

#include <cstdint>

extern const char * appName;
extern const char * appVersion;

extern const uint16_t R_mask;
extern const uint16_t G_mask;
extern const uint16_t B_mask;

extern const unsigned int paletteSize;

extern const unsigned int maxColor;
extern const char * ASCII;
extern const char * BIN;

enum Type { Palette = 8, Data = 15 };

void loadPPM(const char * input, uint16_t ** data, uint16_t * width, uint16_t * height, uint16_t * colors);
void savePPM(const char * file, const uint8_t * data, uint16_t width, uint16_t height, const char * format);

void hi2ppm(const char * input, const char * output, const char * format);
void ppm2hi(const char * input, const char * output, Type type);