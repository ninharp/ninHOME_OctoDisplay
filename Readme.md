# Introduction

ninHOME OctoDisplay is a small firmware to attach a display (LCD, gLCD, OLED, ...)
to a ESP8266 Chip and act as a wireless Display for your OctoPrint controlled 3d Printers.
It is roughly based on ninHOME Node Firmware which you can find on my Repositories.

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

# Screenshots

![wifi networks](https://raw.github.com/ninharp/ninESP/master/doc/screenshots/web_networks_small.jpg "Wifi Network Settings")
![mqtt settings](https://raw.github.com/ninharp/ninESP/master/doc/screenshots/web_mqtt.jpg "MQTT Settings")

# Dependencies

The project relies on SmingFramework for ESP8266, the SmingFramework is as submodule added but not the 
esp-open-sdk, which you had to install also.

It uses a slightly modified version of the SmingFramework MQTTClient Class, cause i needed
some small additions to that.



   