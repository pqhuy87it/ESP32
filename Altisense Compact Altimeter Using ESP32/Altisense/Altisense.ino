#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <MS5611.h> // https://github.com/jarzebski/Arduino-MS5611
#include <EEPROM.h>
#include <Arduino.h> // Required for ESP32 and IRAM_ATTR
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // See datasheet for Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MS5611 ms5611;
// Rotary Encoder Inputs - Using the working code's definitions
#define ENCODER_PIN_A D0 // D0
#define ENCODER_PIN_B D1 // D1
#define ENCODER_BUTTON_PIN D2 // D2
// EEPROM Addresses
#define EEPROM_REFERENCE_PRESSURE 0
#define EEPROM_UNITS 8
// Menu States
enum MenuState {
MAIN_PAGE,
SETTINGS_MENU,
CALIBRATION_PAGE,
UNITS_PAGE
};
MenuState currentMenuState = MAIN_PAGE;
// Calibration Variables
double currentReferencePressure;
float currentSetAltitude = 0; // User-defined altitude for calibration
// Units Variable (0 for meters, 1 for feet)
uint8_t currentUnits = 0;
// Encoder Variables - Using the working code's logic
volatile long encoderCount = 0;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;
unsigned long buttonDebounceTime = 1000; // Debounce delay for button in ms
byte buttonPressCount = 0;
// Function Prototypes
void displayMainPage();
void displaySettingsMenu(int selectedOption);
void displayCalibrationPage();
void displayUnitsPage(int selectedOption);
void readEncoder();
void readButton();
void saveCalibration();
void loadCalibration();
void saveUnits();
void loadUnits();
float calculateAltitude(double pressure);
float convertToFeet(float meters);
void setupEncoder();
void setupButton();
void setup() {
Serial.begin(115200);
// Initialize MS5611 sensor
Serial.println("Initialize MS5611 Sensor");
while (!ms5611.begin()) {
Serial.println("Could not find a valid MS5611 sensor, check wiring!");
delay(500);
}
// Initialize OLED display
if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
Serial.println(F("SSD1306 allocation failed"));
for (;;); // Don't proceed, loop forever
}
display.clearDisplay();
display.display();
delay(500);
// Initialize Encoder
setupEncoder();
// Initialize Button
setupButton();
// Load calibration and units from EEPROM
loadCalibration();
loadUnits();
// Set initial reference pressure
currentReferencePressure = ms5611.readPressure();
Serial.print("Initial Reference Pressure: ");
Serial.println(currentReferencePressure);
Serial.print("Initial Units: ");
Serial.println(currentUnits == 0 ? "Meters" : "Feet");
displayMainPage();
}
void loop() {
readButton();
readEncoder(); // Call the encoder reading function in the loop
switch (currentMenuState) {
case MAIN_PAGE:
displayMainPage();
break;
case SETTINGS_MENU: {
static int selectedSetting = 0;
if (encoderCount != 0) {
selectedSetting -= encoderCount;
if (selectedSetting < 0) selectedSetting = 2;
if (selectedSetting > 2) selectedSetting = 0;
encoderCount = 0;
displaySettingsMenu(selectedSetting);
}
if (buttonPressCount == 1) {
buttonPressCount = 0; // Reset after action
if (selectedSetting == 0) {
currentMenuState = CALIBRATION_PAGE;
currentSetAltitude = calculateAltitude(ms5611.readPressure()); // Initialize with current calculated altitude
displayCalibrationPage();
} else if (selectedSetting == 1) {
currentMenuState = UNITS_PAGE;
displayUnitsPage(currentUnits);
} else if (selectedSetting == 2) {
currentMenuState = MAIN_PAGE;
displayMainPage();
}
}
break;
}
case CALIBRATION_PAGE:
if (encoderCount != 0) {
currentSetAltitude += encoderCount; // Adjust the 'Set Altitude' value
encoderCount = 0;
displayCalibrationPage();
}
if (buttonPressCount == 1) {
buttonPressCount = 0;
// Recalculate reference pressure based on the set altitude
long currentPressure = ms5611.readPressure();
currentReferencePressure = currentPressure / pow(1.0 - (currentSetAltitude / 44330.0), 5.255);
saveCalibration();
currentMenuState = SETTINGS_MENU;
displaySettingsMenu(0);
}
break;
case UNITS_PAGE: {
static int selectedUnit = currentUnits;
if (encoderCount != 0) {
selectedUnit -= encoderCount;
if (selectedUnit < 0) selectedUnit = 1;
if (selectedUnit > 1) selectedUnit = 0;
encoderCount = 0;
displayUnitsPage(selectedUnit);
}
if (buttonPressCount == 1) {
buttonPressCount = 0;
currentUnits = selectedUnit;
saveUnits();
currentMenuState = SETTINGS_MENU;
displaySettingsMenu(1);
}
break;
}
}
delay(1); // Small delay for overall loop
}
void displayMainPage() {
double realTemperature = ms5611.readTemperature();
long realPressure = ms5611.readPressure();
float altitudeMeters = calculateAltitude(realPressure);
float altitudeDisplay = (currentUnits == 1) ? convertToFeet(altitudeMeters) : altitudeMeters;
String unitString = (currentUnits == 1) ? "ft" : "m";
display.clearDisplay();
display.setTextColor(SSD1306_WHITE);
display.setTextSize(1);
display.setCursor(3, 50);
display.print("Tmp");
display.setTextSize(2);
display.setCursor(24, 46);
display.print(realTemperature, 1);
display.print("c");
display.setTextSize(1);
display.setCursor(3, 28);
display.print("Pre");
display.setTextSize(2);
display.setTextSize(2);
display.setCursor(24, 24);
display.print((int)(realPressure / 100));
display.print("hPa");
display.setTextSize(1);
display.setCursor(3, 7);
display.print("Alt");
display.setTextSize(2);
display.setCursor(24, 3);
display.print(altitudeDisplay, 0);
display.print(unitString);
display.display();
}
void displaySettingsMenu(int selectedOption) {
display.clearDisplay();
display.setTextColor(SSD1306_WHITE);
display.setTextSize(1);
String options[] = {"Calibration", "Units", "Back"};
for (int i = 0; i < 3; i++) {
display.setCursor(0, i * 16);
if (i == selectedOption) {
display.print("> ");
} else {
display.print(" ");
}
display.println(options[i]);
}
display.display();
}
void displayCalibrationPage() {
long realPressure = ms5611.readPressure();
display.clearDisplay();
display.setTextColor(SSD1306_WHITE);
display.setTextSize(1);
display.setCursor(4, 4);
display.print("Altitude Calibration");
display.setCursor(6, 50);
display.print("Pre");
display.setTextSize(2);
display.setCursor(30, 46);
display.print((int)(realPressure / 100));
display.print("hPa");
display.setTextSize(1);
display.setCursor(6, 27);
display.print("Alt");;
display.setTextSize(2);
display.setCursor(32, 23);
display.print(currentSetAltitude, 0);
display.print("m");
display.display();
}
void displayUnitsPage(int selectedUnit) {
display.clearDisplay();
display.setTextColor(SSD1306_WHITE);
display.setTextSize(1);
display.setCursor(0, 0);
display.print("Select Units:");
display.setCursor(0, 16);
if (selectedUnit == 0) {
display.print("> Meters");
} else {
display.print(" Meters");
}
display.setCursor(0, 32);
if (selectedUnit == 1) {
display.print("> Feet");
} else {
display.print(" Feet");
}
display.display();
}
void readEncoder() {
// Rotary Encoder Inputs
int currentStateCLK = digitalRead(ENCODER_PIN_A);
// If last and current state of CLK are different, then pulse occurred
// React to only 1 state change to avoid double count
if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
// If the DT state is different than the CLK state then
// the encoder is rotating CCW so decrement
if (digitalRead(ENCODER_PIN_B) != currentStateCLK) {
encoderCount--;
currentDir = "CCW";
} else {
// Encoder is rotating CW so increment
encoderCount++;
currentDir = "CW";
}
Serial.print("Direction: ");
Serial.print(currentDir);
Serial.print(" | Counter: ");
Serial.println(encoderCount);
}
// Remember last CLK state
lastStateCLK = currentStateCLK;
}
void readButton() {
unsigned long currentTime = millis();
int buttonState = digitalRead(ENCODER_BUTTON_PIN);
if (buttonState == LOW) {
if (currentTime - lastButtonPress > buttonDebounceTime) {
buttonPressCount++;
lastButtonPress = currentTime;
}
}
if (currentMenuState == MAIN_PAGE && buttonPressCount >= 2) {
currentMenuState = SETTINGS_MENU;
displaySettingsMenu(0);
buttonPressCount = 0; // Reset the count after entering the menu
} else if (currentMenuState != MAIN_PAGE && buttonPressCount >= 1) {
// For other menus, a single press acts as "select" or "save"
// The action is handled within the respective menu's state logic
buttonPressCount = 1; // Ensure it's treated as a single action
} else if (buttonState == HIGH) {
// Reset the count if the button is released for a while
if (currentTime - lastButtonPress > 200) { // Adjust this delay as needed
buttonPressCount = 0;
}
}
}
void saveCalibration() {
EEPROM.put(EEPROM_REFERENCE_PRESSURE, currentReferencePressure);
Serial.println("Calibration saved to EEPROM");
}
void loadCalibration() {
if (EEPROM.read(EEPROM_REFERENCE_PRESSURE) != 0xFF) { // Check if EEPROM has been written before
EEPROM.get(EEPROM_REFERENCE_PRESSURE, currentReferencePressure);
Serial.print("Calibration loaded from EEPROM: ");
Serial.println(currentReferencePressure);
} else {
Serial.println("No calibration data in EEPROM, using default.");
}
}
void saveUnits() {
EEPROM.write(EEPROM_UNITS, currentUnits);
Serial.print("Units saved to EEPROM: ");
Serial.println(currentUnits == 0 ? "Meters" : "Feet");
}
void loadUnits() {
currentUnits = EEPROM.read(EEPROM_UNITS);
if (currentUnits != 0 && currentUnits != 1) {
currentUnits = 0; // Default to meters if invalid value
Serial.println("Invalid units in EEPROM, using default (Meters).");
} else {
Serial.print("Units loaded from EEPROM: ");
Serial.println(currentUnits == 0 ? "Meters" : "Feet");
}
}
float calculateAltitude(double pressure) {
// Simplified altitude calculation based on pressure and reference pressure
// Assumes standard atmospheric conditions
return 44330.0 * (1.0 - pow(pressure / currentReferencePressure, 0.1903));
}
float convertToFeet(float meters) {
return meters * 3.28084;
}
void setupEncoder() {
// Set encoder pins as inputs with pull-up resistors
pinMode(ENCODER_PIN_A, INPUT_PULLUP);
pinMode(ENCODER_PIN_B, INPUT_PULLUP);
// Initialize the last state of CLK for the encoder reading logic
lastStateCLK = digitalRead(ENCODER_PIN_A);
}
void setupButton() {
// Set the button pin as an input with a pull-up resistor
pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);
}