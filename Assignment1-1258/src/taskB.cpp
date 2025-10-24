/*
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pin Definitions ---
#define BUTTON_PIN 18
#define LED_PIN    4
#define BUZZER_PIN 13

// --- Variables ---
unsigned long pressStart = 0;
bool buttonPressed = false;
bool ledState = false;
const unsigned long LONG_PRESS_TIME = 1500; // 1.5 seconds

// --- Helper: Display message on OLED ---
void showMessage(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.print(msg);
  display.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA=21, SCL=22
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  showMessage("Ready...");
}

void loop() {
  int state = digitalRead(BUTTON_PIN);

  // Button pressed
  if (state == LOW && !buttonPressed) {
    buttonPressed = true;
    pressStart = millis();
  }

  // Button released
  if (state == HIGH && buttonPressed) {
    unsigned long pressDuration = millis() - pressStart;
    buttonPressed = false;

    if (pressDuration < LONG_PRESS_TIME) {
      // --- Short press ---
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      showMessage("Short Press:\nLED Toggled");
      Serial.println("Short press");
    } else {
      // --- Long press ---
      tone(BUZZER_PIN, 1000, 500);  // 1 kHz for 0.5 s
      showMessage("Long Press:\nBuzzer Tone");
      Serial.println("Long press");
    }
  }
}
*/  