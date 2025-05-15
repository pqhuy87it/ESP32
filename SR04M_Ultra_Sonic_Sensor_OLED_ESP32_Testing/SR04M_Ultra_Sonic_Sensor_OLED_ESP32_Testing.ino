#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define echoPin 14
#define trigPin 27
long duration;
int distance;
boolean DEBUG_MODE = 1;

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  Serial.println("\nProject Started");
  delay(1000);
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);   // Sets the echoPin as an INPUT
}
void loop() {
  Ultra();
  update_OLED();
  delay(1000);
}

void Ultra() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 20000);
  distance = duration * 0.034 / 2;
  debugPrint("\nDistance: " + String(distance));
  delay(10);
}

void update_OLED() {
  display.setTextSize(2);
  display.clearDisplay();
  display.setCursor(15, 5);
  display.print("Distance");
  display.setTextSize(3);
  display.setCursor(20, 35);
  display.print(distance);
  display.print("cm");
  display.display();
}
void debugPrint(String text) {
  if (DEBUG_MODE == 1)
    Serial.println(text);
}
