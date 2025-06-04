#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Mouse.h>
#include <Keyboard.h>
 
Adafruit_BNO055 bno = Adafruit_BNO055(55);
 
const float rotateSensitivity = 1.0;
const float panSensitivity = 1.0;
const float zoomSensitivity = 2.0;
 
bool dragging = false;
bool panning = false;
bool bnoReady = false;
 
const int touchPin = 4;
 
float zeroRoll = 0;
float zeroPitch = 0;
 
void setup() {
  Serial.begin(115200);
  Mouse.begin();
  Keyboard.begin();
 
  if (!bno.begin()) {
    Serial.println("BNO055 not found");
    while (1);
  }
 
  delay(1000);
  bno.setExtCrystalUse(true);
 
  pinMode(touchPin, INPUT_PULLUP);
 
  Serial.println("Stabilizing BNO055...");
  delay(3000); // Let sensor settle
  sensors_event_t startEvent;
  bno.getEvent(&startEvent);
  zeroRoll = startEvent.orientation.y;
  zeroPitch = startEvent.orientation.z;
  bnoReady = true;
  Serial.println("BNO055 Zeroed and Ready.");
}
 
void loop() {
  if (!bnoReady) return;
 
  sensors_event_t event;
  bno.getEvent(&event);
 
  float roll  = event.orientation.y - zeroRoll;
  float pitch = event.orientation.z - zeroPitch;
 
  bool touched = (digitalRead(touchPin) == LOW);
 
  if (touched) {
    int panMove  = (int)(-roll * panSensitivity);
    int zoomMove = (int)(-pitch * zoomSensitivity);
 
    if (abs(panMove) > 1) {
      if (!panning) {
        Mouse.press(MOUSE_MIDDLE);
        panning = true;
      }
      Mouse.move(panMove, 0, 0);
    } else if (panning) {
      Mouse.release(MOUSE_MIDDLE);
      panning = false;
    }
 
    if (abs(zoomMove) > 1) {
      Mouse.move(0, 0, zoomMove / 5);
    }
 
    if (dragging) {
      Mouse.release(MOUSE_MIDDLE);
      Keyboard.release(KEY_LEFT_SHIFT);
      dragging = false;
    }
 
  } else {
    int moveX = (int)(-pitch * rotateSensitivity);
    int moveY = (int)(-roll * rotateSensitivity);
 
    if (abs(moveX) > 1 || abs(moveY) > 1) {
      if (!dragging) {
        Keyboard.press(KEY_LEFT_SHIFT);
        Mouse.press(MOUSE_MIDDLE);
        dragging = true;
      }
      Mouse.move(moveX, moveY);
    } else if (dragging) {
      Mouse.release(MOUSE_MIDDLE);
      Keyboard.release(KEY_LEFT_SHIFT);
      dragging = false;
    }
 
    if (panning) {
      Mouse.release(MOUSE_MIDDLE);
      panning = false;
    }
  }
 
  delay(20);
}