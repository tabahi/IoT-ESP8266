
#include "ESPwebadminScales.h"



int ESPAdmin::initialize(void)
{
	consolemsg.reserve(50);
	EEPROM.begin(16384);
	WiFi.persistent(false);
	if(is_reset_pressed(reset_btn))
	{
		ESP_debug("ESP:Settings RESET");
		ESP_reset_settings();
	}
	ESP_read_settings();
	print_settings();
	ESP_debug(sys_name);
	WiFi.disconnect();
	delay(100);
	wifiscan();
	delay(100);
	if(setting[WIFI_SSID].value.length() >= 1)
	{
		ESP_mode = CLIENT_MODE;
		wifi_connected = false;
		ESP_debug("Client Mode. Conncecting to Wifi: ");
		ESP_debug(setting[WIFI_SSID].value);
		ESP_debug(setting[WIFI_PASS].value);
		delay(500);
		WiFi.mode(WIFI_STA);
		wifiMulti.addAP(setting[WIFI_SSID].value.c_str(), setting[WIFI_PASS].value.c_str());
		if(wifiMulti.run() == WL_CONNECTED) 
		{
			ESP_debug("WiFi connected");
			ESP_debug("IP address: ");
			ESP_IP = WiFi.localIP();
			ESP_IP_addresss = "";
			ESP_IP_addresss += String(ESP_IP[0]);
			ESP_IP_addresss += ".";
			ESP_IP_addresss += String(ESP_IP[1]);
			ESP_IP_addresss += ".";
			ESP_IP_addresss += String(ESP_IP[2]);
			ESP_IP_addresss += ".";
			ESP_IP_addresss += String(ESP_IP[3]);
			ESP_debug(ESP_IP_addresss);
			wifi_connected = true;
		}
	}
	else
	{
		ESP_mode = AP_MODE;
		 WiFi.mode(WIFI_AP);
		 ESP_debug("AP mode");
		//ESP_debug("AP Mode. SSID: " + setting[AP_SSID].value + "\tPASS: " + setting[AP_PASS].value);
		/* You can remove the password parameter if you want the AP to be open. */
		WiFi.disconnect();
		delay(500);
		
		WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
		
		char this_ap[30];
		strcpy(this_ap, sys_name);
		strcat(this_ap, "_");
		strcat(this_ap, getmac().c_str());
		
		ESP_debug(this_ap);
		WiFi.softAP(this_ap);
		dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
		dnsServer.start(DNS_PORT, "*", apIP);
		delay(1000);
		ESP_IP = WiFi.softAPIP();
		ESP_IP_addresss = "";
		ESP_IP_addresss += String(ESP_IP[0]);
		ESP_IP_addresss += ".";
		ESP_IP_addresss += String(ESP_IP[1]);
		ESP_IP_addresss += ".";
		ESP_IP_addresss += String(ESP_IP[2]);
		ESP_IP_addresss += ".";
		ESP_IP_addresss += String(ESP_IP[3]);
		ESP_debug(ESP_IP_addresss);
		//wifiscan();
	}
	
	// Setup MDNS responder
	if (!MDNS.begin(sys_name))
	{
	  ESP_debug("MDNS:ERROR");
	}
	else
	{
	  ESP_debug("Goto:\r\nhttp://" +String(sys_name) + ".local");
	  // Add service to MDNS-SD
	  MDNS.addService("http", "tcp", 80);
	}
	return ESP_mode;
}

void ESPAdmin::Start_HTTP_Server(void)
{
	server.on("/", std::bind(&ESPAdmin::handleRoot, this));
	server.on("/styles", std::bind(&ESPAdmin::print_css_file, this));
	server.on("/error", std::bind(&ESPAdmin::handle_error, this));
	server.on("/login", std::bind(&ESPAdmin::handle_login, this));

	server.on("/setup", std::bind(&ESPAdmin::handle_setup, this));
	server.on("/readSettings", std::bind(&ESPAdmin::handle_read_data, this));
	server.on("/readSSID", std::bind(&ESPAdmin::handle_ssid_list, this));
	server.on("/readCout", std::bind(&ESPAdmin::handle_cout, this));


	server.onNotFound(std::bind(&ESPAdmin::handleNotFound, this));
	  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
	  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);

	  //ask server to track these headers
	server.collectHeaders(headerkeys, headerkeyssize );

	server.begin();
	isHTTPserverRunning = true;
	ESP_debug("HTTP server started\n");
}

