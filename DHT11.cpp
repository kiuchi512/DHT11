#include "Arduino.h"
#include "DHT11.h"

#define TIMEOUT UINT32_MAX


dht11::dht11(uint8_t _pin){
  pin = _pin;
}

int dht11::judgeValue(int level){
  int count = 0;
  uint8_t portState = level ? _bit : 0;
  
  while((*portInputRegister(port) & _bit) == portState){
    if(count++ >= Mclock){
      return TIMEOUT;
    }
  }
  return count;
}


void dht11::initialize() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  port = digitalPinToPort(pin);
  _bit = digitalPinToBitMask(pin);

  Mclock = microsecondsToClockCycles(1000);
}

void dht11::measureData() {
  // put your main code here, to run repeatedly:
  pinMode(pin, INPUT_PULLUP);
  delay(1);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delay(20);
  pinMode(pin, INPUT_PULLUP);
  delayMicroseconds(55);

  noInterrupts();
  
  if(judgeValue(LOW) == TIMEOUT){
    Serial.println("failed1");
    return;
  }

  if(judgeValue(HIGH) == TIMEOUT){
    Serial.println("failed2");
    return;
  }

  interrupts();

  
  Serial.println("succeed"); 
  
  for(i = 0; i < 40; i++){
    highCycles[i] = judgeValue(HIGH);
    lowCycles[i] = judgeValue(LOW);
  }

  for(i = 0; i < 40; i++){
    int highValue = highCycles[i];
    int lowValue = lowCycles[i];
    
    if((highValue == TIMEOUT) || (lowValue == TIMEOUT)){
      //Serial.println("failed");
    } 

    if(highValue > lowValue){
      bitData[i] = 1;
    }else{
      bitData[i] = 0;
    }
    
  }
  binaryToDecimal();

  delay(2000);
}

void dht11::printData(int iHumid, int dHumid, int iTemp, int dTemp){
  Serial.print("temperature: ");
  Serial.print(iTemp);
  Serial.print(".");
  Serial.print(dTemp);
  Serial.println("℃");
  Serial.print("humidity: ");
  Serial.print(iHumid);
  Serial.print(".");
  Serial.print(dHumid);
  Serial.println("%");
}

int dht11::binaryToDecimal(){
  // deal with data
    
    // calculate integer data of temperature
    integerHumid = bitData[0] * pow(2, 7) + bitData[1] * pow(2, 6) + bitData[2] * pow(2, 5) + bitData[3] * pow(2, 4) + bitData[4] * pow(2, 3) + bitData[5] * pow(2, 2) + bitData[6] * 2 + bitData[7];
  
    // calculate decimal data of temperature
    decimalHumid = bitData[8] * pow(2, 7) + bitData[9] * pow(2, 6) + bitData[10] * pow(2, 5) + bitData[11] * pow(2, 4) + bitData[12] * pow(2, 3) + bitData[13] * pow(2, 2) + bitData[14] * 2 + bitData[15];
  
    // calculate integer data of humidity
    integerTemp = bitData[16] * pow(2, 7) + bitData[17] * pow(2, 6) + bitData[18] * pow(2, 5) + bitData[19] * pow(2, 4) + bitData[20] * pow(2, 3) + bitData[21] * pow(2, 2) + bitData[22] * 2 + bitData[23];
  
    // calculate decimal data of humidity
    decimalTemp = bitData[24] * pow(2, 7) + bitData[25] * pow(2, 6) + bitData[26] * pow(2, 5) + bitData[27] * pow(2, 4) + bitData[28] * pow(2, 3) + bitData[29] * pow(2, 2) + bitData[30] * 2 + bitData[31];
  
    // calculate parity data 
    parity = bitData[32] * pow(2, 7) + bitData[33] * pow(2, 6) + bitData[34] * pow(2, 5) + bitData[35] * pow(2, 4) + bitData[36] * pow(2, 3) + bitData[37] * pow(2, 2) + bitData[38] * 2 + bitData[39];
  
  
    // judge data correct or incorrect
    sum = integerTemp + decimalTemp + integerHumid + decimalHumid;
  
    if(sum != parity){
      Serial.println("data is incorrect");
    } 
    temp = integerTemp + decimalTemp;
    humid = integerHumid + decimalHumid;
}

dht11::readTemperature(){
  return temp;
}

dht11::readHumidity(){
  return humid;
}