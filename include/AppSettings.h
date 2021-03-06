/**
 * Project: ninHOME_OctoDisplay
 * @file AppSettings.h
 * @author Anakod, Michael Sauer <sauer.uetersen@gmail.com>
 * @date 17.04.2018
 * @based ninHOME_Node
 *
 * This file includes all config file dependent functions
 *
 */

#include <SmingCore/SmingCore.h>
#include <app_defaults.h>

#ifndef INCLUDE_APPSETTINGS_H_
#define INCLUDE_APPSETTINGS_H_

/* Config file definitions, with leading point for security reasons */
#define APP_GLOBAL_SETTINGS_FILE ".settings.conf"
#define APP_MQTT_SETTINGS_FILE ".mqtt_settings.conf"

struct ApplicationSettingsStorage
{
	/* Network Config */
	IPAddress ip;
	IPAddress netmask;
	IPAddress gateway;
	String ssid;
	String password;
	bool dhcp = true;

	bool udp = DEFAULT_UDP;
	uint16_t udp_port = DEFAULT_UDP_PORT;

	/* Status LED Config */
	int8_t status_led_pin = 16;
	bool status_led_inv = false;

	/* MQTT Config */
	bool mqtt_enabled = false;
	String mqtt_server = DEFAULT_MQTT_SERVER;
	uint16_t mqtt_port = DEFAULT_MQTT_PORT;
	String mqtt_userid = DEFAULT_MQTT_USERID;
	String mqtt_login = DEFAULT_MQTT_LOGIN;
	String mqtt_password = DEFAULT_MQTT_PASS;
	String mqtt_topic_lwt = DEFAULT_MQTT_LWT;
	String mqtt_topic_cmd = DEFAULT_MQTT_CMD;
	String mqtt_topic_pub = DEFAULT_MQTT_PUB;

	/* Display Config */
	bool display = true; // display function enabled
	bool display_enabled = true; // displaying enabled
	displaytype_t display_type = ST7735; // display type
	bool display_backlight_on = true; // display backlight on after init

	// HD44780 Display
	String display_topic_prefix = DEFAULT_DISPLAY_TOPIC_PREFIX;
	String display_topic_enable = DEFAULT_DISPLAY_TOPIC_ENABLE;
	String display_topic_line1 = DEFAULT_DISPLAY_TOPIC_LINE1;
	String display_topic_line2 = DEFAULT_DISPLAY_TOPIC_LINE2;
	String display_topic_line3 = DEFAULT_DISPLAY_TOPIC_LINE3;
	String display_topic_line4 = DEFAULT_DISPLAY_TOPIC_LINE4;
	String display_topic_text = DEFAULT_DISPLAY_TOPIC_TEXT;
	String display_topic_clear = DEFAULT_DISPLAY_TOPIC_CLEAR;
	String display_topic_backlight = DEFAULT_DISPLAY_TOPIC_BACKLIGHT;
	String display_topic_scroll = DEFAULT_DISPLAY_TOPIC_SCROLL;
	String display_topic_blink = DEFAULT_DISPLAY_TOPIC_BLINK;

	String octoprint_topic_prefix = DEFAULT_OCTOPRINT_TOPIC_PREFIX;
	String octoprint_topic_lwt = DEFAULT_OCTOPRINT_LWT_TOPIC;
	String octoprint_temperature_topic_prefix = DEFAULT_OCTOPRINT_TEMPERATURE_TOPIC_PREFIX;
	String octoprint_progress_topic_prefix = DEFAULT_OCTOPRINT_PROGRESS_TOPIC_PREFIX;
	String octoprint_event_topic_prefix = DEFAULT_OCTOPRINT_EVENT_TOPIC_PREFIX;

	/* Key Input Config */
	bool keyinput = DEFAULT_KEYINPUT;
	bool keyinput_invert = DEFAULT_KEYINPUT_INVERT;
	int8_t keyinput_pin = DEFAULT_KEYINPUT_PIN;
	uint16_t keyinput_debounce = DEFAULT_KEYINPUT_DEBOUNCE_MS;

