#include <Servo.h>

// Create a servo object to control the motor
Servo myServo; 

void setup() {
  // Attaches the servo on pin 9 to the servo object
  myServo.attach(9); 
}

void loop() {
  // Sweep from 0 degrees to 180 degrees
  for (int angle = 0; angle <= 180; angle += 1) {
    myServo.write(angle);
    delay(15); // Wait for the servo to reach the position
  }

  // Sweep back from 180 degrees to 0 degrees
  for (int angle = 180; angle >= 0; angle -= 1) {
    myServo.write(angle);
    delay(15); // Wait for the servo to reach the position
  }
}