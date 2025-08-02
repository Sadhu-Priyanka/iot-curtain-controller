#include <Arduino.h>

// Motor A
const int motor2Pin1 = 12; 
const int motor2Pin2 = 13; 
const int enable1Pin = 14; 

// LDR Sensor
const int ldrPin = 34; // Analog pin for LDR sensor

// PWM settings
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200; // Initial duty cycle value

// Define constants for motor speed and distance
const float motorSpeed = 1; // Speed in m/s
const float distanceToTravel = 10; // Distance in meters

bool wasLight = false; // Track previous light condition

void setup() {
  // Set pins as outputs
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // LDR sensor setup
  pinMode(ldrPin, INPUT);

  // Start Serial
  Serial.begin(115200);
  Serial.println("System ready");
}

void loop() {
  // Read LDR value
  int ldrValue = analogRead(ldrPin);
  bool isLight = (ldrValue > 500); // Adjust threshold if needed

  // Move motor based on light intensity
  if (isLight && !wasLight) {
    Serial.println("Moving Forward");
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    ledcWrite(pwmChannel, dutyCycle);
    
    // Time to travel forward
    unsigned long forwardDuration = (unsigned long)(distanceToTravel / motorSpeed * 1000);
    delay(forwardDuration);
    
    // Stop motor
    Serial.println("Motor Stopped");
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    ledcWrite(pwmChannel, 0);
    delay(1000);

    dutyCycle = 200; // Reset speed
  } 
  else if (!isLight && wasLight) {
    Serial.println("Moving Backwards");
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    ledcWrite(pwmChannel, 255); // Full speed

    unsigned long backwardDuration = (unsigned long)(distanceToTravel / motorSpeed * 1000);
    delay(backwardDuration);
    
    Serial.println("Motor Stopped");
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    ledcWrite(pwmChannel, 0);
    delay(1000);
  }

  // Update light state
  wasLight = isLight;
}