


typedef struct DishObject {
  const String DishName;
  String station;
  unsigned char SignalUPType;
  unsigned char SignalDNType;
  String Spacecraft;
  boolean LEDUPState;
  boolean LEDDNState;
  const int LedUPPin;
  const int LedDNPin;
};


#define TOTALDISHES 12
struct DishObject Dishes[TOTALDISHES] =
{
  //Arduino MEGA PINS: 
  /*const int allpins[TOTALPINS] = {
    2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
    A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15
  };*/
  //For no LED pin, write -1

  //Format:
  //Dish, ????, 0, 0, "", 0, 0, LedUPPin, LedDNPin
  {"DSS63", "????", 0, 0, "", 0, 0, 2, 3},
  {"DSS65", "????", 0, 0, "", 0, 0, 4, 5},
  {"DSS54", "????", 0, 0, "", 0, 0, 6, 7},
  {"DSS55", "????", 0, 0, "", 0, 0, 8, 9},
  {"DSS14", "????", 0, 0, "", 0, 0, -1, -1},
  {"DSS24", "????", 0, 0, "", 0, 0, -1, -1},
  {"DSS25", "????", 0, 0, "", 0, 0, -1, -1},
  {"DSS26", "????", 0, 0, "", 0, 0, -1, -1},
  {"DSS43", "????", 0, 0, "", 0, 0, 22, 23},
  {"DSS34", "????", 0, 0, "", 0, 0, 24, 25},
  {"DSS35", "????", 0, 0, "", 0, 0, -1, -1},
  {"DSS36", "????", 0, 0, "", 0, 0, -1, -1},
};



