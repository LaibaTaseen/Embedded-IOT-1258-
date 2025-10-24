
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pin Setup ---
#define LED1 23
#define LED2 22
#define BTN_MODE 18   // Button 1 → Change mode
#define BTN_RESET 19  // Button 2 → Reset to OFF
#define BUZZER 13

// --- Variables ---
int mode = 0;                       // 0: OFF, 1: Alternate, 2: Both ON, 3: Fade
unsigned long lastPress = 0;
const int debounceDelay = 300;

// --- Function: short beep for feedback ---
void beep() {
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}

// --- OLED display function ---
void showMode(String text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 30);
  display.println(text);
  display.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA=21, SCL=22
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED init failed"));
    while (true);
  }

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  showMode("Mode 0: Both OFF");
}

void loop() {
  // --- Mode Change Button ---
  if (digitalRead(BTN_MODE) == LOW && millis() - lastPress > debounceDelay) {
    mode = (mode + 1) % 4; // Cycle through 4 modes
    lastPress = millis();
    beep(); // short buzzer feedback
  }

  // --- Reset Button ---
  if (digitalRead(BTN_RESET) == LOW) {
    mode = 0; // Go back to OFF mode
    beep();   // feedback
  }

  // --- LED Behavior ---
  switch (mode) {
    case 0: // Both OFF
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(BUZZER, LOW);
      showMode("Mode 0: Both OFF");
      break;

    case 1: // Alternate blink
      showMode("Mode 1: Alternate");
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      delay(300);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      delay(300);
      break;

    case 2: // Both ON + Buzzer ON
      showMode("Mode 2: Both ON");
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(BUZZER, HIGH);
      break;

    case 3: // PWM fade
      showMode("Mode 3: Fade");
      digitalWrite(BUZZER, LOW);
      for (int i = 0; i <= 255; i++) {
        analogWrite(LED1, i);
        analogWrite(LED2, 255 - i);
        delay(5);
      }
      for (int i = 255; i >= 0; i--) {
        analogWrite(LED1, i);
        analogWrite(LED2, 255 - i);
        delay(5);
      }
      break;
  }
}