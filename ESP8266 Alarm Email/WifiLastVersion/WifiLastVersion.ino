#include <SoftwareSerial.h>

#define RX_pin 10  //TX pin of ESP8266
#define TX_pin 11 //RX pin of ESP8266

SoftwareSerial ESp8266(RX_pin, TX_pin); // RX, TX

//wifi settings
#define Wifi_SSID "ABCD"
#define Wifi_PASS "123456789"


#define reciever_email_1 "service@uiiu.com"
#define reciever_email_2 "ui@ui.ui.com"




const int alarmPin = 5;
const int testPin = 6;
const int led = 13;




String g;

void setup()
{
  g.reserve(40);

  pinMode(alarmPin, INPUT);
  pinMode(testPin, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  Serial.begin(115200);
  ESp8266.begin(115200);
  delay(400);
  
  ESP8266_connect_wifi();


  Send_SMTP(reciever_email_1);
  delay(2000);
  Send_SMTP(reciever_email_2);
}

void loop()
{
  if(ESp8266.available())
  {
    Serial.write(ESp8266.read());
  }

  if(Serial.available())
  {
    ESp8266.write(Serial.read());
  }
}
