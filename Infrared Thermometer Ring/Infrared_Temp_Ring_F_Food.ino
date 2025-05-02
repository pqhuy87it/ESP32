#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
static const unsigned char PROGMEM image_weather_temperature_f_bits[] = {0x1c,0x00,0x22,0x02,0x2b,0x05,0x2a,0x02,0x2b,0x78,0x2a,0x40,0x2b,0x70,0x2a,0x40,0x2a,0x40,0x49,0x00,0x9c,0x80,0xae,0x80,0xbe,0x80,0x9c,0x80,0x41,0x00,0x3e,0x00};
void setup() {
mlx.begin();
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
display.clearDisplay();
display.display();
}
void loop() {
  float foodTTemperature = mlx.readObjectTempF();
  display.clearDisplay();
  if (foodTTemperature < 122) // Put the minimum temp
{    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(6, 25);
    display.print("LOW SERVE TEMP<122F");
  } else if (foodTTemperature >= 122 && foodTTemperature <= 140) //Put the Good and to hot temp
{
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(6, 25);
    display.print("GOOD SERVE TEMP>122F");
  } else
{
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(6, 25);
    display.print("HIGH SERVE TEMP>140F");
  }
  display.drawBitmap(105, 3, image_weather_temperature_f_bits, 16, 16, 1);
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(13, 0);
  display.print(foodTTemperature,1);
  display.display();
  delay(500);  // Update every second
}