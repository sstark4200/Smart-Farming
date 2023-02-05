#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
int soilsensor = A0;
int sensorVal = 0;
int wet = 1023;
int dry = 0;
void setup() {
  Serial.begin(9600);
  dht.begin();
  while (millis() < 5000) {
    sensorVal = analogRead(soilsensor);

    if (sensorVal > wet) {
      dry = sensorVal;
    }
    if (sensorVal < dry) {
      wet = sensorVal;
    }
  }


}

void loop() {
  int sensorVal = analogRead(soilsensor);
  int percentageHumididy = map(sensorVal, wet, dry, 100, 0);
  float h = dht.readHumidity();
  Serial.print(h);
  Serial.print(" , ");
  Serial.print(percentageHumididy);
  Serial.println("%");

}