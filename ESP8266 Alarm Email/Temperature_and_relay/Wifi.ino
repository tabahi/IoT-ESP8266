

#define ESP8266_debug 2 //0, 1 or 2 


//STMP mail settings


//wifi settings
#define Wifi_SSID "D"
#define Wifi_PASS "123456789"




#define server_hostname "mail.smtp2go.com" //SMTP server address
#define server_PORT "2525"                    //SMTP server port
#define sender_email "w@w.com"
#define sender_pass "wwww"
#define email_subject "Heat_Index_Alert"
#define email_body "Something%20has%20happened."


//emailing API configuration - NO NEED TO CHANGE:
#define WebServerAddress "phpmailer.server.net"
#define WebServerPort "80"
#define WebAPIPathPart1 "/easymailer"
#define WebAPIPathPart2 "/PHPMailer/"
#define WebAPIPathPart3 "sendemail.php"



String g;
void Wifi_Initialize()
{
  g.reserve(40);
  ESp8266.begin(115200);
  delay(400);

  ESP8266_connect_wifi();
}





void Send_SMTP(String to_email)
{
  byte fail_count = 0;
  delay(2000);
  ESP8266_get_response(1000, 5000, "OK", "ERR");
  while (true)
  {
    ESP8266_check_busy();
    ESP_print(F("AT+CIPSTART="));
    ESP_print(F("\"TCP\",\""));
    ESP_print(F(WebServerAddress));
    ESP_print("\",");
    ESP_print(WebServerPort);
    ESP_print("\r\n");
    if (ESP8266_get_response(1000, 5000, "OK", "ERR") == true)
    {
      fail_count = 0;
      break;
    }
    else
    {
      break;
      fail_count++;
      delay(2000);
      if (fail_count > 5)
      {
        ESP8266_connect_wifi();
        fail_count = 0;
      }
    }
  }

  delay(4000);
  ESP8266_get_response(1000, 5000, "OK", "ERR");


  int lenCount = 1;

  g = "GET ";
  g += WebAPIPathPart1;
  lenCount += g.length();
  g = WebAPIPathPart2;
  lenCount += g.length();
  g = WebAPIPathPart3;
  lenCount += g.length();

  g = "?host=";
  g += server_hostname;
  lenCount += g.length();
  g = "&port=";
  g += server_PORT;
  lenCount += g.length();
  g = "&tls=1";
  g += "&user=";
  g += sender_email;
  lenCount += g.length();
  g = "&pass=";
  g += sender_pass;
  lenCount += g.length();
  g = "&from=";
  g += sender_email;
  lenCount += g.length();
  g = "&replyto=";
  g += sender_email;
  lenCount += g.length();
  g = "&subject=";
  g += email_subject;
  lenCount += g.length();
  g = "&body=";
  g += email_body;
  lenCount += g.length();
  g = "&toemail=";
  g += to_email;
  lenCount += g.length();

  g = F(" HTTP/1.1\r\n");
  lenCount += g.length();
  g = "Host: ";
  g += WebServerAddress; //"westermaster.netau.net";
  g += "\r\n";
  lenCount += g.length();
  g = F("User-Agent: ESP8266/1.1\r\n");
  lenCount += g.length();
  g = F("Connection: close\r\n\r\n");
  lenCount += g.length();

  Serial.println(lenCount);
  g = "";


  while (true)
  {
    Serial.println(F("Data length:"));
    ESp8266.print("AT+CIPSEND=");
    ESp8266.print(lenCount);
    ESp8266.print("\r\n");
    Serial.println(lenCount);

    if (ESP8266_get_response(1000, 5000, "OK", "ERR") == true)
    {
      fail_count = 0;
      break;
    }
    else
    {
      break;
      fail_count++;
      delay(1000);
      if (fail_count > 2)
      {
        break;
      }
    }
  }

  delay(4000);
  ESP8266_get_response(1000, 5000, "OK", "ERR");


  g = "GET ";
  g += WebAPIPathPart1;
  ESP_print(g);
  g = WebAPIPathPart2;
  ESP_print(g);
  g = WebAPIPathPart3;
  ESP_print(g);

  g = "?host=";
  g += server_hostname;
  ESP_print(g);
  g = "&port=";
  g += server_PORT;
  ESP_print(g);
  g = "&tls=1";
  g += "&user=";
  g += sender_email;
  ESP_print(g);
  g = "&pass=";
  g += sender_pass;
  ESP_print(g);
  g = "&from=";
  g += sender_email;
  ESP_print(g);
  g = "&replyto=";
  g += sender_email;
  ESP_print(g);
  g = "&subject=";
  g += email_subject;
  ESP_print(g);
  g = "&body=";
  g += email_body;
  ESP_print(g);
  g = "&toemail=";
  g += to_email;
  ESP_print(g);

  g = F(" HTTP/1.1\r\n");
  ESP_print(g);
  g = "Host: ";
  g += WebServerAddress; //"westermaster.netau.net";
  g += "\r\n";
  ESP_print(g);
  g = F("User-Agent: ESP8266/1.1\r\n");
  ESP_print(g);
  g = F("Connection: close\r\n\r\n");
  ESP_print(g);

  delay(1000);

  ESP8266_send_cmd(F("AT+CIPCLOSE"));
  ESP8266_get_response(500, 2000, "OK", "ERR");

  delay(4000);
  ESP8266_get_response(1000, 5000, "OK", "ERR");

}


