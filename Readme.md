# Introduction

ninHOME OctoDisplay is a small firmware to attach a display (LCD, gLCD, OLED, ...)
to a ESP8266 Chip and act as a wireless Monitor for your OctoPrint controlled 3d Printers.
It is roughly based on ninHOME Node Firmware which you can find also on my Repositories.

Currently it is still in work in progress state so i will add a few more displays at later time
and more configuration options, currently it is bleeding edge but already useable.

Everything should be accessible and especially configurable from a simple webinterface.
The Webinterface based on the NetworkConfig Example from the SmingFramework.

On first run it starts a Wifi Accesspoint from itself with name "ninHome OctoDisplay Setup"
which you can connect from any Wifi enabled device and then connect with a webbrowser
to "192.168.4.1" to get into the webinterface.
There you can first configure the Wifi it should connect to, the mqtt settings and the 
connected display.

Any contribution is very warm welcome...

# Currently supported Displays

* Any HD44780 compatible Display over I2C Adapter
* ST7735 Color LCD Display over Hardware SPI
* SSD1306 (work in progress)

# Screenshots

![hd44780_screen_1](https://raw.github.com/ninharp/ninHOME_OctoDisplay/master/doc/screenshots/screen_hd44780_1.jpg "HD44780 Screen 1")
![hd44780_screen_2](https://raw.github.com/ninharp/ninHOME_OctoDisplay/master/doc/screenshots/screen_hd44780_2.jpg "HD44780 Screen 2")

# Dependencies

The project relies on SmingFramework for ESP8266, the SmingFramework is as submodule added but not the 
esp-open-sdk, which you had to install also.

It uses a slightly modified version of the SmingFramework MQTTClient Class, cause i needed
some small additions to that.



   