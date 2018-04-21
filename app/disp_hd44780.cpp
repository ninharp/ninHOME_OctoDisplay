/**
 * Project: ninHOME_OctoDisplay
 * @file disp_hd44780.cpp
 * @author Michael Sauer <sauer.uetersen@gmail.com>
 * @date 20.04.2018
 * @based ninHOME_Node
 *
 * HD44780 dependend routines
 *
 */

#include "application.h"
#include "disp_hd44780.h"

/* LCD HD44780 Custom Chars */
byte nozzle_icon[8] = {
	0b00100,
	0b01010,
	0b01010,
	0b01010,
	0b01010,
	0b10001,
	0b10001,
	0b01110
};
byte degree_icon[8] = {
	0b01100,
	0b10010,
	0b10010,
	0b01100,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

byte num_ug[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111
};

byte num_og[8] = {
	0b11111,
	0b11111,
	0b11111,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

byte num_um[8] = {
	0b11111,
	0b11111,
	0b11111,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111
};

byte num_full[8] = {
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

byte percent[8] = {
	0b11111,
	0b11111,
	0b11011,
	0b11011,
	0b11011,
	0b11011,
	0b11111,
	0b11111
};

byte percent_b[8] = {
	0b00011,
	0b00011,
	0b00110,
	0b00110,
	0b01100,
	0b01100,
	0b11000,
	0b11000
};

int8_t font_big_num[11][7] {
	{ 5,4,5,-2,5,2,5 },  //0
	{ 4,5,-1,-2,2,5,2 }, //1
	{ 3,3,5,-2,5,2,2 },  //2
	{ 3,3,5,-2,2,2,5 },  //3
	{ 5,2,5,-2,-1,-1,5 },//4
	{ 5,3,3,-2,2,2,5 },  //5
	{ 5,3,3,-2,5,2,5 },  //6
	{ 4,4,5,-2,-1,-1,5 },//7
	{ 5,3,5,-2,5,2,5 },  //8
	{ 5,3,5,-2,-1,-1,5 },//9
	{ -1,6,7,-2,-1,7,6 }
};

display_hd44780::display_hd44780(octoprint_t *octo) {
	//(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE)
	this->octoprintValues = octo;
}

void display_hd44780::begin(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t backlighPin, t_backlighPol pol) {
	this->lcd = new LiquidCrystal_I2C(lcd_Addr, En, Rw, Rs, d4, d5, d6, d7, backlighPin, pol);
	lcd->begin(cols, rows);
	lcd->createChar(0, nozzle_icon);
	lcd->createChar(1, degree_icon);
	lcd->createChar(2, num_ug);
	lcd->createChar(3, num_um);
	lcd->createChar(4, num_og);
	lcd->createChar(5, num_full);
	lcd->createChar(6, percent);
	lcd->createChar(7, percent_b);

	lcd->clear();
	lcd->home();
	//lcd->print(DEFAULT_STARTUP_STRING);
	printBig(4, 9);
	printBig(8, 3);
	printBig(12, 10);
}

void display_hd44780::printBig(uint8_t x, uint8_t num) {
	uint8_t y = 0;
	lcd->setCursor(x, y);
	for (uint8_t c = 0; c < 7; c++) {
		if (font_big_num[num][c] == -2)
			lcd->setCursor(x, y+1);
		else if (font_big_num[num][c] == -1)
			lcd->write((uint8_t)32);
		else
			lcd->write((uint8_t)font_big_num[num][c]);
	}
}

void display_hd44780::showPage(uint8_t displayPage) {
	if (displayPage == 0) {
			//lcd.clear();
			lcd->home();
			lcd->print(DEFAULT_CLEARLINE_STRING);
			lcd->home();
			lcd->write((uint8_t)0);
			lcd->printf("%03d", (int)round(octoprintValues->temperature[0].actual));
			lcd->write((uint8_t)1);
			lcd->print("/");
			lcd->printf("%03d", (int)round(octoprintValues->temperature[0].target));
			lcd->write((uint8_t)1);
			//lcd->print("     ");

			lcd->setCursor(0,1);
			lcd->print(DEFAULT_CLEARLINE_STRING);
			lcd->setCursor(0,1);
			lcd->print("B");
			lcd->printf("%03d", (int)round(octoprintValues->temperature[octoprintValues->num_extruders].actual));
			lcd->write((uint8_t)1);
			lcd->print("/");
			lcd->printf("%03d", (int)round(octoprintValues->temperature[octoprintValues->num_extruders].target));
			lcd->write((uint8_t)1);
			//lcd->print("      ");
		} else if (displayPage == 1) {
			lcd->home();
			lcd->print("Progress: ");
			lcd->print(octoprintValues->progress.progress);
			lcd->print("%    ");
			//long x, long in_min, long in_max, long out_min, long out_max
			uint8_t c = map(octoprintValues->progress.progress, 0, 100, 0, 14);
			lcd->setCursor(0, 1);
			lcd->print("[");
			for (uint8_t i = 0; i < c; i++) {
				lcd->print("#");
			}
			for (uint8_t i = c; i < 14; i++) {
				lcd->print(" ");
			}
			lcd->print("]");
		} else if (displayPage == 2) {
			lcd->clear();
			lcd->home();
			uint8_t x = 0;
			if (octoprintValues->progress.progress < 100)
				x += 4;
			String progressStr = String(octoprintValues->progress.progress);
			for (uint8_t c = 0; c < progressStr.length(); c++) {
				switch(progressStr[c]) {
					case '0': printBig(x, 0); break;
					case '1': printBig(x, 1); break;
					case '2': printBig(x, 2); break;
					case '3': printBig(x, 3); break;
					case '4': printBig(x, 4); break;
					case '5': printBig(x, 5); break;
					case '6': printBig(x, 6); break;
					case '7': printBig(x, 7); break;
					case '8': printBig(x, 8); break;
					case '9': printBig(x, 9); break;
				}
				x += 4;
			}
			printBig(x, 10);
		}
		/*
		led.clear();

		if (scrollText)
			led.scrollTextLeft();

		led.drawText();
		//led.commit();

		if (displayEnable) {
			led.commit(); // commit transfers the byte buffer to the displays
		} else {
			led.clear();
			led.commit();
		}
		*/
}

LiquidCrystal_I2C display_hd44780::getLCD(void) {
	return *lcd;
}

