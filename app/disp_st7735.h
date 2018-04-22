/**
 * Project: ninHOME_OctoDisplay
 * @file disp_st7735.h
 * @author Michael Sauer <sauer.uetersen@gmail.com>
 * @date 21.04.2018
 * @based ninHOME_Node
 *
 * ST7735 TFT dependend routine header
 *
 */
#ifndef APP_DISP_ST7735_H_
#define APP_DISP_ST7735_H_

#include <Libraries/Adafruit_ST7735/Adafruit_ST7735.h>
#include <Libraries/Adafruit_GFX/Adafruit_GFX.h>

#define TFT_CS     15
#define TFT_RST    5  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to -1!
#define TFT_DC     4

enum st7735_t {
	TFT14,
	TFT18
};

class display_st7735 {
	public:
		display_st7735(octoprint_t *octo);
		void begin(st7735_t type);
		void showPage(uint8_t displayPage);
		void printBig(uint8_t x, uint8_t num);
		Adafruit_ST7735 getDisplay(void);
	private:
		Adafruit_ST7735 *tft;
		octoprint_t *octoprintValues;
};



#endif /* APP_DISP_ST7735_H_ */
