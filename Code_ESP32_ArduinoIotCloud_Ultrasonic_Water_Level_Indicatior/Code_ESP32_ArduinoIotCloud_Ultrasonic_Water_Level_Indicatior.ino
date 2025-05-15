/**********************************************************************************
 *  TITLE: IoT-based Water Level Indicator using ESP32, Ultrasonic Sensor & Arduino IoT Cloud with 0.96" OLED
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/dqB6Vfq2Xcw
 *  Related Blog : https://iotcircuithub.com/esp32-projects/
 *  
 *  This code is provided free for project purpose and fair use only.
 *  Please do mail us to techstudycell@gmail.com if you want to use it commercially.
 *  Copyrighted © by Tech StudyCell
 *  
 *  Preferences--> Aditional boards Manager URLs : 
 *  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  
 *  Download Board ESP32 (3.0.5) : https://github.com/espressif/arduino-esp32
 *
 *  Download the libraries 
 *  ArduinoIoTCloud Library (Version 2.2.0) with all the dependencies: https://github.com/arduino-libraries/ArduinoIoTCloud
 *  Adafruit_SSD1306 Library (2.5.13): https://github.com/adafruit/Adafruit_SSD1306

 **********************************************************************************/

#include <WiFi.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <Adafruit_SSD1306.h>

// WiFi credentials
const char SSID[]     = "";        // Replace with your WiFi SSID
const char PASS[]     = "";    // Replace with your WiFi password

// Arduino IoT Cloud Device credentials
const char DEVICE_ID[] = "";  // Replace with your device ID
const char SECRET_KEY[] = "";// Replace with your secret key

// Variables to be synced with the cloud
int waterLevelPercentage;
String waterDistance;
bool buzzerState;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Cloud connection handler
WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

// GPIO Pins
#define TRIGPIN    27
#define ECHOPIN    26
#define wifiLed    2
#define ButtonPin1 12
#define BuzzerPin  13
#define GreenLed   14

// Tank distance thresholds (in cm)
const int emptyTankDistance = 70;
const int fullTankDistance = 30;

// Trigger percentage for alarm
const int triggerPer = 10;


float duration;
float distance;

// Variables for button debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // 50ms debounce delay
bool lastButtonState = HIGH;      // Button unpressed state
bool toggleBuzzer = HIGH;         // Keeps track of buzzer toggle state

void initProperties(){

  // Arduino IoT Cloud setup
  ArduinoCloud.setBoardId(DEVICE_ID);
  ArduinoCloud.setSecretDeviceKey(SECRET_KEY);
  ArduinoCloud.addProperty(waterLevelPercentage, READ, 2 * SECONDS, onWaterLevelPercentageChange);
  ArduinoCloud.addProperty(waterDistance, READ, 2 * SECONDS, NULL);
  ArduinoCloud.addProperty(buzzerState, READWRITE, ON_CHANGE, onBuzzerStateChange);
}

void doThisOnConnect(){
  /* add your custom code here */
  Serial.println("Board successfully connected to Arduino IoT Cloud");
  digitalWrite(wifiLed, HIGH); //Turn off WiFi LED
}
void doThisOnSync(){
  /* add your custom code here */
  Serial.println("Thing Properties synchronised");
}

void doThisOnDisconnect(){
  /* add your custom code here */
  Serial.println("Board disconnected from Arduino IoT Cloud");
  digitalWrite(wifiLed, LOW); //Turn off WiFi LED
}



void onWaterLevelPercentageChange() {
  Serial.print("Water Level Changed to: ");
  Serial.println(waterLevelPercentage);
}

void onBuzzerStateChange() {
  if (!buzzerState) {
    digitalWrite(BuzzerPin, LOW);
    toggleBuzzer = LOW;
  }
}

void setup() {
  // Initialize Serial
  Serial.begin(115200);

  // Pin configurations
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(wifiLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(ButtonPin1, INPUT_PULLUP);
  
  digitalWrite(wifiLed, LOW);
  digitalWrite(GreenLed, LOW);
  digitalWrite(BuzzerPin, LOW);

  // OLED Display initialization
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();

  initProperties();

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, doThisOnConnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, doThisOnSync);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, doThisOnDisconnect);
}

void loop() {
  ArduinoCloud.update();
  measureWaterLevel();
  handleBuzzerButton();
}

void measureWaterLevel() {
  // Trigger the ultrasonic sensor
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  // Measure the duration of the echo pulse
  duration = pulseIn(ECHOPIN, HIGH);
  distance = ((duration / 2) * 0.343) / 10;

  if (distance > (fullTankDistance - 10) && distance < emptyTankDistance) {
    // Calculate water level percentage
    waterLevelPercentage = map((int)distance, emptyTankDistance, fullTankDistance, 0, 100);
    waterDistance = String(distance) + " cm";

    // Update OLED display
    display.clearDisplay();
    display.setTextSize(4);
    display.setCursor(8, 2);
    display.print(waterLevelPercentage);
    display.print(" %");
    display.display();

    // Update cloud variables
    ArduinoCloud.update();

    // Manage LED and buzzer
    if (waterLevelPercentage < triggerPer) {
      digitalWrite(GreenLed, HIGH);
      if (toggleBuzzer == HIGH) {
        digitalWrite(BuzzerPin, HIGH);
        buzzerState = true;
      }
    } else {
      digitalWrite(GreenLed, LOW);
      if (toggleBuzzer == HIGH) {
        digitalWrite(BuzzerPin, HIGH);
      }
    }

    if (distance > (fullTankDistance + 5) && waterLevelPercentage > (triggerPer + 5)) {
      toggleBuzzer = HIGH;
      digitalWrite(BuzzerPin, LOW);
      buzzerState = false;
    }
  }
}

void handleBuzzerButton() {
  bool currentButtonState = digitalRead(ButtonPin1);

  if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState == LOW && toggleBuzzer == HIGH) { // Button pressed
      digitalWrite(BuzzerPin, LOW);
      toggleBuzzer = LOW;
      buzzerState = false;
      ArduinoCloud.update();
    }
  }

  lastButtonState = currentButtonState; // Update the button state
}