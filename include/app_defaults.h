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
#define VER_BUILD		183
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

#define DEFAULT_OCTOPRINT_TOPIC_PREFIX "octoprint/"
#define DEFAULT_OCTOPRINT_LWT_TOPIC "mqtt"
#define DEFAULT_OCTOPRINT_TEMPERATURE_TOPIC_PREFIX "temperature/"
#define DEFAULT_OCTOPRINT_PROGRESS_TOPIC_PREFIX "progress/"
#define DEFAULT_OCTOPRINT_EVENT_TOPIC_PREFIX "event/"

#define OCTOPRINT_VER13 // Octoprint Version higher than 1.3? Uncomment if not
#define OCTOPRINT_NUM_EXTRUDERS 2 // Number of connected extruders

#define OCTOPRINT_LWT_CONNECTED_STRING "connected"
#define OCTOPRINT_LWT_DISCONNECTED_STRING "disconnected"
#define OCTOPRINT_TEMP_TOOL0_STRING "tool0"
#define OCTOPRINT_TEMP_TOOL1_STRING "tool1"
#define OCTOPRINT_TEMP_TOOL2_STRING "tool2"
#define OCTOPRINT_TEMP_BED_STRING "bed"
#define OCTOPRINT_PROGRESS_PRINTING_STRING "printing"
#define OCTOPRINT_PROGRESS_SLICING_STRING "slicing"
#define OCTOPRINT_EVENT_PRINTSTARTED_STRING "PrintStarted"
#define OCTOPRINT_EVENT_CONNECTED_STRING "Connected"

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
