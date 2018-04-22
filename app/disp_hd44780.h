/**
 * Project: ninHOME_OctoDisplay
 * @file disp_hd44780.h
 * @author Michael Sauer <sauer.uetersen@gmail.com>
 * @date 20.04.2018
 * @based ninHOME_Node
 *
 * HD44780 dependend routine header
 *
 * TODO: Implement functions to support 16x4/20x4 ... i got only 16x2 to play with
 */
#ifndef APP_DISP_HD44780_H_
#define APP_DISP_HD44780_H_

#include <Libraries/LiquidCrystal/LiquidCrystal_I2C.h>

#define DEFAULT_STARTUP_STRING_LINE1 "OctoDisplay"
#define DEFAULT_STARTUP_STRING_LINE2 "  is starting up"
#define DEFAULT_CLEARLINE_STRING "                "

class display_hd44780 {
	public:
		display_hd44780(octoprint_t *octo);
		void begin(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t backlighPin, t_backlighPol pol);
		void showPage(uint8_t displayPage);
		void printBig(uint8_t x, uint8_t num);
		LiquidCrystal_I2C getDisplay(void);
	private:
		// Set the LCD address to 0x27
		//LiquidCrystal_I2C lcd(0x27);
		LiquidCrystal_I2C *lcd; //(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
		octoprint_t *octoprintValues;
		uint8_t cols = 16;
		uint8_t rows = 2;
};

#endif /* APP_DISP_HD44780_H_ */
