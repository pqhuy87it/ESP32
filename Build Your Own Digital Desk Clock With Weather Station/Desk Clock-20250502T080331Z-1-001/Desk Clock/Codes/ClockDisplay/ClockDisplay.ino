#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "bitmaps.h"
 
//ESP82266 Board Manager - https://arduino.esp8266.com/stable/package_esp8266com_index.json

// WIFI INFORMATION
#define WIFI_SSID "YOUR WIFI SSIDE"
#define WIFI_PASSWORD "YOUR WIFI PASSWORD"
#define JSON_MEMORY_BUFFER 1024*2

// DISPLAY PINS
#define TFT_CS 15
#define TFT_DC 4
#define TFT_RST 2 
#define TFT_BL 5

// You can get API KEY and HOST KEY from RapidAPI, Search weatherapi.com and subscribe.
const char* API_KEY = "YOUR API KEY";
const char* API_HOST = "YOUR HOST KEY";

// Display and WiFiUdp 
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
WiFiUDP ntpUDP;

// NTP pool link:- 
// in.pool.ntp.org is for India
// You can visit pool.ntp.org to find your server
NTPClient timeClient(ntpUDP, "in.pool.ntp.org"); 

// Latitude and Longitude of you location. 
float lat = 28.63;
float lon = 77.22;
 
// API endpoint.
String weather_url = "https://weatherapi-com.p.rapidapi.com/current.json?q=" + String(lat) + "%2C" + String(lon);

// Global variables
String current_time;
String hour;
String minute;
String alternative;
String weekDay;
String month;
int day;
int year;
int temp;

// Array for days and months
String weekDays[7]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String months[12]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// For delay in fetching weather data.
unsigned long lastTime = 0;
unsigned long fetch_delay = 5000;

void setup(void){

  // Initialization
  Serial.begin(9600); 
  tft.init(240, 240);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  timeClient.begin();

  // Set this to you timezone in seconds i.e 5:30 = 19800 seconds;
  timeClient.setTimeOffset(19800);

  // Set display rotation
  tft.setRotation(3);

  // Clear display
  tft.fillScreen(0);

  // Set text color
  tft.setTextColor(ST77XX_CYAN);

  // Set font size
  tft.setTextSize(2);

  String loading = ".";

  // While connecting to wifi 
  while(WiFi.status() != WL_CONNECTED){  
    tft.setCursor(40, 90);
    tft.println("Connecting to ");
    tft.setCursor(40, 125);
    tft.print(WIFI_SSID);
    tft.println(loading);
    loading += ".";
    delay(500);
  }
  // Clear display
  tft.fillScreen(0);

  // Show connected
  tft.setCursor(60, 110);
  tft.println("Connected!");
  delay(3000);

  // Clear display and fetch tempurature
  tft.fillRect(60, 110, 130, 50, ST77XX_BLACK);
  fetchTemp();
}

void loop(){
  // Update time.
  timeClient.update();

  // Fetching weather after delay
  if((millis() - lastTime) > fetch_delay){
    currentTime();
    fetchTemp();
    lastTime = millis();
  }

  // Displaying items.
  display();
}

void display(){
  // default font size = 6x8px
  int font_w = 6;
  int font_h = 8;

  // UI size
  int time_size = 6;
  int alt_size = 2;
  int day_size = 3;

  // Display WxH
  int display_w = 240;
  int display_h = 240;

  // Distance between items
  int padding = 8;

  tft.setTextSize(time_size); // ie. 6x8 * 5 = 30x40
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);

  // X and Y of time on screen
  int time_x = (display_w/2) - ((font_w*time_size)*5)/2 - (font_w * alt_size);
  int time_y = 40;

  tft.setCursor(time_x, time_y);
  tft.println(current_time);
  tft.setTextSize(alt_size);
  tft.setCursor((time_x + (font_w*time_size)*5), time_y);
  tft.println(alternative);
  tft.drawBitmap((time_x + (font_w*time_size)*4 + 14), (time_y + (font_h*time_size) + padding), wifi, 31, 24, ST77XX_WHITE);
  tft.setTextSize(day_size);
  tft.setCursor(20, time_y+(font_h*time_size) + padding + 10);
  tft.println(weekDay);
  tft.setCursor(20, time_y+(font_h*time_size) + (font_h*day_size) + padding * 2 + 10);
  tft.println(day);
  tft.setCursor(20 + (font_w * day_size)*2 + padding, time_y+(font_h*time_size) + (font_h*day_size) + padding * 2 + 10);
  tft.println(month);
  tft.setTextSize(4);
  tft.setCursor(20,  time_y+(font_h*time_size) + (font_h*day_size) * 2 + padding * 3 + 10);
  tft.println(year);
  int temp_x = display_w - (font_w * 4)*2 - padding - (font_w * alt_size);
  tft.setCursor(temp_x,  time_y+(font_h*time_size) + (font_h*day_size) + padding * 2 + 10);
  tft.println(temp);
  tft.setTextSize(alt_size);
  tft.setCursor(temp_x +(font_w * 4) *2 , time_y+(font_h*time_size) + (font_h*day_size) + padding * 2 + 10);
  tft.println("o");
  tft.setTextSize(4);
  tft.setCursor(temp_x + 10 ,time_y+(font_h*time_size) + (font_h*day_size) * 2 + padding * 3 + 10);
  tft.println("C");
}

// Formatting and setting time
void currentTime(){
  hour = String(timeClient.getHours());
  minute = String(timeClient.getMinutes());
  weekDay = weekDays[timeClient.getDay()];
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  day = ptm->tm_mday;
  int current_month = ptm->tm_mon+1;
  month = months[current_month-1];
  year = ptm->tm_year+1900;
  if(hour.toInt() >= 12){
    alternative = "PM";
  }else{
    alternative = "AM";
  }
  if(hour.toInt() > 12){
    hour = map(hour.toInt(), 13, 24, 1, 12);
  }
  if(hour.toInt() < 10){
    hour = "0" + hour;
  }
  if(minute.toInt() < 10){
    minute = "0" + minute;
  }
  current_time = String(hour) + ":" + minute;
}

// Getting tempurature from API using Https request
void fetchTemp(){
  WiFiClientSecure client;
  HTTPClient https;
  client.setInsecure();
  https.useHTTP10(true);
  if(https.begin(client, weather_url.c_str())){
    https.addHeader("x-rapidapi-key", API_KEY);
    https.addHeader("x-rapidapi-host", API_HOST);

    int httpCode = https.GET();
    if(httpCode > 0){
      if(httpCode == 200){
        DynamicJsonDocument doc(JSON_MEMORY_BUFFER);
        DeserializationError error = deserializeJson(doc, https.getStream());
        Serial.print(https.getStream());
        if(error){
          Serial.println("deserialization error");
          Serial.println(error.f_str());
          temp = -1;
        }else{
          temp = doc["current"]["temp_c"].as<int>();
        }
      }
    }
  }
  https.end();
}