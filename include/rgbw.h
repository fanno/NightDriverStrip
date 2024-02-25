/* FastLED_RGBW
 * 
 * Hack to enable SK6812 RGBW strips to work with FastLED.
 *
 * Original code by Jim Bumgardner (http://krazydad.com).
 * Modified by David Madison (http://partsnotincluded.com).
 * 
*/
#ifndef FastLED_RGBW_h
#define FastLED_RGBW_h 1

#include "FastLED.h"
#include <stdint.h>

struct CRGBW  {
	union {
		struct {
			union {
				uint8_t g;
				uint8_t green;
			};
			union {
				uint8_t r;
				uint8_t red;
			};
			union {
				uint8_t b;
				uint8_t blue;
			};
			union {
				uint8_t w;
				uint8_t white;
			};
		};
		uint8_t raw[4];
	};
	CRGBW(){}
	CRGBW(uint8_t rd, uint8_t grn, uint8_t blu, uint8_t wht){
		r = rd;
		g = grn;
		b = blu;
		w = wht;
	}
	inline void operator = (const CRGB c) __attribute__((always_inline)){ 
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->w = 0;
	}
};
inline uint16_t getRGBWsize(uint16_t nleds) {
	uint16_t nbytes = nleds * 4;
	if(nbytes % 3 > 0) return nbytes / 3 + 1;
	else return nbytes / 3;
}

inline void setRGBW(CRGB* led, int index, const CRGBW& color) {

	float start = index * (sizeof(CRGBW) / sizeof(CRGB) );

	int _pos = start;

	switch (index % 4)
	{
		case 1:
			led[_pos].g = color.r;
			led[_pos].b = color.g;
			led[_pos+1].r = color.b;
			led[_pos+1].g = color.w;
			break;
		case 2:
			led[_pos].b = color.r;
			led[_pos+1].r = color.g;
			led[_pos+1].g = color.b;
			led[_pos+1].b = color.w;
			break;
		default:
			led[_pos].r = color.r;
			led[_pos].g = color.g;
			led[_pos].b = color.b;
			led[_pos+1].r = color.w;
			break;
	}
}

inline void setRGBW(CRGB *led, int x, CRGB color) {
	CRGBW rgbw;
//	rgbw = color;

	rgbw.r = color.r;
	rgbw.g = color.g;
	rgbw.b = color.b;
	rgbw.w = 0;

	setRGBW(led, x, rgbw);
}

inline CRGB getRGBW(CRGB *led, int x) {
	CRGB color;

	float start = x * (sizeof(CRGBW) / sizeof(CRGB) );

	int _pos = start;
	switch (x % 4)
	{
		case 1:
			color.r = led[_pos].g;
			color.g = led[_pos].b;
			color.b = led[_pos+1].r;
			break;
		case 2:
			color.r = led[_pos].b;
			color.g = led[_pos+1].r;
			color.b = led[_pos+1].g;
			break;
		default:
			color.r = led[_pos].r;
			color.g = led[_pos].g;
			color.b = led[_pos].b;
			break;
	}
	return color;
}

#endif