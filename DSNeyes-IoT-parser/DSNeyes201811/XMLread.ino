
String LastStationRead = "";
String LastDishRead = "";
boolean IntrprtXML(String xmlLine)
{
  //Serial.println(xmlLine);
  if (xmlLine.indexOf("<station") >= 0)
  {
    LastStationRead = extractBtwn(xmlLine, "name=\"", "\"");
    //Serial.print("STATION:\t"); Serial.println(LastStationRead);
  }
  else if (xmlLine.indexOf("<dish") >= 0)
  {
    LastDishRead = extractBtwn(xmlLine, "name=\"", "\"");
    int dishIndex = GetDishIndex(LastDishRead);
    if (dishIndex >= 0)
    {
      Dishes[dishIndex].station = LastStationRead;
    }
  }
  else if (xmlLine.indexOf("<downSignal") >= 0)
  {
    int dishIndex = GetDishIndex(LastDishRead);
    if (dishIndex >= 0)
    {
      if ((extractBtwn(xmlLine, "signalType=\"", "\"") == "data") || (extractBtwn(xmlLine, "signalType=\"", "\"") == "carrier"))
      {
        Dishes[dishIndex].SignalDNType++;
        Dishes[dishIndex].Spacecraft += extractBtwn(xmlLine, "spacecraft=\"", "\"");
        Dishes[dishIndex].Spacecraft += "\t";
      }
    }
  }
  else if (xmlLine.indexOf("<upSignal") >= 0)
  {
    int dishIndex = GetDishIndex(LastDishRead);
    if (dishIndex >= 0)
    {
      if ((extractBtwn(xmlLine, "signalType=\"", "\"") == "data") || (extractBtwn(xmlLine, "signalType=\"", "\"") == "carrier"))
      {
        Dishes[dishIndex].station = LastStationRead;
        Dishes[dishIndex].SignalUPType++;
        Dishes[dishIndex].Spacecraft += extractBtwn(xmlLine, "spacecraft=\"", "\"");
        Dishes[dishIndex].Spacecraft += "\t";
      }
    }
  }
  else
  {
  return false;
  }
  return true;
}

String extractBtwn(String all, String first, String second)
{
  String thisall = all;
  int i1 = thisall.indexOf(first);
  if (i1 >= 0)
  {
    thisall = thisall.substring(i1 + first.length());
    int i2 = thisall.indexOf(second);
    if (i2 > 0)
    {
      thisall = thisall.substring(0 , i2);
      return thisall;
    }
    else
    {
      return thisall;
    }

  }
  return "";
}

int GetDishIndex(String DishName)
{
  for (int i = 0; i < TOTALDISHES; i++)
  {
    if (Dishes[i].DishName == DishName)
      return i;
  }
  return -1;
}

void resetDishes()
{
  Serial.println("DISHES RST");
  for (int i = 0; i < TOTALDISHES; i++)
  {
    Dishes[i].station = "?????";
    Dishes[i].SignalUPType = 0;
    Dishes[i].SignalDNType = 0;
    Dishes[i].Spacecraft = "";
  }
}

void printDishes()
{
  for (int i = 0; i < TOTALDISHES; i++)
  {
    Serial.print("Dish: "); Serial.print(Dishes[i].DishName); 
    Serial.print("\tSTA: "); Serial.print(Dishes[i].station); 
    Serial.print("\tDN: "); Serial.print(Dishes[i].SignalDNType); 
    Serial.print("\tUP: "); Serial.print(Dishes[i].SignalUPType);
    Serial.print("\tSAT: "); Serial.print(Dishes[i].Spacecraft);
    Serial.println();
  }
}

