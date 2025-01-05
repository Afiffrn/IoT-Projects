#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN D4
#define DHTTYPE DHT11
#define triggerPin D5
#define echoPin D6
#define rainSensorPin D1  // Pin untuk sensor hujan

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "Nama Wifi";
const char *pass = "Password Wifi";
const char *host = "IP Host";

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(rainSensorPin, INPUT);  // Set pin sensor hujan sebagai input

  WiFi.begin(ssid, pass);
  Serial.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(".");
    delay(500);
  }

  Serial.println("Connected");
}

void loop()
{
  // put your main code here, to run repeatedly:
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();
  int ldr = analogRead(A0);
  int rainSensorValue = digitalRead(rainSensorPin);  // Baca nilai sensor hujan

  long duration, jaraks;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jaraks = 500 - (duration / 2) / 29.1;
  float jarak = jaraks / 100.0;

  Serial.println("Suhu : " + String(suhu));
  Serial.println("Kelembapan : " + String(kelembapan));
  Serial.println("LDR : " + String(ldr));
  Serial.println("Jarak : " + String(jarak));
  Serial.println("Sensor Hujan : " + String(rainSensorValue));  // Tampilkan nilai sensor hujan

  WiFiClient client;
  if (!client.connect(host, 80))
  {
    Serial.println("Connection Failed");
    return;
  }

  //Proses pengiriman data ka server
  String Link;
  HTTPClient http;
  Link = "http://192.168.43.165/sensor/kirimdata.php?suhu=" + String(suhu) + "&kelembapan=" + String(kelembapan) + "&ldr=" + String(ldr) + "&jarak=" + String(jarak) + "&rain=" + String(rainSensorValue);

  http.begin(client, Link);
  http.GET();

  http.end();

  delay(5000);
}
