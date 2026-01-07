
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Pin Setup ---
#define LED1 23
#define LED2 25
#define BTN_MODE 18
#define BTN_RESET 19
#define BUZZER 13

// --- PWM Configuration ---
const int LED1_CH = 0;        // PWM channel for LED1
const int LED2_CH = 1;        // PWM channel for LED2
const int LED1_FREQ = 5000;   // Frequency for LED1
const int LED2_FREQ = 4000;   // Frequency for LED2
const int LED1_RES = 8;       // Resolution (8-bit)
const int LED2_RES = 8;       // Resolution (8-bit)
// --- Variables ---
int mode = 0;
unsigned long lastPress = 0;
const int debounceDelay = 300;

// --- Function: short beep for feedback ---
void beep() {
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}

// --- OLED display function ---
void showMode(const String &text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 30);
  display.println(text);
  display.display();
}

// --- Reset button check function ---
bool resetPressed() {
  if (digitalRead(BTN_RESET) == LOW && millis() - lastPress > debounceDelay) {
    lastPress = millis();
    return true;
  }
  return false;
}

// --- Turn everything OFF ---
void turnAllOff() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(BUZZER, LOW);
  showMode("Mode 0: Both OFF");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA=21, SCL=22

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED init failed"));
    while (true);
  }
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  // --- Setup PWM for LEDs ---
  ledcSetup(LED1_CH, LED1_FREQ, LED1_RES);
  ledcSetup(LED2_CH, LED2_FREQ, LED2_RES);

  ledcAttachPin(LED1, LED1_CH);
  ledcAttachPin(LED2, LED2_CH);

  turnAllOff();
}

void loop() {
  // --- Mode Change Button ---
 if (digitalRead(BTN_MODE) == LOW && millis() - lastPress > debounceDelay) {
  delay(50); // confirm stability
  if (digitalRead(BTN_MODE) == LOW) { // still pressed
    mode = (mode + 1) % 4;
    lastPress = millis();
    beep();
    while (digitalRead(BTN_MODE) == LOW); // wait for release
  }
}


  // --- Reset Button ---
  if (resetPressed()) {
    mode = 0;
    beep();
    turnAllOff();
    return;
  }

  // --- LED Behavior ---
  switch (mode) {
    case 0: // Both OFF
      turnAllOff();
      break;

    case 1: // Alternate blink
      showMode("Mode 1: Alternate");
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      for (int t = 0; t < 30; t++) {
        if (resetPressed()) { mode = 0; turnAllOff(); return; }
        delay(10);
      }
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      for (int t = 0; t < 30; t++) {
        if (resetPressed()) { mode = 0; turnAllOff(); return; }
        delay(10);
      }
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
        if (resetPressed()) { mode = 0; turnAllOff(); return; }
        ledcWrite(LED1_CH, i);
        ledcWrite(LED2_CH, 255 - i);
        delay(5);
  }

      for (int i = 255; i >= 0; i--) {
       if (resetPressed()) { mode = 0; turnAllOff(); return; }
       ledcWrite(LED1_CH, i);
       ledcWrite(LED2_CH, 255 - i);
       delay(5);
  }
      break;

  }
}
