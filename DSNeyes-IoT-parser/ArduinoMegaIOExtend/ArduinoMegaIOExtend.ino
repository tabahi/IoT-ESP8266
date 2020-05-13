
#define TOTALPINS 60
const int allpins[TOTALPINS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                                22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
                                A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15
                               };


String s = "";
void setup()
{
  for (int i = 0; i < TOTALPINS; i++)
  {
    pinMode(allpins[i], OUTPUT);
  }
  for (int i = 0; i < TOTALPINS; i++)
  {
    digitalWrite(allpins[i], LOW);
  }
  s.reserve(150);
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Start");
}

void loop()
{
  while (Serial1.available())
  {
    char c = Serial1.read();
    Serial.print(c);
    s += c;

    if (s.indexOf(';') > 0)
    {
      if (s.indexOf('@') >= 0)
      {
        s = s.substring(s.indexOf('@') + 1);
        if (s.indexOf(':') > 0)
        {
          int pinNo = s.substring(0, s.indexOf(':')).toInt();
          s = s.substring(s.indexOf(':')+1);
          int pinState = s.substring(0, s.indexOf(';')).toInt();
          if( (pinState==0 || pinState ==1) && validPin(pinNo))
          {
            digitalWrite(pinNo, pinState);
          }
          else
          {
            Serial.println("Invalid Pin");
          }
        }
      }
      s = "";
    }
  }
}


boolean validPin(int pinNo)
{
  for (int i = 0; i < TOTALPINS; i++)
  {
    if(allpins[i]==pinNo)
    return true;
  }
  return false;
}

