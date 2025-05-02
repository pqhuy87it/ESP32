#include <Wire.h>
#include <Adafruit_MPU6050.h>
Adafruit_MPU6050 mpu;  // Create the sensor object
const int buttonPin = 2;
const int ledPin = 11;

const int activationTime = 3000;
const int standbyDelay = 4000;
const int alarmDuration = 10000;
const int movementThreshold = 12;

unsigned long buttonPressTime = 0;
unsigned long activationStartTime = 0;
bool alarmActive = false;
bool inStandby = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }


  Serial.println("Motion detection alarm system starting...");
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}


void loop() {
  // Button press handling and activation sequence
  if (!digitalRead(buttonPin)) {
    buttonPressTime = millis();
    Serial.println("Button pressed");
  } else if (buttonPressTime > 0 && millis() - buttonPressTime >= activationTime) {
    activationStartTime = millis();
    alarmActive = true;
    inStandby = true;  // Enter standby mode
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    Serial.println("Alarm activation sequence initiated");
    buttonPressTime = 0;
  }
  // Standby sequence (no sensor readings)
  if (inStandby && millis() - activationStartTime >= standbyDelay) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(ledPin, HIGH);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(250);
    }
    Serial.println("Detection sequence started");
    inStandby = false;  // Exit standby mode


    // Activate MPU6050 only after standby
    if (!mpu.begin()) {
      Serial.println("Failed to find MPU6050 chip");
      while (1) {
        delay(10);
      }
    }
  }
  // Motion detection and alarm (now using sensor readings)
  if (!inStandby && alarmActive) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);  // Read sensor data
    int totalAccel = sqrt(a.acceleration.x * a.acceleration.x +
                           a.acceleration.y * a.acceleration.y +
                           a.acceleration.z * a.acceleration.z);
    Serial.print("Total acceleration: ");
    Serial.println(totalAccel);

    if (totalAccel >= movementThreshold) {
      Serial.println("Motion detected!");
      unsigned long alarmStart = millis();
      while (millis() - alarmStart <= alarmDuration) {
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
      }
      Serial.println("Alarm stopped");
      alarmActive = false;  // Reset for next activation
    } else {
      Serial.println("No motion");
    }
  }
}