#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include  "webpages\\define_vars.h";

#define ESP_settings_size 6

class ESPAdmin;

enum ESP_MODES {
  AP_MODE,
  CLIENT_MODE
};

enum settingsIndex {
  WIFI_SSID,
  WIFI_PASS,
  AP_SSID,
  AP_PASS,
  WEB_USER,
  WEB_PASS
};


class SettingsObject
{
	public:
		const String name;
		String value;
		const unsigned char max_size;
		const unsigned int addr;
};





class ESPAdmin
{
	const char *sys_name;
	int reset_btn = 5;
	ESP8266WebServer server;

	
	public:
		ESPAdmin(const char *sys_namex, int port, int thisreset_btn): server(port)
		{
			sys_name = sys_namex;
			reset_btn = thisreset_btn;
		};
		IPAddress ESP_IP;
		const byte DNS_PORT = 53;
		IPAddress apIP = {192, 168, 1, 1};
		DNSServer dnsServer;
		int ESP_mode = 0;
		boolean isHTTPserverRunning = false;
		unsigned long last_conn_to_http = 0;
		SettingsObject setting[ESP_settings_size] = {{"WIFI_SSID", "", 25, 0}, 
													  {"WIFI_PASS", "", 25, 25},
													  {"AP_SSID", "APSSID", 20, 50},
													  {"AP_PASS", "appass", 20, 70},
													  {"WEB_USER", "admin", 10, 80},
													  {"WEB_PASS", "webpass", 10, 90}};
		

		boolean is_reset_pressed(int);
		void print_settings(void);
		void ESP_reset_settings(void);
		String getmac(void);
		int initialize(void);
		boolean wifi_connected = false;
		String error_msg = "";
		void handle(void);
		void Start_HTTP_Server(void);
		
		
		
	private:
		void onStationConnected(const WiFiEventSoftAPModeStationConnected& evt);
		void onStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt);
		void ESP_read_settings(void);
		void ESP_save_settings(void);
		int wifi_scan_count = 0;
		void wifiscan(void);
		String consolemsg = "";
		void ESP_debug(String);
		ESP8266WiFiMulti wifiMulti;
		String ESP_IP_addresss = "";
		unsigned long last_try_wifi_connect = 0;
		
		
		void handleRoot(void);
		void handleNotFound(void);
		void print_login_page(void);
		void print_setup_page(void);
		void print_css_file(void);
		void handle_error(void);
		bool is_authentified(void);
		
		void handle_login(void);
		void handle_setup(void);
		void handle_ssid_list(void);
		void handle_read_data(void);
		void handle_cout(void);
		
};
