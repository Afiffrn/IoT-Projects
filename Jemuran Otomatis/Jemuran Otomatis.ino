#define BLYNK_TEMPLATE_ID "enter here"
#define BLYNK_TEMPLATE_NAME "enter here"
#define BLYNK_AUTH_TOKEN "enter here"
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "nama wifi";
char pass[] = "password";

#include "DHT.h"
#define DHTPIN D7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
byte ldr = D0;
int nilai;
const int servoPin = D4;
Servo myservo;

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
}
void loop() {

  Blynk.run();

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);

  int rainAnalogVal = analogRead(A0);

  nilai = analogRead(ldr);
  Blynk.virtualWrite(V3, nilai);

  if (t < 30 && nilai > 700 || rainAnalogVal < 1023) {
    // Putar motor servo ke posisi tertentu (misalnya  derajat)
    myservo.write(180);
  }
  else{
    myservo.write(0);
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F(" Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print("nilai hujan : ");
  Serial.println(rainAnalogVal);
  Serial.print("Nilai LDR: ");
  Serial.println(nilai);

  delay(2000);
  

}

