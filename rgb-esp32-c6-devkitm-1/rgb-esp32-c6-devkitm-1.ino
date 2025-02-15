#include <Adafruit_NeoPixel.h>

#define noOfPixels 1
#define rgbPin 8 //GPIO 8 As shown in datasheet / schematic of dev kit

Adafruit_NeoPixel rgbLed(noOfPixels,rgbPin, NEO_GRB + NEO_KHZ800); //pixel count, gpio, type

void setup() {
  // put your setup code here, to run once:
  rgbLed.begin();
  rgbLed.show();
  rgbLed.setPixelColor(0,rgbLed.Color(255,0,0));
  rgbLed.show();
  delay(2000);
  rgbLed.setPixelColor(0,rgbLed.Color(0,255,0));
  rgbLed.show();
  delay(2000);
  rgbLed.setPixelColor(0,rgbLed.Color(0,0,255));
  rgbLed.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  rainbow();
  delay(200);
}

void rainbow(){
  int randomR = random(255);
  int randomG = random(255);
  int randomB = random(255);
  rgbLed.setPixelColor(0,rgbLed.Color(randomR,randomG,randomB));
  rgbLed.show();
}