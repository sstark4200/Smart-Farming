#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
int soilsensor = A0;
int sensorVal = 0;
int wet = 1023;
int dry = 0;
const char* ssid = "XXXXXXXX";
const char* password = "XXXXXXXX";
const char* serverName = "XXXXXXXX";
String apiKey = "XXXXXXXX";
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  randomSeed(analogRead(0));
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

  if ((millis() - lastTime) > timerDelay) {

    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = "api_key=" + apiKey + "&field1=" + String(random(40));
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}