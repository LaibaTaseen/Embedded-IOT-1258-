
// Task B - Fixed for Wokwi/ESP32 (short vs long press)
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

const int BUZZER_CH = 0;    // PWM channel
const int BUZZER_FREQ = 2000; // 2 kHz tone
const int BUZZER_RES = 8;   // 8-bit resolution

// --- Variables ---
unsigned long pressStart = 0;
bool buttonPressed = false;
bool ledState = false;
const unsigned long LONG_PRESS_TIME = 1500; // 1.5 seconds
const unsigned long DEBOUNCE_MS = 50;

// debounce + long press control
unsigned long lastBounce = 0;
bool longHandled = false;

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
  digitalWrite(LED_PIN, LOW);

  // --- Setup buzzer PWM properly ---
  ledcSetup(BUZZER_CH, BUZZER_FREQ, BUZZER_RES);
  ledcAttachPin(BUZZER_PIN, BUZZER_CH);
  ledcWrite(BUZZER_CH, 0);  // start silent

  showMessage("Ready...");
}

void shortPressAction() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  showMessage("Short Press:\nLED Toggled");
  Serial.println("Short press");
}

void longPressAction() {
  showMessage("Long Press:\nBuzzer Tone");
  Serial.println("Long press");

  // Play 500ms tone using channel (not pin)
  ledcWrite(BUZZER_CH, 128);  // 50% duty
  delay(500);
  ledcWrite(BUZZER_CH, 0);    // stop tone
}


void loop() {
  int reading = digitalRead(BUTTON_PIN);

  // pressed
  if (reading == LOW && !buttonPressed && (millis() - lastBounce) > DEBOUNCE_MS) {
    buttonPressed = true;
    pressStart = millis();
    longHandled = false;
    lastBounce = millis();
  }

  // held down
  if (buttonPressed && !longHandled && (millis() - pressStart) >= LONG_PRESS_TIME) {
    longPressAction();
    longHandled = true;
  }

  // released
  if (reading == HIGH && buttonPressed && (millis() - lastBounce) > DEBOUNCE_MS) {
    unsigned long pressDuration = millis() - pressStart;
    buttonPressed = false;
    lastBounce = millis();

    if (!longHandled && pressDuration < LONG_PRESS_TIME) {
      shortPressAction();
    }

    longHandled = false;
  }

  delay(5);
}
