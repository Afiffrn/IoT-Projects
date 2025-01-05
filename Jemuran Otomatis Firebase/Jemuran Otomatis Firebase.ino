#include <Servo.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <FirebaseESP8266.h>

char ssid[] = "nama wifi";
char pass[] = "passwordnya";

#define DHTPIN D7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
byte ldr = D0;
int nilai;
const int servoPin = D4;
Servo myservo;

// Inisialisasi objek Firebase
FirebaseData fbdo;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  // Tunggu koneksi WiFi terhubung
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi, IP address: ");
  Serial.println(WiFi.localIP());

  // Inisialisasi Firebase
  Firebase.begin("Masukkan Link Firebase Projek disini", "Masukkan token Projek disini");
  Firebase.reconnectWiFi(true);

  dht.begin();
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  int rainAnalogVal = analogRead(A0);
  nilai = analogRead(ldr);

  if (t < 30 && nilai > 700 || rainAnalogVal < 1023) {
    // Putar motor servo ke posisi tertentu (misalnya  derajat)
    myservo.write(180);
  }
  else {
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

  // Upload data humidity ke Firebase
  if (Firebase.ready()) {
    if (Firebase.RTDB.setFloat(&fbdo, "DHT/kelembapan", h)) {
      Serial.print("Kelembapan: ");
      Serial.println(h);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }

  // Upload data temperature ke Firebase
  if (Firebase.ready()) {
    if (Firebase.RTDB.setFloat(&fbdo, "DHT/suhu", t)) {
      Serial.print("Suhu: ");
      Serial.println(t);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }

  if (Firebase.ready()) {
    if (Firebase.RTDB.setFloat(&fbdo, "DHT/cahaya", nilai)) {
      Serial.print("Sensor_Cahaya: ");
      Serial.println(nilai);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
  if (Firebase.ready()) {
    if (Firebase.RTDB.setFloat(&fbdo, "DHT/hujan", rainAnalogVal)) {
      Serial.print("Sensor_Hujan: ");
      Serial.println(rainAnalogVal);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }  

  Serial.println("__________");
  delay(2000);
}