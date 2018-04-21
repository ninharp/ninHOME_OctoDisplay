/**
 * Project: ninHOME_OctoDisplay
 * @file webinterface.cpp
 * @author Michael Sauer <sauer.uetersen@gmail.com>
 * @date 17.04.2018
 * @based ninHOME_Node
 *
 * All Webinterface dependend functions
 *
 */
#include "webinterface.h"

/* Web and FTP Server instance */
HttpServer server;
FTPServer ftp;

/* Timer for make connection ajax callback */
Timer connectionTimer;

const char* checked_str = "checked='checked'";

/* Contains list of available wifi networks after populating */
BssList wNetworks;

/* Current network and password for wifi connection, used in ajax callback */
String wNetwork, wPassword;

/* Last modified string for webinterface */
String lastModified;

extern ninMqttClient mqtt;

void startWebServer();
void startFTP();

/* Index target for webserver */
void onIndex(HttpRequest &request, HttpResponse &response)
{
	TemplateFileStream *tmpl = new TemplateFileStream("index.html");
	auto &vars = tmpl->variables();
	vars["lastedit"] = lastModified;

	vars["status"] = getStatusString();
	vars["version"] = app_version;
	vars["app_title"] = NINHOME_APP_TITLE;

	response.sendTemplate(tmpl); // will be automatically deleted
}

/* Update target for webserver */
void onUpdate(HttpRequest &request, HttpResponse &response)
{
	TemplateFileStream *tmpl = new TemplateFileStream("update.html");
	//rBootUpdateItem add;
	//add.targetOffset = RBOOT_SPIFFS_0;
	//add.size = 0;
	//rboot_write_status rBootWriteStatus;
	//rBootWriteStatus = rboot_write_init( add.targetOffset );


	//if(!rboot_write_flash(&rBootWriteStatus, (uint8_t *)data, size)) {
	//	debugf("rboot_write_flash: Failed. Size: %d", size);
	//}

	auto &vars = tmpl->variables();
	vars["lastedit"] = lastModified;

	vars["status"] = getStatusString();
	vars["version"] = app_version;
	vars["app_title"] = NINHOME_APP_TITLE;

	response.sendTemplate(tmpl); // will be automatically deleted
}

/* On Reboot target for webserver */
void onReboot(HttpRequest &request, HttpResponse &response)
{
	debugf("Restarting...");
	/* Disable Watchdog */
	//WDT.enable(true);
	/* Restart System */
	System.restart();
}

/* Network config target for webserver */
void onConfig(HttpRequest &request, HttpResponse &response)
{
	/* If values are passed from browser we will set here */
	if (request.method == HTTP_POST)
	{
		AppSettings.status_led_pin = request.getPostParameter("statuspin").toInt();
		AppSettings.status_led_inv = request.getPostParameter("statusinv").equals("on") ? true : false;

		AppSettings.dhcp = request.getPostParameter("dhcp") == "1";
		AppSettings.ip = request.getPostParameter("ip");
		AppSettings.netmask = request.getPostParameter("netmask");
		AppSettings.gateway = request.getPostParameter("gateway");

		AppSettings.udp = request.getPostParameter("udp").equals("on") ? true : false;;
		AppSettings.udp_port = request.getPostParameter("udpport").toInt();

		//debugf("Updating IP settings: %d", AppSettings.ip.isNull());
		AppSettings.saveNetwork();
	}

	AppSettings.loadNetwork();

	TemplateFileStream *tmpl = new TemplateFileStream("settings.html");
	auto &vars = tmpl->variables();

	vars["statuspin"] = String(AppSettings.status_led_pin);
	vars["statusinv"] = AppSettings.status_led_inv ? checked_str : "";

	bool dhcp = WifiStation.isEnabledDHCP();
	vars["dhcpon"] = dhcp ? checked_str : "";
	vars["dhcpoff"] = !dhcp ? checked_str : "";

	vars["udpon"] = AppSettings.udp ? checked_str : "";
	vars["udpport"] = String(AppSettings.udp_port);

	if (!WifiStation.getIP().isNull())
	{
		vars["ip"] = WifiStation.getIP().toString();
		vars["netmask"] = WifiStation.getNetworkMask().toString();
		vars["gateway"] = WifiStation.getNetworkGateway().toString();
	}
	else
	{
		vars["ip"] = "192.168.1.77";
		vars["netmask"] = "255.255.255.0";
		vars["gateway"] = "192.168.1.1";
	}

	vars["lastedit"] = lastModified;

	vars["status"] = getStatusString();
	vars["version"] = app_version;
	vars["app_title"] = NINHOME_APP_TITLE;

	response.sendTemplate(tmpl); // will be automatically deleted
}

