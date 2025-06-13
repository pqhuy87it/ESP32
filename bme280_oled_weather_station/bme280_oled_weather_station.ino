#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SEALEVELPRESSURE_HPA (1020.00)

// Initialize the display and sensor
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//----------------------------------------Initialize the BME280 sensor based on the communication used. Select one of the options below.
Adafruit_BME280 bme; //--> I2C

void setup() {
  Serial.begin(115200);

  // Initialize BME280 sensor
  if (!bme.begin(0x76)) {  // Try 0x77 if 0x76 doesn't work
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1);
  }

  // Initialize I2C with specific SDA (22) and SCL (21) pins
  Wire.begin(4, 5);

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Clear display buffer
  display.clearDisplay();
  display.display();
}


void loop() {
  // Read sensor data
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(1013.25);  // Adjust for your location's sea-level pressure

  // Clear display buffer
  display.clearDisplay();

  // Reduced header text size
  display.setTextSize(2);       // Smaller text size for the header
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 0);     // Center the text at the top
  display.println("DHK DUDE");

  // Set smaller text size for values display
  display.setTextSize(1); // Bigger text for values
  display.setCursor(5, 20);  // Position for Temperature
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(5, 30);  // Position for Humidity
  display.print("Humidity: ");
  display.print(humidity, 1);
  display.println(" %");

  display.setCursor(5, 40);  // Position for Pressure
  display.print("Pressure: ");
  display.print(pressure, 1);
  display.println(" hPa");


  display.setCursor(5, 50);  // Position for Pressure
  display.print("Altitude: ");
  display.print(altitude, 1);
  display.println(" m");
  // You can choose to omit altitude or display it below if needed
  // display.setCursor(10, 76);  // Position for Altitude
  // display.print("Altitude: ");
  // display.print(altitude, 1);
  // display.println(" m");

  // Update the display with the buffer contents
  display.display();

  // Wait before the next reading
  delay(2000);
}
