/**
 * Project: ninHOME_OctoDisplay
 * @file app_config.h
 * @author Michael Sauer <sauer.uetersen@gmail.com>
 * @date 17.04.2018
 * @based ninHOME_Node
 * This file includes all default settings
 *
 */
#ifndef INCLUDE_APP_DEFAULTS_H_
#define INCLUDE_APP_DEFAULTS_H_

/* Version information (Note: had to be in this order to work properly at the moment, need to fix the perl script) */
#define VER_BUILD		82
#define VER_MINOR		1
#define VER_MAJOR		0

/* Default Wifi Accesspoint Name for Setup */
#define NINHOME_AP_NAME "ninHome OctoDisplay Setup"
#define NINHOME_APP_TITLE "ninHOME OctoDisplay"

/* mDNS Settings */
#define DEFAULT_MDNS_HOSTNAME "ninhome_octodisplay"
#define DEFAULT_MDNS_SERVERNAME "ninHOME_OctoDisplay"
#define DEFAULT_MDNS_SERVERPORT 80

#define DEFAULT_CONNECT_CHECK_INTERVAL 3000

/* Key debouncing intervall (100ms) */
#define DEFAULT_KEYINPUT false
#define DEFAULT_KEYINPUT_PIN 5 //D1
#define DEFAULT_KEYINPUT_DEBOUNCE_MS 500
#define DEFAULT_KEYINPUT_INVERT false

/* MQTT Settings */
#define DEFAULT_MQTT_SERVER "test.mosquitto.org"
#define DEFAULT_MQTT_PORT 1883
#define DEFAULT_MQTT_LWT "lwt/ninhome_octodisplay"
#define DEFAULT_MQTT_CMD "cmd/ninhome_octodisplay"
#define DEFAULT_MQTT_PUB "clients/ninhome_octodisplay"
#define DEFAULT_MQTT_USERID "ninHOME_OctoDisplay"
#define DEFAULT_MQTT_LOGIN "user"
#define DEFAULT_MQTT_PASS "password"

/* Display Settings */
#define DEFAULT_DISPLAY_TOPIC_PREFIX "octodisplay/"
#define DEFAULT_DISPLAY_TOPIC_ENABLE "enable"
#define DEFAULT_DISPLAY_TOPIC_LINE1 "line1"
#define DEFAULT_DISPLAY_TOPIC_LINE2 "line2"
#define DEFAULT_DISPLAY_TOPIC_LINE3 "line3"
#define DEFAULT_DISPLAY_TOPIC_LINE4 "line4"
#define DEFAULT_DISPLAY_TOPIC_CLEAR "clear"
#define DEFAULT_DISPLAY_TOPIC_BACKLIGHT "backlight"
#define DEFAULT_DISPLAY_TOPIC_SCROLL "scroll"
#define DEFAULT_DISPLAY_TOPIC_BLINK "blink"

/* FTP Settings */
#define DEFAULT_FTP_USER "octo"
#define DEFAULT_FTP_PASS "1234"
#define DEFAULT_FTP_PORT 21

/* UDP Settings */
#define DEFAULT_UDP true
#define DEFAULT_UDP_PORT 1234

/* Display Types */
typedef enum  {
	HD44780,	// HD44780 compatible LCD display
	ST7735		// ST7735 LCD display
} displaytype_t;

#endif /* INCLUDE_APP_DEFAULTS_H_ */
