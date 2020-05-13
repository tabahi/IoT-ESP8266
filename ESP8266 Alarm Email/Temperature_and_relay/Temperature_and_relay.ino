
//Using an Arduino R3 a DHT11, and Reading the results and calculating the Heat Index

#define reciever_email_1 "service@uiiu.com"
#define reciever_email_2 "ui@ui.ui.com"


#define RX_pin 10  //TX pin of ESP8266
#define TX_pin 11 //RX pin of ESP8266

#define DHTTYPE DHT11 // Tell the Duino which DHT ver you have 11 or 22
#define DHTPIN (8) // Tell the Duino that the DHT is on Analog pin 2


#include <DHT.h>// ADAfruit Library
#include <LiquidCrystal.h> // include the library code:    
#include <SoftwareSerial.h>


SoftwareSerial ESp8266(RX_pin, TX_pin); // RX, TX
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // initialize the library with the numbers of the interface pins

int in1 = 7;
int HIMAX = 100;

// Heat Index Function
// http://en.wikipedia.org/wiki/Heat_index
double heatIndex(double Temperature, double Humidity)
{
  double c1 = -42.38, c2 = 2.049, c3 = 10.14, c4 = -0.2248, c5 = -6.838e-3, c6 = -5.482e-2, c7 = 1.228e-3, c8 = 8.528e-4, c9 = -1.99e-6 ;
  double T = Temperature;// Your outside Temp sensor reading
  double R = Humidity;// Your Outside Humidity sensor reading
  double T2 = T * T;
  double R2 = R * R;
  double TR = T * R;
  double rv = c1 + c2 * T + c3 * R + c4 * T * R + c5 * T2 + c6 * R2 + c7 * T * TR + c8 * TR * R + c9 * T2 * R2;
  return rv;
}
void setup() {

  lcd.begin(16, 2);
  lcd.setCursor(4, 0);
  lcd.print("Vazarelle");
  delay(2000);
  lcd.clear();
  lcd.print("Booting");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(500);
  lcd.print(".");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Getting Data...");
  delay(2000);
  lcd.clear();

  Serial.print("BOOTING!");
  delay(500);
  Serial.print(".");
  Serial.println("");
  Serial.println ("getting Data..");
  delay(200);// makes it fun looking :-)
  pinMode(in1, OUTPUT);


  // put your setup code here, to run once:
  Serial.begin(9600);
  Wifi_Initialize();
  dht.begin();// Start the DHT11 sensor



}

void loop() {

  // put your main code here, to run repeatedly:
  float Humidity =    (dht.readHumidity()); //Read the DHT humidity sensor
  float Temperature = (dht.readTemperature() * 9.0 / 5.0 + 32.0); //Read the DHT Temp sensor (OutSide)convert deg C to deg F
  float HI =          (heatIndex(Temperature, Humidity));

  Serial.print("heatIndex   : ");
  Serial.println(HI);
  Serial.print("Temperature : ");
  Serial.println(Temperature, 1);
  Serial.print("Humidity    : ");
  Serial.println(Humidity, 1);
  delay(1000);//slow it down a bit so you can read it.

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.setCursor(2, 0);
  lcd.print(Temperature);

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.setCursor(2, 1);
  lcd.print(Humidity);


  lcd.setCursor(8, 0);
  lcd.print("HI:");
  lcd.setCursor(11, 0);
  lcd.print(HI);




  if (HI > HIMAX)
  {
    // if heat index is higher than Heat Index MAX
    digitalWrite(in1, HIGH);  // turn on relay

    Send_SMTP(reciever_email_1);
    delay(2000);
    Send_SMTP(reciever_email_2);
  }
  else                    // else turn off relay
    digitalWrite(in1, LOW);


  delay (10000);

}
