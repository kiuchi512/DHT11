#include "DHT11.h"
#include "Arduino.h"

float temp,humid;
uint8_t pin = 2;

dht11 sensor = dht11(pin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  dht11::dht11_reading data = sensor.measureData();
  temp = data.temp1;
  humid = data.humid1;

  Serial.print("temperature: ");
  Serial.println(temp);
  Serial.print("humidity: ");
  Serial.println(humid);

  delay(2000);

}
