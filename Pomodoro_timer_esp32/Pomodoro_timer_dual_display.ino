#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pins
#define RED_LED 25
#define BLUE_LED 26
#define GREEN_LED 27
#define BUTTON_PIN 2     // Pause/Resume
#define RESET_PIN 4      // Also used to confirm during init
#define POT_PIN  32      // Potentiometer

// Timing values
int studyMinutes = 25;
int breakMinutes = 5;
int minutes = 0;
int seconds = 0;

bool isBreak = false;
bool isPaused = true;

unsigned long previousMillis = 0;
const long interval = 1000;

// Debounce
bool lastButtonState = HIGH;
bool currentButtonState = HIGH;
unsigned long lastDebounceTime = 0;

bool lastResetState = HIGH;
bool currentResetState = HIGH;
unsigned long lastResetDebounceTime = 0;

const unsigned long debounceDelay = 50;

// States
enum Mode { INIT_FOCUS, INIT_BREAK, RUNNING } mode = INIT_FOCUS;

void setup() {
  Wire.begin(21, 22);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  lcd.init();
  lcd.backlight();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);

  lcd.setCursor(0, 0);
  lcd.print(" Initializing... ");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set focus time-");
}

void loop() {
  handleButtonPress();
  handleResetButton();

  if (mode == INIT_FOCUS || mode == INIT_BREAK) {
    int potValue = analogRead(POT_PIN);
    int mappedMinutes = map(potValue, 0, 4095, 0, 60);

    // Display time on OLED
    oled.clearDisplay();
    oled.setTextSize(3);
    String timeStr = (mappedMinutes < 10 ? "0" : "") + String(mappedMinutes) + ":00";

    int16_t x1, y1;
    uint16_t w, h;
    oled.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
    int x = (SCREEN_WIDTH - w) / 2;
    int y = (SCREEN_HEIGHT - h) / 2;

    oled.setCursor(x, y);
    oled.print(timeStr);
    oled.display();

    // Store for confirmation
    if (mode == INIT_FOCUS) studyMinutes = mappedMinutes;
    else if (mode == INIT_BREAK) breakMinutes = mappedMinutes;

  } else if (mode == RUNNING && !isPaused && millis() - previousMillis >= interval) {
    previousMillis = millis();

    if (seconds == 0) {
      if (minutes == 0) {
        isBreak = !isBreak;
        minutes = isBreak ? breakMinutes : studyMinutes;
        seconds = 0;
        updateLCD();
      } else {
        minutes--;
        seconds = 59;
      }
    } else {
      seconds--;
    }

    updateLEDs();
    updateOLED();
  }

  if (mode == RUNNING && isBreak) {
    updateLEDs(); // keep blinking red
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(isBreak ? "   BREAK MODE  " : "   STUDY MODE  ");
  lcd.setCursor(0, 1);
  lcd.print(isPaused ? "==== PAUSED ====" : "=== RUNNING ===");
}

void updateOLED() {
  oled.clearDisplay();
  oled.setTextSize(3);
  String timeStr = (minutes < 10 ? "0" : "") + String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds);

  int16_t x1, y1;
  uint16_t w, h;
  oled.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
  int x = (SCREEN_WIDTH - w) / 2;
  int y = (SCREEN_HEIGHT - h) / 2;

  oled.setCursor(x, y);
  oled.print(timeStr);
  oled.display();
}

void updateLEDs() {
  if (isBreak) {
    static bool State = false;
    static bool State2;
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink >= 500) {
      State2 = State;
      State = !State;
       
      
      digitalWrite(RED_LED, State);
      delay(500);
      digitalWrite(RED_LED, State2);
      digitalWrite(BLUE_LED, State);
      delay(500);
      digitalWrite(BLUE_LED, State2);
      digitalWrite(GREEN_LED, State);
      delay(500);
      digitalWrite(GREEN_LED, State2);
      lastBlink = millis();
    }
  } else {
    int total = studyMinutes * 60;
    int current = minutes * 60 + seconds;
    float progress = 1.0 - (float)current / total;

    if (progress < 0.2) {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      digitalWrite(RED_LED, HIGH);
    } else if (progress < 0.8) {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
      digitalWrite(RED_LED, HIGH);
    } else {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      digitalWrite(RED_LED, HIGH);
    }
  }
}

void handleButtonPress() {
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currentButtonState) {
      currentButtonState = reading;
      if (currentButtonState == LOW && mode == RUNNING) {
        isPaused = !isPaused;
        updateLCD();
      }
    }
  }

  lastButtonState = reading;
}

void handleResetButton() {
  int reading = digitalRead(RESET_PIN);
  if (reading != lastResetState) lastResetDebounceTime = millis();

  if ((millis() - lastResetDebounceTime) > debounceDelay) {
    if (reading != currentResetState) {
      currentResetState = reading;

      if (currentResetState == LOW) {
        if (mode == INIT_FOCUS) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Initializing...");
          lcd.setCursor(0, 1);
          lcd.print("Set break time- ");
          mode = INIT_BREAK;
        } else if (mode == INIT_BREAK) {
          lcd.clear();
          isBreak = false;
          isPaused = true;
          minutes = studyMinutes;
          seconds = 0;
          updateLCD();
          updateOLED();
          mode = RUNNING;
        } else {
          // Reinitialize
          mode = INIT_FOCUS;
          lcd.clear();

          lcd.setCursor(0, 0);
          lcd.print("Initializing...");
          lcd.setCursor(0, 1);
          lcd.print("Set focus time-");
        }
      }
    }
  }

  lastResetState = reading;
}
