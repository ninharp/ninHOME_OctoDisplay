/**
 * Project: ninHOME_OctoDisplay
 * @file application.cpp
 * @author Michael Sauer <sauer.uetersen@gmail.com>
 * @date 17.04.2018
 * @based ninHOME_Node
 *
 * Main Application source file
 *
 */
#include <application.h>

extern BssList wNetworks;
extern String lastModified;

/* Forward declarations */
void startMqttClient();
void onReceiveUDP(UdpConnection& connection, char *data, int size, IPAddress remoteIP, uint16_t remotePort);
void statusLed(bool state);

// Set the LCD address to 0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

/* MQTT client instance */
/* For quickly check you can use: http://www.hivemq.com/demos/websocket-client/ (Connection= test.mosquitto.org:8080) */
ninMqttClient mqtt(DEFAULT_MQTT_SERVER, DEFAULT_MQTT_PORT, false, onMQTTMessageReceived);

/* UDP Connection instance */
UdpConnection udp = UdpConnection(onReceiveUDP);

/* Timer to check for connection */
Timer checkConnectionTimer;

Timer debounceTimer;
bool key_pressed = false;
long lastKeyPress = 0;

/* IRQ Callback for interrupt of key input */
void IRAM_ATTR keyIRQHandler()
{
	/*if (AppSettings.relay && AppSettings.keyinput) {
		noInterrupts();
		if (AppSettings.keyinput_invert) {
			if (digitalRead(AppSettings.keyinput_pin) == LOW)
				key_pressed = true;
			else
				key_pressed = false;
		} else {
			if (digitalRead(AppSettings.keyinput_pin) == HIGH)
				key_pressed = true;
			else
				key_pressed = false;
		}
		//interrupts();
	}*/
}