/* MQTT Config target for Webserver */
void onMQTTConfig(HttpRequest &request, HttpResponse &response)
{
	//TODO: Message in webinterface, mqtt will enabled after first save
	/* If values are passed from browser we will set here */
	if (request.method == HTTP_POST)
	{
		mqtt.setEnabled(true);
		AppSettings.mqtt_server = request.getPostParameter("server");
		AppSettings.mqtt_port = request.getPostParameter("port").toInt();
		AppSettings.mqtt_userid = request.getPostParameter("userid");
		AppSettings.mqtt_login = request.getPostParameter("login");
		AppSettings.mqtt_password = request.getPostParameter("password");
		AppSettings.mqtt_topic_lwt = request.getPostParameter("topic_lwt");
		AppSettings.mqtt_topic_cmd = request.getPostParameter("topic_cmd");
		AppSettings.mqtt_topic_pub = request.getPostParameter("topic_pub");
		AppSettings.mqtt_enabled = mqtt.isEnabled();
		//debugf("Updating MQTT settings: %d", AppSettings.mqtt_server.length());
		AppSettings.saveMQTT();

	}

	TemplateFileStream *tmpl = new TemplateFileStream("mqtt.html");
	auto &vars = tmpl->variables();

	if (AppSettings.mqtt_server.length() <= 0) { vars["server"] = DEFAULT_MQTT_SERVER; } else { vars["server"] = AppSettings.mqtt_server; }
	if (AppSettings.mqtt_port > 1024) { vars["port"] = DEFAULT_MQTT_PORT; } else { vars["port"] = AppSettings.mqtt_port; }
	if (AppSettings.mqtt_login.length() <= 0) { vars["login"] = DEFAULT_MQTT_LOGIN; } else { vars["login"] = AppSettings.mqtt_login; }
	if (AppSettings.mqtt_userid.length() <= 0) { vars["userid"] = DEFAULT_MQTT_USERID; } else { vars["userid"] = AppSettings.mqtt_userid; }
	if (AppSettings.mqtt_password.length() <= 0) { vars["password"] = DEFAULT_MQTT_PASS; } else { vars["password"] = AppSettings.mqtt_password; }
	if (AppSettings.mqtt_topic_lwt.length() <= 0) { vars["topic_lwt"] = DEFAULT_MQTT_LWT; } else { vars["topic_lwt"] = AppSettings.mqtt_topic_lwt; }
	if (AppSettings.mqtt_topic_cmd.length() <= 0) { vars["topic_cmd"] = DEFAULT_MQTT_CMD; } else { vars["topic_cmd"] = AppSettings.mqtt_topic_cmd; }
	if (AppSettings.mqtt_topic_pub.length() <= 0) { vars["topic_pub"] = DEFAULT_MQTT_PUB; } else { vars["topic_pub"] = AppSettings.mqtt_topic_pub; }

	vars["lastedit"] = lastModified;

	vars["status"] = getStatusString();
	vars["version"] = app_version;
	vars["app_title"] = NINHOME_APP_TITLE;

	response.sendTemplate(tmpl); // will be automatically deleted
}

