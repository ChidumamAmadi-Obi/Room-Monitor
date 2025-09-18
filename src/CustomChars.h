#ifndef CUSTOM_CHARS
#define CUSTOM_CHARS

byte thermometer[8] = {
  B01110,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

byte waterDroplet[8] = {
  B00000,
  B00100,
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

byte degreesSymbol[8] = {
  B00010,
  B00101,
  B00010,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte exclamation[8] = {
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B00000,
  B01110
};

byte doubleCheck[8] = {
  B00001,
  B00010,
  B10100,
  B01001,
  B00010,
  B10100,
  B01000,
  B00000
};

byte topCheckBottomX[8] = {
  B00001,
  B00010,
  B10100,
  B01000,
  B00000,
  B11011,
  B00100,
  B11011
};

byte topXBottomCheck[8] = {
  B11011,
  B00100,
  B11011,
  B00000,
  B00001,
  B00010,
  B10100,
  B01000

};

#endif