void ESP8266_connect_wifi()
{
  Serial.print(F("Connecting Wifi:\t"));
  Serial.println(F(Wifi_SSID));

  while (true)
  {
    int fail249 = 0;
    ESP8266_check_busy();

    ESP8266_send_cmd(F("AT+RST"));
    ESP8266_get_response(500, 5000, "OK", "ERR");
    ESP8266_check_busy();

    //ESP8266_send_cmd(F("AT+CWMODE=0"));
    ESP8266_get_response(500, 2000, "OK", "ERR");
    ESP8266_check_busy();

    delay(2000);

    ESP8266_send_cmd("AT+CWLAP"); //shows available networks
    ESP8266_get_response(500, 5000, "OK", "ERR");
    ESP8266_check_busy();

    delay(2000);

    String cmd = "AT+CWJAP=\"";
    cmd += F(Wifi_SSID);
    cmd += "\",\"";
    cmd += F(Wifi_PASS);
    cmd += "\"";

    ESP8266_send_cmd(cmd);
    if (ESP8266_get_response(1000, 10000, "OK", "ERR") == true)
    {
      break;
    }
    else
    {
      Serial.println(F("ERROR: Wifi Connection Failed"));
      delay(2000);
      fail249++;
      if (fail249 > 10)
      {
        break;
      }
    }
  }
  ESP8266_send_cmd(F("AT+CIFSR"));

  Serial.println(F("Wifi Connected\r\n"));
  ESP8266_check_busy();
}



void ESP8266_check_busy()
{
  boolean busy = true;
  while (busy)
  {
    ESP8266_send_cmd("AT");

    if (ESP8266_get_response(100, 1000, "busy", "ERR") == false)
    {
      busy = false;
    }
    else
    {
      delay(1000);
    }
  }
}


boolean ESP8266_get_response(unsigned long ts_min, unsigned long ts_max, String expected, String expected_error)
{
  String ss = "";
  unsigned long tpassed = 0;
  boolean exp_found = false;
  boolean exp_error_found = false;
  int lexp = expected.length();

  while ((tpassed < ts_max) && (((!exp_found) && (!exp_error_found)) || (tpassed < ts_min)) )
  {
    while (ESp8266.available())
    {
      char incHAR = ESp8266.read();
      ss += incHAR;
#if ESP8266_debug==2
      Serial.print(incHAR);
#endif
    }
    if (ss.indexOf(expected) != -1)
    {
#if ESP8266_debug==1
      Serial.println("ESP OK:");
      Serial.println(ss);
#endif
      ss = "";
      exp_found = true;
    }
    else if ((ss.indexOf("ERR") != -1) || (ss.indexOf("FAIL") != -1) || (ss.indexOf(expected_error) != -1))
    {
#if ESP8266_debug==1
      Serial.println(F("ESP Error:"));
      Serial.println(ss);
#endif
      ss = "";
      exp_error_found = true;
    }
    else if (ss.length() > (lexp * 2))
    {
#if ESP8266_debug==1
      Serial.println("ESP ??:");
      Serial.println(ss.substring(0, lexp));
#endif
      ss = ss.substring(lexp - 1);
    }
    tpassed = tpassed + 10;
    delay(10);
  }

  if (ss.length() > 0)
  {
#if ESP8266_debug==1
    Serial.print("ESP?:");
    Serial.print(ss);
#endif
  }

  if (exp_found)
    return true;
  else
    return false;
}




void ESP8266_send_cmd(String cmd)
{
  ESp8266.print(cmd);
  ESp8266.print("\r\n");

#if ESP8266_debug
  Serial.print("ESP_CMD: ");
  Serial.println(cmd);
#endif
}


void ESP_print(String tosend)
{
  ESp8266.print(tosend);
  Serial.print(tosend);
}
