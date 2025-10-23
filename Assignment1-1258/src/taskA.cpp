
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pin Definitions ---
#define LED1 23
#define LED2 22
#define LED3 4
#define BTN1 18
#define BTN2 19
#define BUZZER 13

// --- Variables ---
int mode = 0;
unsigned long pressStart = 0;
bool isPressed = false;
const unsigned long LONG_PRESS_TIME = 1000; // 1 sec

// --- Function Prototypes ---
void changeMode();
void displayMode();
void buzz();
void modeAction();

void setup() {
  // Initialize serial and display
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA=21, SCL=22
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // Pin setup
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);

  displayMode();
}

void loop() {
  // --- Button 1 for changing modes ---
  if (digitalRead(BTN1) == LOW && !isPressed) {
    isPressed = true;
    pressStart = millis();
  }

  if (digitalRead(BTN1) == HIGH && isPressed) {
    unsigned long pressDuration = millis() - pressStart;
    isPressed = false;
    if (pressDuration < LONG_PRESS_TIME) {
      // Short press → change mode
      changeMode();
    } else {
      // Long press → reset mode
      mode = 0;
      displayMode();
      buzz();
    }
  }

  // --- Button 2 could be for special action ---
  if (digitalRead(BTN2) == LOW) {
    buzz();
  }

  // Perform LED pattern according to current mode
  modeAction();
}

// --- Change Mode Function ---
void changeMode() {
  mode = (mode + 1) % 4; // 4 modes (0–3)
  displayMode();
  buzz();
}

// --- Display Current Mode on OLED ---
void displayMode() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.print("Mode: ");
  display.print(mode);
  display.display();
}

// --- Buzzer Feedback ---
void buzz() {
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}

// --- LED Patterns for Each Mode ---
void modeAction() {
  switch (mode) {
    case 0: // All OFF
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      break;

    case 1: // Blink All
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      delay(300);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      delay(300);
      break;

    case 2: // Chase Pattern
      digitalWrite(LED1, HIGH); delay(200);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH); delay(200);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH); delay(200);
      digitalWrite(LED3, LOW);
      break;

    case 3: // Alternate
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
      delay(300);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      delay(300);
      break;
  }
}