void ESPAdmin::handle(void)
{
	if((ESP_mode != AP_MODE) && ((millis() - last_try_wifi_connect) > 5000))
	{
		last_try_wifi_connect = millis();
		if(wifiMulti.run() != WL_CONNECTED)
		{
			if(wifi_connected)
			{
				wifi_connected = false;
				ESP_debug("WiFi disconnected!");
			}
		}
		else
		{
			if(!wifi_connected)
			{
				wifi_connected = true;
				ESP_debug("WiFi connected!");
				ESP_IP = WiFi.localIP();
				ESP_IP_addresss = "";
				ESP_IP_addresss += String(ESP_IP[0]);
				ESP_IP_addresss += ".";
				ESP_IP_addresss += String(ESP_IP[1]);
				ESP_IP_addresss += ".";
				ESP_IP_addresss += String(ESP_IP[2]);
				ESP_IP_addresss += ".";
				ESP_IP_addresss += String(ESP_IP[3]);
				ESP_debug(ESP_IP_addresss);
			}
		}
	}
	if(isHTTPserverRunning)
	{
		dnsServer.processNextRequest();
		delay(0);
		server.handleClient();
	}
}

void ESPAdmin::handle_setup()
{
  ESP_debug("ESP:req_setup");
  delay(0);
  yield();
  if (!is_authentified())
  {
    server.sendContent("HTTP/1.1 301 OK\r\nLocation: ./login\r\nCache-Control: no-cache\r\n\r\n");
    return;
  }
  if (server.hasArg("WIFI_SSID_un") && (server.arg("WIFI_SSID_un").length() > 0))
  {
    setting[WIFI_SSID].value = server.arg("WIFI_SSID_un");
    if (server.hasArg("WIFI_PASS"))
    {
      setting[WIFI_PASS].value = server.arg("WIFI_PASS");
    }
    else
    {
      setting[WIFI_PASS].value = "";
    }
    delay(0);
    yield();
    ESP_save_settings();
	//ESP.reset();
  }
  else if ((server.hasArg("WIFI_SSID_list")) && (server.arg("WIFI_SSID_list").length() > 0))
  {
    setting[WIFI_SSID].value = server.arg("WIFI_SSID_list");
    if (server.hasArg("WIFI_PASS"))
    {
      setting[WIFI_PASS].value = server.arg("WIFI_PASS");
    }
    else
    {
      setting[WIFI_PASS].value = "";
    }
    delay(0);
    yield();
    ESP_save_settings();
	//ESP.reset();
  }


  for (int i = 2; i < ESP_settings_size; i++)
  {
    if (server.hasArg(setting[i].name))
    {
      setting[i].value = server.arg(setting[i].name);
      ESP_save_settings();
    }
  }
  delay(0);
  yield();

  print_setup_page();
  delay(0);
  yield();
}


bool ESPAdmin::is_authentified(void) {

	last_conn_to_http = millis();
  //Serial.printf("Enter is_authentified\n");
  if (server.hasHeader("Cookie"))
  {
    //Serial.printf("Found cookie: ");
    String cookie = server.header("Cookie");
    //Serial.printf(cookie.c_str());
    //Serial.printf("\r\n");
    if (cookie.indexOf("ESPSESSIONID=1") != -1)
    {
      //Serial.println("Authentification Successful");
      return true;
    }
  }
  //Serial.printf("Authentification Failed\n");
  return false;
}

