#include <Adafruit_NeoPixel.h>

#define LED_PIN 8  // The ESP32-C6 pin connected to the built-in RGB LED
#define NUM_LEDS 1

Adafruit_NeoPixel rgbLed(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

struct RGB {
    uint8_t r, g, b;
};

constexpr RGB COLOR_OFF = {0, 0, 0};
constexpr RGB COLOR_BLUE = {0, 0, 255};
// ... Add more color definitions here

void setup() {
    rgbLed.begin();
    rgbLed.show();
}

void setColor(const RGB& color) {
    rgbLed.setPixelColor(0, rgbLed.Color(color.r, color.g, color.b));
    rgbLed.show();
}

void loop() {
    setColor(COLOR_BLUE);
    delay(500);
    setColor(COLOR_OFF);
    delay(500);
}