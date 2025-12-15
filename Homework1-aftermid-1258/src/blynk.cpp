
/****************************************************
 * ESP32 + DHT11 + SSD1306 OLED + Button + Blynk
 ****************************************************/

#define BLYNK_TEMPLATE_ID "TMPL6ElTw83gS"
#define BLYNK_TEMPLATE_NAME "DHT11"
#define BLYNK_AUTH_TOKEN "Z51O6F5NRyBMHagbwSu5qquHPkaujzPa"

#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

// ------------ WiFi credentials ------------
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// ------------ Pins ------------
#define DHTPIN   23
#define DHTTYPE  DHT11
#define BUTTON_PIN 5

// ------------ OLED ------------
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;
int lastButtonState = HIGH;

// ------------ Function ------------
void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("DHT read error");
    return;
  }

  // OLED update
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Blynk Monitor");
  display.print("Temp: ");
  display.print(t);
  display.println(" C");
  display.print("Hum : ");
  display.print(h);
  display.println(" %");
  display.display();

  // Send to Blynk
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
}

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(5000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();

  int currentState = digitalRead(BUTTON_PIN);
  if (lastButtonState == HIGH && currentState == LOW) {
    sendSensorData();
  }
  lastButtonState = currentState;
}
