/**
 * Project: ninHOME_OctoDisplay
 * @file disp_st7735.cpp
 * @author Michael Sauer <sauer.uetersen@gmail.com>
 * @date 21.04.2018
 * @based ninHOME_Node
 *
 * ST7735 TFT dependend routines
 *
 */

#include "application.h"
#include "disp_st7735.h"

display_st7735::display_st7735(octoprint_t *octo) {
	this->octoprintValues = octo;
}

void display_st7735::begin(st7735_t type) {
	this->tft = new Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
	if (type == TFT14) {
		tft->initR(INITR_144GREENTAB);// 1,4"
	} else if (type == TFT18) {
		tft->initR(INITR_BLACKTAB); // 1,8"
	} else {
		//TODO: ERROR Handling
		tft->fillScreen(ST7735_RED);

	}
}

Adafruit_ST7735 display_st7735::getDisplay(void) {
	return *tft;
}
