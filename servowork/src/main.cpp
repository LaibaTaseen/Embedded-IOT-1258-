#include <ESP32Servo.h>

Servo servo;

void setup() {
  servo.setPeriodHertz(50);
  servo.attach(25);
  servo.write(0);   // closed
  delay(1500);
}

void loop() {
  servo.write(180); // open
  delay(3000);

  servo.write(0);   // close
  delay(5000);

  
}
