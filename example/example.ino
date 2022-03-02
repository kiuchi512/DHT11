#include "DHT11.h"
#include "Arduino.h"

int temp,humid;

dht11 sensor = dht11(2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  temp = sensor.readTemperature();
  humid = sensor.readHumidity();

  Serial.println(temp);
  Serial.println(humid);

  delay(2000);

}