/* Default target for Webserver */
void onFile(HttpRequest &request, HttpResponse &response)
{
	if (lastModified.length() >0 && request.getHeader("If-Modified-Since").equals(lastModified)) {
		response.code = HTTP_STATUS_NOT_MODIFIED;
		return;
	}

	String file = request.getPath();
	if (file[0] == '/')
		file = file.substring(1);

	if (file[0] == '.')
		response.forbidden();
	else
	{
		if(lastModified.length() > 0) {
			response.setHeader("Last-Modified", lastModified);
		}

		response.setCache(86400, true); // It's important to use cache for better performance.
		response.sendFile(file);
	}
}

/* Debugging configuration values, dumps out the whole JSON config file which is currently active */
/* Syntax: /dumpcfg?type=<type>   where type is the type of the config file (network, mqtt, periph) */
void onDumpConfig(HttpRequest &request, HttpResponse &response)
{
	if (request.method == HTTP_GET) {
		String type = request.getQueryParameter("type");
		if (type.equals("network")) {
			int size = fileGetSize(APP_GLOBAL_SETTINGS_FILE);
			char* cfgString = new char[size + 1];
			fileGetContent(APP_GLOBAL_SETTINGS_FILE, cfgString, size + 1);

			response.sendString(cfgString);
		} else if (type.equals("mqtt")) {
			int size = fileGetSize(APP_MQTT_SETTINGS_FILE);
			char* cfgString = new char[size + 1];
			fileGetContent(APP_MQTT_SETTINGS_FILE, cfgString, size + 1);

			response.sendString(cfgString);
			delete[] cfgString;
		}
	} else {
		int size1 = fileGetSize(APP_GLOBAL_SETTINGS_FILE);
		int size2 = fileGetSize(APP_MQTT_SETTINGS_FILE);
		char* cfgString1 = new char[size1 + 1];
		char* cfgString2 = new char[size2 + 1];
		String delimiter("\r\n\r\n-\r\n\r\n");
		fileGetContent(APP_GLOBAL_SETTINGS_FILE, cfgString1, size1 + 1);
		fileGetContent(APP_MQTT_SETTINGS_FILE, cfgString2, size2 + 1);
		String temp = String(cfgString1) + delimiter + String(cfgString2);
		response.sendString(temp);
		delete[] cfgString1;
		delete[] cfgString2;
	}
}

/* Ajax target on Network List receive */
void onAjaxNetworkList(HttpRequest &request, HttpResponse &response)
{
	/* Create JSON stream */
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	json["status"] = (bool)true;

	/* Get connection state of wifi station */
	bool connected = WifiStation.isConnected();
	json["connected"] = connected;
	if (connected)
	{
		/* Copy full string to JSON buffer memory */
		json["network"]= WifiStation.getSSID();
	}

	JsonArray& netlist = json.createNestedArray("available");
	for (int i = 0; i < wNetworks.count(); i++)
	{
		if (wNetworks[i].hidden) continue;
		JsonObject &item = netlist.createNestedObject();
		item["id"] = (int)wNetworks[i].getHashId();
		/* Copy full string to JSON buffer memory */
		item["title"] = wNetworks[i].ssid;
		item["signal"] = wNetworks[i].rssi;
		item["encryption"] = wNetworks[i].getAuthorizationMethodName();
	}

	response.setAllowCrossDomainOrigin("*");

	/* Send back JSON created network list to our ajax script */
	response.sendDataStream(stream, MIME_JSON);
}


//Start connection triggered from ajax service
void makeConnection()
{
	// Enable wifistation if not already enabled
	WifiStation.enable(true);

	// Set config for wifistation from values
	WifiStation.config(wNetwork, wPassword, true, false);
	// Run our method when station was connected to AP (or not connected)
	// Set callback that should be triggered when we have assigned IP
	WifiEvents.onStationGotIP(connectOk);

	// Set callback that should be triggered if we are disconnected or connection attempt failed
	WifiEvents.onStationDisconnect(connectFail);

	// Trigger wifistation connection manual
	WifiStation.connect();

	// Write values from connection to appsettings and save to config file
	AppSettings.ssid = wNetwork;
	AppSettings.password = wPassword;
	// Save all config files from values of AppSettings
	AppSettings.saveNetwork();
	//debugf("Making Connection");

	delay(3000);

	// Set network value back to "" for task completion indication
	wNetwork = "";
}

