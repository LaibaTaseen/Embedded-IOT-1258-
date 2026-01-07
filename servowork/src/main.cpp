/*
#include <ESP32Servo.h>

Servo servo;

void setup() {
  servo.setPeriodHertz(50);
  servo.attach(33);
  servo.write(0);   // closed
  delay(1500);
}

void loop() {
  servo.write(180); // open
  delay(3000);

  servo.write(0);   // close
  delay(5000);

  
}


#include <ESP32Servo.h>

// Update these pins to match your current wiring
#define SERVO1_PIN 18  
#define SERVO2_PIN 13 
#define SERVO3_PIN 33 

Servo s1, s2, s3;

void setup() {
  Serial.begin(115200);
  
  // Allow timers for ESP32
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  
  s1.setPeriodHertz(50);
  s2.setPeriodHertz(50);
  s3.setPeriodHertz(50);

  Serial.println("Starting Servo Test...");
  Serial.println("Attaching servos...");
  
  s1.attach(SERVO1_PIN, 500, 2400);
  s2.attach(SERVO2_PIN, 500, 2400);
  s3.attach(SERVO3_PIN, 500, 2400);
}

void loop() {
  // Test 1: Move to 0 degrees
  Serial.println("Moving to 0 degrees");
  s1.write(0);
  s2.write(0);
  s3.write(0);
  delay(2000); // Wait 2 seconds

  // Test 2: Move to 90 degrees
  Serial.println("Moving to 90 degrees");
  s1.write(180);
  s2.write(180);
  s3.write(180);
  delay(2000);

  // Test 3: Move to 180 degrees
  Serial.println("Moving to 180 degrees");
  s1.write(180);
  s2.write(180);
  s3.write(180);
  delay(2000);

  // Test 4: Slow Sweep (Checks for jitter during transit)
  Serial.println("Starting Slow Sweep...");
  for (int pos = 180; pos >= 0; pos -= 1) {
    s1.write(pos);
    s2.write(pos);
    s3.write(pos);
    delay(20); // Adjust speed of sweep
  }
  
  Serial.println("Test Cycle Complete. Resting for 3 seconds...");
  delay(3000);
}

*/
#include <ESP32Servo.h>

Servo servo1, servo2, servo3;

void setup() {
  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);
  servo3.setPeriodHertz(50);

  servo1.attach(18, 1000, 2000);
  servo2.attach(13, 1000, 2000);
  servo3.attach(33, 1000, 2000);

  delay(1000);

  servo1.write(90);
  delay(3000);
  servo1.write(0);
  delay(2000);

  servo2.write(90);
  delay(3000);
  servo2.write(0);
  delay(2000);

  servo3.write(90);
  delay(3000);
  servo3.write(0);
}

void loop() {}
