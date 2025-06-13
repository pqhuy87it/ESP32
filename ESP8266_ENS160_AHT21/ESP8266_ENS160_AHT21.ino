/***************************************************************************
  ENS160+AHT21 - Digital Air Quality Sensor
  
 ***************************************************************************/

#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <ScioSense_ENS160.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// I2C pins for ENS160+AHT21
#define I2C_SDA D2
#define I2C_SCL D1

// Initialize the display and sensor
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//----------------------------------------Initialize AHT20
Adafruit_AHTX0 aht;

//----------------------------------------Initialize ENS160+AHT21
ScioSense_ENS160 ens160(ENS160_I2CADDR_1); //0x53
// ScioSense_ENS160      ens160(ENS160_I2CADDR_0); //0x52

/*--------------------------------------------------------------------------
  SETUP function
  initiate sensor
 --------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);

  // Initialize I2C for ENS160+AHT21
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!aht.begin()) {
    Serial.println("Failed to find AHT sensor!");
    while (1) delay(10);
  }

  Serial.println("AHT sensor initialized.");

  Serial.println("ENS160...");

  if (!ens160.begin()) {
    Serial.println("Failed to find ENS160 sensor!");
    while (1) delay(10);
  }

  Serial.println("ENS160 sensor initialized.");

  if (ens160.available()) {
    // Print ENS160 versions
    Serial.print("\tRev: "); Serial.print(ens160.getMajorRev());
    Serial.print("."); Serial.print(ens160.getMinorRev());
    Serial.print("."); Serial.println(ens160.getBuild());

    Serial.print("\tCustom mode ");
    // ens160.initCustomMode(3);                                     // example has 3 steps, max. 20 steps possible
    
    // Step time is a multiple of 24ms and must not be smaller than 48ms
    // ens160.addCustomStep(48, 0, 0, 0, 0, 80, 80, 80, 80);         // Step 1: 48ms, no measurments, all hotplates at low temperatures 
    // ens160.addCustomStep(196, 0, 0, 0, 0, 160, 215, 215, 200);    // Step 2: 196ms, no measurments, all hotplates at medium temperatures 
    // ens160.addCustomStep(600, 1, 1, 1, 1, 250, 350, 350, 325);    // Step 3: 600ms, measurments done, all hotplates at high temperatures 
    Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
  }

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Clear display buffer
  display.clearDisplay();
  display.display();
}

/*--------------------------------------------------------------------------
  MAIN LOOP FUNCTION
  Cylce every 1000ms and perform measurement
 --------------------------------------------------------------------------*/
void loop() {
  // Read sensor data
  // Read temperature and humidity from AHT21
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  // Clear display buffer
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  // Set smaller text size for values display
  display.setTextSize(1);

  display.setCursor(5, 0);  // Position for Temperature
  display.print("Temp: ");
  display.print(temp.temperature, 1);
  display.println(" C");

  display.setCursor(5, 10);  // Position for Humidity
  display.print("Humidity: ");
  display.print(humidity.relative_humidity, 1);
  display.println(" %");

  if (ens160.available()) {
    // Give values to Air Quality Sensor.
    ens160.set_envdata(temp.temperature, humidity.relative_humidity);
    
    ens160.measure(true);
    ens160.measureRaw(true);
    // ens160.measure();

    Serial.print("AQI: ");Serial.print(ens160.getAQI());Serial.print("\t");
    Serial.print("TVOC: ");Serial.print(ens160.getTVOC());Serial.print("ppb\t");
    Serial.print("eCO2: ");Serial.print(ens160.geteCO2());Serial.println("ppm\t");

    display.setCursor(5, 20);  // Position for AQI
    display.print("AQI: ");
    display.println(ens160.getAQI(), 1);
    
    display.setCursor(5, 30); // Position for TVOC
    display.print("TVOC: ");
    display.print(ens160.getTVOC(), 1);
    display.println(" ppb");

    display.setCursor(5, 40); // Position for eCO2
    display.print("eCO2: ");
    display.print(ens160.geteCO2(), 1);
    display.println(" ppm");

    display.setCursor(5, 50); // Position for R HP0
    display.print("R HP0: ");
    display.print(ens160.getHP0(), 1);
    display.println(" Ohm");
  }

  // Update the display with the buffer contents
  display.display();
  
  delay(2000);
}