// Ajax target to connect to wifi network
void onAjaxConnect(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	String curNet = request.getPostParameter("network");
	String curPass = request.getPostParameter("password");

	bool updating = curNet.length() > 0 && (WifiStation.getSSID() != curNet || WifiStation.getPassword() != curPass);
	bool connectingNow = WifiStation.getConnectionStatus() == eSCS_Connecting || wNetwork.length() > 0;

	if (updating && connectingNow)
	{
		debugf("wrong action: %s %s, (updating: %d, connectingNow: %d)", wNetwork.c_str(), wPassword.c_str(), updating, connectingNow);
		json["status"] = (bool)false;
		json["connected"] = (bool)false;
	}
	else
	{
		json["status"] = (bool)true;
		if (updating)
		{
			wNetwork = curNet;
			wPassword = curPass;
			//debugf("CONNECT TO: %s %s", network.c_str(), password.c_str());
			json["connected"] = false;
			connectionTimer.initializeMs(1200, makeConnection).startOnce();
		}
		else
		{
			json["connected"] = WifiStation.isConnected();
			//debugf("Network already selected. Current status: %s", WifiStation.getConnectionStatusName());
		}
	}

	if (!updating && !connectingNow && WifiStation.isConnectionFailed())
		json["error"] = WifiStation.getConnectionStatusName();

	response.setAllowCrossDomainOrigin("*");
	response.sendDataStream(stream, MIME_JSON);
}

/* Will be called when system initialization was completed */
void startWebinterface()
{
	startFTP();
	startWebServer();
}

/* Starts the implemented Webserver and add the used targets */
void startWebServer()
{
	/* Listen on Port */
	//TODO: Webserver port in configuration file?
	server.listen(80);

	/* Add targets to server */
	server.addPath("/", onIndex);
	server.addPath("/reboot", onReboot);
	server.addPath("/config", onConfig);
	server.addPath("/dumpcfg", onDumpConfig);
	server.addPath("/mqttconfig", onMQTTConfig);
	server.addPath("/ajax/get-networks", onAjaxNetworkList);
	server.addPath("/ajax/connect", onAjaxConnect);

	/* Any other targets */
	server.setDefaultHandler(onFile);
	}

/* Starts the FTP Server with fixed credentials, just for debugging purposes, had to be disabled in production state */
void startFTP()
{
	/* Check for existing index.html, else create a empty one with error message */
	if (!fileExist("index.html"))
			fileSetContent("index.html", "<h3>No SPIFFS found! Contact supplier!</h3>");

	/* Start FTP server instance */
	ftp.listen(DEFAULT_FTP_PORT);

	/* Add fixed credention ftp user account */
	ftp.addUser(DEFAULT_FTP_USER, DEFAULT_FTP_PASS); // FTP account
}

String getStatusString()
{
	String status;
	if (WifiAccessPoint.isEnabled())
		status += "Access Point ";
	if (WifiStation.isEnabled()) {
		if (WifiAccessPoint.isEnabled())
			status += "& ";
		status += "Station ";
	}
	status += "Mode - ";
	if (WifiStation.isEnabled() && WifiStation.isConnected()) {
		IPAddress ip = WifiStation.getIP();
		String ssid = WifiStation.getSSID();
		status += "Connected to " + ssid + " with IP " + ip.toString();
	}
	if (WifiAccessPoint.isEnabled()) {
		IPAddress ip = WifiAccessPoint.getIP();
		String ssid = WifiAccessPoint.getSSID();
		if (WifiStation.isEnabled())
			status += " & ";
		status += "Wifi AP '" + ssid + "' on IP " + ip.toString();
	}
	return status;
}




