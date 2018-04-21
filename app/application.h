/**
 * Project: ninHOME_OctoDisplay
 * @file application.h
 * @author Michael Sauer <sauer.uetersen@gmail.com>
 * @date 17.04.2018
 * @based ninHOME_Node
 *
 * Main Application Header file
 *
 */
#ifndef APP_APPLICATION_H_
#define APP_APPLICATION_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <AppSettings.h>
#include <Libraries/Adafruit_ST7735/Adafruit_ST7735.h>
#include <Libraries/Adafruit_GFX/Adafruit_GFX.h>
#include <ninMQTTClient.h>
#include <system/include/debug_progmem.h>

#include <octoprint.h>
#include <disp_hd44780.h>
#include <app_defaults.h>
#include <webinterface.h>

void connectOk(IPAddress ip, IPAddress mask, IPAddress gateway);
void connectFail(String ssid, uint8_t ssidLength, uint8_t *bssid, uint8_t reason);
void onMQTTMessageReceived(String topic, String message);
void startServices();

const String app_version = String(VER_MAJOR) + "." +String(VER_MINOR) + " build " + String(VER_BUILD);

#endif /* APP_APPLICATION_H_ */