void serialShowInfo() {
    Serial.printf("\r\nSDK: v%s\r\n", system_get_sdk_version());
    Serial.printf("Free Heap: %d\r\n", system_get_free_heap_size());
    Serial.printf("CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
    Serial.printf("System Chip ID: %x\r\n", system_get_chip_id());
    Serial.printf("SPI Flash ID: %x\r\n", spi_flash_get_id());
    //Serial.printf("SPI Flash Size: %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
}

/* Callback for UDP Receiving */
void onReceiveUDP(UdpConnection& connection, char *data, int size, IPAddress remoteIP, uint16_t remotePort)
{
	//debugf("UDP Sever callback from %s:%d, %d bytes", remoteIP.toString().c_str(), remotePort, size);
	//udp.sendStringTo(remoteIP, remotePort, text);
}

/* Callback for messages, arrived from MQTT server */
void onMQTTMessageReceived(String topic, String message)
{
	//debugf("Debug: %s - %s", topic.c_str(), message.c_str());
}

/* Start MQTT client and publish/subscribe to the used services */
void startMqttClient()
{
	//if (mqtt.isEnabled()) { // if WifiStation.isConnected() .... TODO
		mqtt.setEnabled(true); //TODO
		/* Set LWT message and topic */
		if(!mqtt.setWill(AppSettings.mqtt_topic_lwt, "offline", 2, true)) {
			debugf("Unable to set the last will and testament. Most probably there is not enough memory on the device.");
		} else {
			debugf("MQTT LWT set");
		}

		/* Populate MQTT Settings from configuration */
		mqtt.setHost(AppSettings.mqtt_server);
		mqtt.setPort(AppSettings.mqtt_port);
		mqtt.connect(AppSettings.mqtt_userid, AppSettings.mqtt_login, AppSettings.mqtt_password);

		/* Subscribe to client command topic for general commands */
		mqtt.subscribe(AppSettings.mqtt_topic_cmd);

		/* Publish LWT message */
		mqtt.publishWithQoS(AppSettings.mqtt_topic_lwt, WifiStation.getIP().toString(), 1, true);
	//}
}

/* Start used services for connected peripherals */
void startServices()
{
	if (AppSettings.udp) {
		/* Close any udp connections */
		udp.close();
		/* Start UDP Server */
		if (AppSettings.udp_port > 0)
			udp.listen(AppSettings.udp_port);
		//TODO UDPserver: else error wrong/no udp port assigned
	}
}

void checkMQTTConnection()
{
	/* Check if MQTT Connection is still alive */
	if (mqtt.getConnectionState() != eTCS_Connected)
		startMqttClient(); /* Auto reconnect */
}

/* Start mDNS Server using ESP8266 SDK functions */
/*
void startmDNS(IPAddress ip)
{
    struct mdns_info *info = (struct mdns_info *)os_zalloc(sizeof(struct mdns_info));
    info->host_name = (char *) DEFAULT_MDNS_HOSTNAME; // You can replace test with your own host name
    info->ipAddr = ip;
    info->server_name = (char *) DEFAULT_MDNS_SERVERNAME;
    info->server_port = DEFAULT_MDNS_SERVERPORT;
    info->txt_data[0] = (char *) "version = now";
    espconn_mdns_init(info);
}

// Stop mDNS Server
void stopmDNS()
{
	espconn_mdns_close();
}
*/

/* Sets Wifi Accesspoint Settings and start it */
void startAP()
{
	/* Put on Setup LED Indicator to indicate running ap */
	statusLed(true);

	/* Start AP for configuration */
	WifiAccessPoint.enable(true);
	WifiAccessPoint.config(NINHOME_AP_NAME, "", AUTH_OPEN);

	//TODO: activate mDNS on startAP()
	/* Start mDNS server on Wifi Accesspoint IP */
	//startmDNS(WifiAccessPoint.getIP());
}

/* Stop Wifi Accesspoint if running */
void stopAP()
{
	/* If Wifi Accesspoint is enabled then disable it */
	if (WifiAccessPoint.isEnabled())
		WifiAccessPoint.enable(false);

	/* Put off Setup LED Indicator */
	statusLed(false);

	//TODO: disable mDNS server on stopAP()
	/* Stop mDNS server */
	//stopmDNS();
}

void debounceKey()
{
	if (key_pressed && (millis() - lastKeyPress) >= 500) { // check if key is pressed and last key press is ddd milliseconds away (need config value)
		if (AppSettings.keyinput_invert) {
			if (digitalRead(AppSettings.keyinput_pin) == LOW)
				key_pressed = true;
			else
				key_pressed = false;
		} else {
			if (digitalRead(AppSettings.keyinput_pin) == HIGH)
				key_pressed = true;
			else
				key_pressed = false;
		}

		if (key_pressed) {
			lastKeyPress = millis(); // reset last key press millis
			key_pressed = false; // reset key_pressed value
			interrupts(); // re-enable the interrupts
		}
	}
}

/* Callback when wifi network scan is finished */
void networkScanCompleted(bool succeeded, BssList list)
{
	/* if the network scan was successful */
	if (succeeded)
	{
		/* populate the networks list with the found networks */
			for (int i = 0; i < list.count(); i++)
					if (!list[i].hidden && list[i].ssid.length() > 0) {
							wNetworks.add(list[i]);
							debugf("Network %d = %s", i, list[i].ssid.c_str());
						}
		}
	/* sort the found networks */
	wNetworks.sort([](const BssInfo& a, const BssInfo& b){ return b.rssi - a.rssi; } );
}

/* Callback when WiFi station was connected to AP and got IP */
void connectOk(IPAddress ip, IPAddress mask, IPAddress gateway)
{
	/* Debug output of IP */
	debugf("I'm CONNECTED");
	Serial.println(ip.toString());

	AppSettings.loadMQTT();
	mqtt.setEnabled(AppSettings.mqtt_enabled);

	/* Start MQTT client and publish/subscribe used extensions */
	//if (mqtt.isEnabled()) {
	debugf("Starting MQTT Client");
	startMqttClient();
	//} else {
	//	debugf("MQTT still disabled, update settings!");
	//}

	/* Start Services for Peripherals */
	startServices();

	/*if (AppSettings.relay && AppSettings.keyinput) {
		debounceTimer.initializeMs(100, debounceKey).start();
	}*/

	/* Start timer which checks the connection to MQTT */
	checkConnectionTimer.initializeMs(DEFAULT_CONNECT_CHECK_INTERVAL, checkMQTTConnection).start();

	//stopAP(); TODO: When to stop AP? Or manually triggered in webinterface?
}

// Callback when WiFi station timeout was reached
void connectFail(String ssid, uint8_t ssidLength, uint8_t *bssid, uint8_t reason)
{
	//debugf("Disconnected from %s. Reason: %d", ssid.c_str(), reason);
	//if (reason >= 200) {//|| reason == 2) {
	//	WifiStation.disconnect();
	//	debugf("Falling Back to Setup Mode");
	//	startAP();
	//} else {
	//	debugf("Trying to reconnect to Wifi");
	//	WifiStation.connect();
	//}

	if (AppSettings.existGlobal()) {
		if (ssid.equals(AppSettings.ssid)) {
			debugf("Wifi Disconnected! Trying to reconnect...");
			WifiStation.connect();
		}
	}
}

void statusLed(bool state)
{
	if (AppSettings.status_led_pin > -1) {
		pinMode(AppSettings.status_led_pin, OUTPUT);
		if (AppSettings.status_led_inv)
			state = !state;
		digitalWrite(AppSettings.status_led_pin, state);
	}
}

bool getStatusLed()
{
	if (AppSettings.status_led_pin > -1) {
		if (AppSettings.status_led_inv)
			return !digitalRead(AppSettings.status_led_pin);
		else
			return digitalRead(AppSettings.status_led_pin);
	} else {
		return false;
	}
}

void serialCb(Stream& stream, char arrivedChar, unsigned short availableCharsCount) {

	if (arrivedChar == '\n') {
		char str[availableCharsCount];
		for (int i = 0; i < availableCharsCount; i++) {
			str[i] = stream.read();
			if (str[i] == '\r' || str[i] == '\n') {
				str[i] = '\0';
			}
		}

		if (!strcmp(str, "ip")) {
			Serial.printf("ip: %s mac: %s\r\n", WifiStation.getIP().toString().c_str(), WifiStation.getMAC().c_str());
		/*} else if (!strcmp(str, "ota")) {
			otaUpdate();
		*/} else if (!strcmp(str, "restart")) {
			System.restart();
		} else if (!strcmp(str, "ls")) {
			Vector<String> files = fileList();
			Serial.printf("filecount %d\r\n", files.count());
			for (unsigned int i = 0; i < files.count(); i++) {
				Serial.println(files[i]);
			}
		} else if (!strcmp(str, "cat")) {
			Vector<String> files = fileList();
			if (files.count() > 0) {
				Serial.printf("dumping file %s:\r\n", files[0].c_str());
				Serial.println(fileGetContent(files[0]));
			} else {
				Serial.println("Empty spiffs!");
			}
		} else if (!strcmp(str, "info")) {
			serialShowInfo();
		} else if (!strcmp(str, "help")) {
			Serial.println();
			Serial.println("available commands:");
			Serial.println("  help - display this message");
			Serial.println("  ip - show current ip address");
			Serial.println("  connect - connect to wifi");
			Serial.println("  restart - restart the esp8266");
			Serial.println("  ota - perform ota update, switch rom and reboot");
			Serial.println("  info - show esp8266 info");
#ifndef DISABLE_SPIFFS
			Serial.println("  ls - list files in spiffs");
			Serial.println("  cat - show first file in spiffs");
#endif
			Serial.println();
		} else {
			Serial.println("unknown command");
		}
	}
}

void init()
{
	/* Mount file system, in order to work with files */
	spiffs_mount();
	//int slot = rboot_get_current_rom();

	/* Start Serial Debug Terminal */
	Serial.begin(115200); // 115200 by default

	//TODO: if debug disabled it get into bootloop, inspect stacktrace
	/* Enable debug output to serial */
	Serial.systemDebugOutput(true);

	Serial.setCallback(serialCb);

/*
#ifndef DISABLE_SPIFFS
	if (slot == 0) {
#ifdef RBOOT_SPIFFS_0
		debugf("trying to mount spiffs at 0x%08x, length %d", RBOOT_SPIFFS_0, SPIFF_SIZE);
		spiffs_mount_manual(RBOOT_SPIFFS_0, SPIFF_SIZE);
#else
		debugf("trying to mount spiffs at 0x%08x, length %d", 0x100000, SPIFF_SIZE);
		spiffs_mount_manual(0x100000, SPIFF_SIZE);
#endif
	} else {
#ifdef RBOOT_SPIFFS_1
		debugf("trying to mount spiffs at 0x%08x, length %d", RBOOT_SPIFFS_1, SPIFF_SIZE);
		spiffs_mount_manual(RBOOT_SPIFFS_1, SPIFF_SIZE);
#else
		debugf("trying to mount spiffs at 0x%08x, length %d", 0x300000, SPIFF_SIZE);
		spiffs_mount_manual(0x300000, SPIFF_SIZE);
#endif
	}
#else
	debugf("spiffs disabled");
#endif

	Serial.printf("\r\nCurrently running rom %d.\r\n", slot);
	Serial.println();
*/
	/* Start initialization of ninHOME node */

	/* Load Last Modified Timestamp from the Webcontent */
	if(fileExist(".lastModified")) {
		// The last modification
		lastModified = fileGetContent(".lastModified");
		lastModified.trim();
	}

	/* Enable Wifi Client, even if none is set. Just to scan out existing networks */
    WifiStation.enable(true, false);
    WifiAccessPoint.enable(false, false);
	/* Enable Wifi Scanning and process in "networkScanCompleted" callback function */
  	WifiStation.startScan(networkScanCompleted);

	/* Check if there is already an existing configuration for wifi network */
	if (AppSettings.existGlobal())
	{
		/* Load other configuration files */
		AppSettings.loadAll();

		statusLed(false);

		/* Blink Setup LED 3 Times to indicate starting */
		for (uint8_t i = 1; i <= 6; i++) {
			statusLed(!getStatusLed());
			delay(200);
		}

		/* If there is any ssid set then assume there is a network set */
		if (AppSettings.ssid.length() > 0) {
			debugf("Settings found. Starting Station Mode");
			/* Stopping Accesspoint mode if it is running */
			stopAP();

			//TODO: nosave option wont work correctly
			/* Set the config values to the corrosponding wifistation settings and disable saving */
			WifiStation.config(AppSettings.ssid, AppSettings.password, true, false);

			/* If dhcp is disabled in settings then enable set fixed ip from settings */
			if (!AppSettings.dhcp && !AppSettings.ip.isNull())
				WifiStation.setIP(AppSettings.ip, AppSettings.netmask, AppSettings.gateway);

			/* register callbacks for GotIP and Disconnect wifi events */
			WifiEvents.onStationGotIP(connectOk);
			/* set callback that should be triggered if we are disconnected or connection attempt failed */
			WifiEvents.onStationDisconnect(connectFail);
		} else {
			/* No valid settings found, so start access point mode */
			debugf("Settings found but SSID is empty! Starting Setup Mode");
			startAP();
		}

		if (AppSettings.display_enabled) {
			// initialize the LCD
			lcd.begin(16,2);
			// Turn on the blacklight and print a message.
			//lcd.setBacklight(LOW);
			lcd.home ();                   // go home
			lcd.print("Fuck off ");
			//lcd.setCursor ( 0, 1 );        // go to the next line
			//lcd.print (" FORUM - fm   ");
			switch (AppSettings.display_type) {
				case 0:	// HD44780 16x2 Display on I2C
						break;
			}
		}
	} else {
		/* No settings file found, so start access point mode */
		debugf("No Settings found. Starting Setup Mode");
		startAP();
	}

	/* Register onReady callback to run services on system ready */
	System.onReady(startWebinterface);

}