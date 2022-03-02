#ifndef DHT11
#define DHT11

#include "Arduino.h"

class dht11 {
public:
  dht11(uint8_t _pin);
  int judgeValue(int level);
  int readTemperature();
  int readHumidity();
  void initialize();
  void measureData();
  void printData(int iHumid, int dHumid, int iTemp, int dTemp);
  int binaryToDecimal();
private:
  uint8_t pin;

  int bitData[40], highCycles[40], lowCycles[40];
  unsigned long i,integerTemp,decimalTemp,integerHumid,decimalHumid,parity,sum,Mclock,temp,humid;
  uint8_t _bit,port;

};

#endif