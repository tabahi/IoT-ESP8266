#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFiMulti.h>
#include "Dishes.h"


const char* ssid = "lalala";
const char* password = "lalala420";

const char* host = "eyes.nasa.gov";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
//const char* fingerprint = "EA 71 05 58 D1 19 63 D0 DB 51 2D 93 C2 0B 01 5F 3E 11 34 E2";  //old
const char* fingerprint = "8C DA 3B 6A 86 2C 76 3B 02 29 11 53 74 7E 22 A5 D4 C2 05 7A";  //Expires in august 2019
ESP8266WiFiMulti WiFiMulti;
WiFiClientSecure client;
unsigned long xcount = 9999;

String line = "";

void setup()
{

  initLEDs();
  line.reserve(200);
  Serial.begin(115200);
  Serial.println("Hello");
  Serial.println();
  Serial.println();
  resetDishes();
  SetLEDs();

  delay(500);

  Serial.print("connecting to ");
  Serial.println(ssid);


  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

}

unsigned long OneSecTimer = 0;
unsigned long OneMinTimer = 0;
int flag = 0;

void loop()
{


  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED) && ((millis() - OneSecTimer) > 500))
  {
    Serial.print("WiFi: ");
    Serial.print(ssid);

    Serial.print("\tconnecting to ");
    Serial.print(host);
    if (!client.connect(host, httpsPort)) {
      Serial.println("\tconnection failed");
    }
    else
    {

      if (client.verify(fingerprint, host) == false)
      {
        Serial.println("\tCERT doesn't match");
      }
      else
      {
        Serial.println("\tCERT matches");

        String url = "/dsn/data/dsn.xml";
        xcount = millis() + micros();
        url += "?x=" + String(xcount);
        Serial.print("requesting URL: ");
        Serial.println(url);

        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "User-Agent: DSNStatusReader\r\n" +
                     "Connection: close\r\n\r\n");

        Serial.println("request sent");
        while (client.connected())
        {
          line = client.readStringUntil('\n');
          if (line == "\r")
          {
            Serial.println("headers rcvd");
            break;
          }
        }

        unsigned long reqStartTime = millis();
        String resp = "";
        char thisChar = '.';
        char lastChar = '.';
        flag = 0;
        while ((client.connected() && (millis() - reqStartTime) < 3000))
        {
          thisChar = client.read();
          resp += thisChar;
          if (thisChar == '<')
          {
            resp = "<";
          }


          if (resp.indexOf("<dsn>") >= 0)
          {
            resetDishes();
            resp = "";
            flag = 1;
          }

          else if (flag == 1)
          {
            if ((thisChar == '>') && ((lastChar == '/') || (lastChar == '"') || (lastChar == 'n')))
            {
              if (IntrprtXML(resp) == false)
              {
                if (resp.indexOf("</dsn>") >= 0)
                {
                  flag = 2;
                  Serial.print("REQ TIME:\t"); Serial.println(millis() - reqStartTime);
                  //Dishes[0].SignalUPType = 0;
                  //Dishes[0].SignalDNType = 0;
                  printDishes();
                  SetLEDs();
                  break;
                }
              }
              resp = "";
            }
          }
          lastChar = thisChar;
        }
        Serial.println("closing connection");
      }
      client.stop();
      client.flush();
    }
    OneSecTimer = millis();
  }
  delay(100);

}


void initLEDs()
{
  for (int i = 0; i < TOTALDISHES; i++)
  {
    if (Dishes[i].LedDNPin != -1)
    {
      //pinMode(Dishes[i].LedUPPin, OUTPUT);
      //pinMode(Dishes[i].LedDNPin, OUTPUT);
    }
  }
}

boolean vard = 0;
void SetLEDs()
{
  //vard = !vard;
  //Serial.println("PIN :  STATE");
  for (int i = 0; i < TOTALDISHES; i++)
  {
    if (Dishes[i].LedUPPin != -1)
    {
      if (Dishes[i].SignalUPType > 0)
      {
        vard = HIGH;
      }
      else
      {
        vard = LOW;
      }
      ///digitalWrite(Dishes[i].LedUPPin, vard);
      Serial.print('@'); Serial.print(Dishes[i].LedUPPin); Serial.print(':'); Serial.print(vard); Serial.println(';');
    }
    if (Dishes[i].LedDNPin != -1)
    {
      if (Dishes[i].SignalDNType > 0)
      {
        vard = HIGH;
      }
      else
      {
        vard = LOW;
      }
      //digitalWrite(Dishes[i].LedDNPin, vard);
      Serial.print('@'); Serial.print(Dishes[i].LedDNPin); Serial.print(':'); Serial.print(vard); Serial.println(';');
    }
  }

}
