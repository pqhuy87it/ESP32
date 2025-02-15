/*
  This code controls an LED using a button. When the button is pressed,
  the LED turns on; when the button is released, the LED turns off.

  Board: ESP32 Development Board
  Component: Button Module and LED Module
*/

const int buttonPin = 23;  // Pin number for the button
const int ledPin = 15;     // Pin number for the LED

int buttonState = 0;  // Variable to hold the current state of the button

// Setup function runs once when the board starts
void setup() {
  pinMode(buttonPin, INPUT);  // Initialize buttonPin as an input pin
  pinMode(ledPin, OUTPUT);    // Initialize ledPin as an output pin

  digitalWrite(ledPin, LOW);  // Turn the LED off
}

// Main loop function runs repeatedly
void loop() {
  // Read the current state of the button
  buttonState = digitalRead(buttonPin);

  // Check if the button is pressed (LOW)
  if (buttonState == LOW) {
    Serial.println("Low Mod");
    digitalWrite(ledPin, HIGH);  // Turn the LED on
  } else {
    Serial.println("High Mod");
    digitalWrite(ledPin, LOW);  // Turn the LED off
  }
}
