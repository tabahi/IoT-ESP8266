#include <ESPwebadmin.h>


const int ServerPort = 80;
const int ESP_reset_btn = D5;

ESPAdmin ESPAdmin(ServerPort, ESP_reset_btn);

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("Start");
  
  if (ESPAdmin.initialize() == AP_MODE)
  {
    Serial.println("Starting HTTP server");
    ESPAdmin.Start_HTTP_Server();
  }
  else if (ESPAdmin.initialize() == CLIENT_MODE)
  {
    Serial.println("Not starting HTTP server");
  }


}

void loop()
{
  if (ESPAdmin.ESP_mode == AP_MODE)
  {
    ESPAdmin.handle();
  }
  delay(0);

}