	void loadAll()
	{
		loadNetwork();
		loadMQTT();
	}

	void loadNetwork()
	{
		DynamicJsonBuffer jsonBuffer;
		if (existGlobal())
		{
			int size = fileGetSize(APP_GLOBAL_SETTINGS_FILE);
			char* jsonString = new char[size + 1];
			fileGetContent(APP_GLOBAL_SETTINGS_FILE, jsonString, size + 1);
			JsonObject& root = jsonBuffer.parseObject(jsonString);

			JsonObject& general = root["general"];
			status_led_pin = general["status_pin"];
			status_led_inv = general["status_inv"];

			JsonObject& network = root["network"];
			ssid = network["ssid"].asString();
			password = network["password"].asString();

			dhcp = network["dhcp"];

			ip = network["ip"].asString();
			netmask = network["netmask"].asString();
			gateway = network["gateway"].asString();

			udp = network["udp"];
			udp_port = String(network["udp_port"].asString()).toInt();

			delete[] jsonString;
		}
	}

	void loadMQTT()
	{
		DynamicJsonBuffer jsonBuffer;
		if (existMQTT())
		{
			int size = fileGetSize(APP_MQTT_SETTINGS_FILE);
			char* jsonString = new char[size + 1];
			fileGetContent(APP_MQTT_SETTINGS_FILE, jsonString, size + 1);
			JsonObject& root = jsonBuffer.parseObject(jsonString);

			JsonObject& mqtt = root["mqtt"];
			mqtt_enabled = mqtt["enabled"];
			mqtt_server = mqtt["server"].asString();
			mqtt_port = String(mqtt["port"].asString()).toInt();
			mqtt_userid = mqtt["userid"].asString();
			mqtt_login = mqtt["login"].asString();
			mqtt_password = mqtt["password"].asString();
			mqtt_topic_lwt = mqtt["lwt"].asString();
			mqtt_topic_cmd = mqtt["topic_cmd"].asString();
			mqtt_topic_pub = mqtt["topic_pub"].asString();

			delete[] jsonString;
		}
	}

	void saveNetwork()
	{
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();

		JsonObject& general = jsonBuffer.createObject();
		root["general"] = general;
		general["status_pin"] = status_led_pin;
		general["status_inv"] = status_led_inv;

		JsonObject& network = jsonBuffer.createObject();
		root["network"] = network;
		network["ssid"] = ssid.c_str();
		network["password"] = password.c_str();

		network["dhcp"] = dhcp;

		// Make copy by value for temporary string objects
		network["ip"] = ip.toString();
		network["netmask"] = netmask.toString();
		network["gateway"] = gateway.toString();

		network["udp"] = udp;
		network["udp_port"] = udp_port;

		String rootString;
		root.printTo(rootString);
		fileSetContent(APP_GLOBAL_SETTINGS_FILE, rootString);
	}

	void saveMQTT()
	{
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();

		JsonObject& mqtt = jsonBuffer.createObject();
		root["mqtt"] = mqtt;
		mqtt["enabled"] = mqtt_enabled;
		mqtt["server"] = mqtt_server;
		mqtt["port"] = mqtt_port;
		mqtt["userid"] = mqtt_userid;
		mqtt["login"] = mqtt_login;
		mqtt["password"] = mqtt_password;
		mqtt["lwt"] = mqtt_topic_lwt;
		mqtt["topic_cmd"] = mqtt_topic_cmd;
		mqtt["topic_pub"] = mqtt_topic_pub;

		String rootString;
		root.printTo(rootString);
		fileSetContent(APP_MQTT_SETTINGS_FILE, rootString);
	}

	bool existGlobal() { return fileExist(APP_GLOBAL_SETTINGS_FILE); }
	bool existMQTT() { return fileExist(APP_MQTT_SETTINGS_FILE); }
};

static ApplicationSettingsStorage AppSettings;

#endif /* INCLUDE_APPSETTINGS_H_ */