void ESPAdmin::handle_login(void)
{
  yield();
  delay(0);
  ESP_debug("ESP:handleLogin");
  if (server.hasHeader("Cookie"))
  {
    //Serial.printf("Found cookie: ");
    String cookie = server.header("Cookie");
    //Serial.printf(cookie.c_str());
    //Serial.printf("\r\n");
  }
  if (server.hasArg("DISCONNECT"))
  {
    //Serial.printf("Disconnection\n");
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: ./login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD"))
  {
    if (server.arg("USERNAME") == setting[WEB_USER].value &&  server.arg("PASSWORD") == setting[WEB_PASS].value)
    {
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);
      //Serial.printf("Log in Successful\n");
      return;
    }
    error_msg += "Wrong username/password! try again. ";
    //Serial.printf("Log in Failed\n");
  }
  delay(0);
  yield();
  print_login_page();
  yield();
}

boolean check_valid_ascii(String strr)
{
	int this_strr_len = strr.length();
	if(this_strr_len>32)
		return false;
	for(int k=0; k<this_strr_len; k++)
	{
		if((strr[k]<32) || (strr[k]>126))
		{
			return false;
		}
	}
	return true;
}

void ESPAdmin::handle_ssid_list()
{
  ESP_debug(F("ESP:req_ssid_list"));
  if(ESP_mode!=AP_MODE)
  {
	wifiscan();
  }
  String content =  "[ ";
  delay(0);
  yield();
	if (wifi_scan_count > 0)
	{
		for (int i = 0; i < (wifi_scan_count - 1); ++i)
		{
			boolean skip_this_ssid = false;
			for(int k=0; k<i; k++)
			{
				if(WiFi.SSID(k)==WiFi.SSID(i))
				{
				  skip_this_ssid = true;
				}
			}
			if((!skip_this_ssid)  &&  (check_valid_ascii(WiFi.SSID(i))))
			{
				content += F("{\"SSID\" : \"");
				content += String(WiFi.SSID(i));
				content += F("\"},");
			}
		}
	}
  content[content.length()-1] = ' ';

  content += "]";
  ESP_debug(F("\nSSID list:"));
  ESP_debug(content);
  delay(0);
  yield();
  server.send(200, F("application/json"), content);
  delay(0);
  yield();
}


void ESPAdmin::handle_read_data(void)
{
	ESP_debug("ESP:req_read_data");
	String content =  "data = [{";
	content += "\"IPAddress\" : \"";
	content += ESP_IP_addresss;
	content += "\",\"MAC\":\"";
	content += getmac();
	content += "\",\"Port\":\"";
	content += "80";
	content += "\",\"System\":\"";
	content += sys_name;
	content += "\"";
	content += ",\"Wmode\":\"";
	if (ESP_mode == 0)
	{
		content += "Access Point\"";
	}
	else
	{
		content += "Wifi Client\"";
	}
	delay(0);
	yield();
  for (int i = 0; i < ESP_settings_size; i++)
  {
    if (setting[i].value.length() > 0)
    {
		content += ", \"";
		content += setting[i].name;
		content += "\" : \"";
		content += setting[i].value;
		content += "\"";
    }
  }
  content += "}];";
  delay(0);
  yield();
  server.send(200, "application/json", content);
  delay(0);
  yield();

}


void ESPAdmin::handle_cout(void)
{
  delay(0);
  yield();
  server.send(200, "text/plain", consolemsg);
  consolemsg = "";
  delay(0);
  yield();
}



















//root page can be accessed only if authentification is ok
void ESPAdmin::handleRoot(void)
{
  ESP_debug("ESP:handleRoot\n");
  delay(0);
  yield();
  if (!is_authentified())
  {
    server.sendContent("HTTP/1.1 301 OK\r\nLocation: ./login\r\nCache-Control: no-cache\r\n\r\n");
    return;
  }
  else
  {
    server.sendContent("HTTP/1.1 301 OK\r\nLocation: ./setup\r\nCache-Control: no-cache\r\n\r\n");
    return;
  }

  delay(0);
  yield();
}

//no need authentification
void ESPAdmin::handleNotFound(void)
{
  delay(0);
  yield();
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  delay(0);
  yield();
}

void ESPAdmin::print_login_page(void)
{
  delay(0);
  yield();
  server.send(200, "text/html", login_page_file);
  delay(0);
  yield();
}


void ESPAdmin::print_setup_page(void)
{
  delay(0);
  yield();
  server.send(200, "text/html", setup_page_file);
  delay(0);
  yield();
}


void ESPAdmin::print_css_file(void)
{
  delay(0);
  yield();
  server.send(200, "text/css", css_file);
  delay(0);
  yield();
}

void ESPAdmin::handle_error(void)
{
  delay(0);
  yield();
  server.send(200, "text/html", "var msg = \"" + error_msg + "\"");
  error_msg = "";
  delay(0);
  yield();
}
































void ESPAdmin::wifiscan()
{

	wifi_scan_count = WiFi.scanNetworks();

	ESP_debug("Scanning Wifi");
	ESP_debug("Found: " + String(wifi_scan_count));

  if (wifi_scan_count > 0)
  {
    for (int i = 0; i < wifi_scan_count; ++i)
    {
		ESP_debug(WiFi.SSID(i));
		/*
      // Print SSID and RSSI for each network found
      Serial.printf("%d", i + 1);
      Serial.printf(": ");
      Serial.printf("%s", WiFi.SSID(i).c_str());
      Serial.printf(" (");
      Serial.printf("%d", WiFi.RSSI(i));
      Serial.printf(")");
      Serial.printf("%s", (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      Serial.printf("\n");
	  */
    }
  }
}

boolean ESPAdmin::is_reset_pressed(int reset_button)
{
	pinMode(reset_button, INPUT_PULLUP);
  for (int i = 0; i < 50; i++)
  {
    if (digitalRead(reset_button) == HIGH)
    {
      return false;
    }
    delay(100);
  }
  return true;
}

void ESPAdmin::ESP_reset_settings()
{
  for (int i = 0; i < ESP_settings_size; i++)
  {
    unsigned int slec = setting[i].value.length();
    unsigned int kx = 0;
    for (unsigned int k = setting[i].addr; k < (setting[i].max_size + setting[i].addr); k++)
    {
      if (kx < slec)
      {
        EEPROM.write(k, 0);
        kx++;
      }
      else if (kx == slec)
      {
        EEPROM.write(k, '\0');
        break;
      }
    }
  }
  EEPROM.commit();
  ESP_read_settings();
}


void ESPAdmin::ESP_read_settings()
{
  for (int i = 0; i < ESP_settings_size; i++)
  {
    String vals = "";
    for (unsigned int k = setting[i].addr; k < (setting[i].max_size + setting[i].addr); k++)
    {
      char chr = (char)EEPROM.read(k);
      if (chr == '\0')
      {
		if(vals.length()>0)
        setting[i].value = vals;
        break;
      }
      else
      {
        vals += chr;
      }
    }
  }
}



void ESPAdmin::ESP_save_settings()
{
  for (int i = 0; i < ESP_settings_size; i++)
  {
    unsigned int slec = setting[i].value.length();
    unsigned int kx = 0;
    for (unsigned int k = setting[i].addr; k < (setting[i].max_size + setting[i].addr); k++)
    {
      if (kx < slec)
      {
        EEPROM.write(k, setting[i].value[kx]);
        kx++;
      }
      else if (kx == slec)
      {
        EEPROM.write(k, '\0');
        break;
      }
    }
  }
  EEPROM.commit();
}

void ESPAdmin::print_settings()
{
	int ESP_settings_count = 0;
  for (int i = 0; i < ESP_settings_size; i++)
  {
    Serial.print(setting[i].name); Serial.print("\t:\t"); Serial.println(setting[i].value);
    ESP_settings_count++;
  }
  Serial.println(ESP_settings_count);
}

String ESPAdmin::getmac()
{
  return String(WiFi.macAddress());
}

void ESPAdmin::ESP_debug(String line)
{
  Serial.println(line);
  consolemsg += line;
  consolemsg += "\n";
  if (consolemsg.length() > 50)
  {
    consolemsg = consolemsg.substring(10);
  }
}