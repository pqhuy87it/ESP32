/**The MIT License (MIT)

  Copyright (c) 2018 by Daniel Eichhorn - ThingPulse

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  See more at https://thingpulse.com
*/

#include <Arduino.h>

#include <ESPWiFi.h>
#include <ESPHTTPClient.h>
#include <JsonListener.h>

// time
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()

#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
#include "Wire.h"
#include "OpenWeatherMapCurrent.h"
#include "OpenWeatherMapForecast.h"
#include "WeatherStationFonts.h"
#include "WeatherStationImages.h"
#include <string>
#include <DNSServer.h>
#include <ESPUI.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN D6  //dht11 
#include "pitches.h"
#include <stdio.h>
#include <stdlib.h>
#include <EEPROM.h>
#include "Bounce2.h"


#if defined(ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif



Bounce debouncer = Bounce();
int val = 0; 
int old_val = 0; 
int state = 0; 
int  makealarmoff =0;   //alarm off when alarm ring

DHT dht(DHTPIN, DHTTYPE);

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;


 int addr1 = 60;
 int addr2 = 15;
 int addr3 = 30;
 char charoftime[20];
 char inputofhour[3]="09";
 char inputofmin[3]="09";
 char inputofsecond[3]="09";
 int inputtime =0;
 int hourofeeprom;
 int minofeeprom;
 int secondofeeprom;
 int  hourofnow = 80 ;
int  minofnow = 80 ;
int  secondofnow = 80 ;



int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int duration = 500;  // 500 miliseconds
const byte tonesound = D5;


// WIFI
const char* WIFI_SSID = "Tenda12"; //Please change to your wifi name
const char* WIFI_PWD = "lym123456"; //please change to your wifi password

#define TZ              15 // (utc+) TZ in hours  // your need to change the number to your local area 0~24
#define DST_MN          60      // use 60mn for summer time in some countries 60

// Setup
const int UPDATE_INTERVAL_SECS = 20 * 60; // Update every 20 minutes

// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
#if defined(ESP8266)
const int SDA_PIN = D3;
const int SDC_PIN = D4;
#else
const int SDA_PIN = 5; //D3;
const int SDC_PIN = 4; //D4;
#endif



// void ICACHE_RAM_ATTR ISRoutine ();
const byte pin_interrput = 13;
int value = 0;








// OpenWeatherMap Settings
// Sign up here to get an API key:
// https://docs.thingpulse.com/how-tos/openweathermap-key/
String OPEN_WEATHER_MAP_APP_ID = "";   //please add your ID from  above link 
/*
  Go to https://openweathermap.org/find?q= and search for a location. Go through the
  result set and select the entry closest to the actual location you want to display
  data for. It'll be a URL like https://openweathermap.org/city/2657896. The number
  at the end is what you assign to the constant below.
*/
String OPEN_WEATHER_MAP_LOCATION_ID = "1819729";// Hong Kong： 1819729  

// Pick a language code from this list:
// Arabic - ar, Bulgarian - bg, Catalan - ca, Czech - cz, German - de, Greek - el,
// English - en, Persian (Farsi) - fa, Finnish - fi, French - fr, Galician - gl,
// Croatian - hr, Hungarian - hu, Italian - it, Japanese - ja, Korean - kr,
// Latvian - la, Lithuanian - lt, Macedonian - mk, Dutch - nl, Polish - pl,
// Portuguese - pt, Romanian - ro, Russian - ru, Swedish - se, Slovak - sk,
// Slovenian - sl, Spanish - es, Turkish - tr, Ukrainian - ua, Vietnamese - vi,
// Chinese Simplified - zh_cn, Chinese Traditional - zh_tw.
String OPEN_WEATHER_MAP_LANGUAGE = "en";
const uint8_t MAX_FORECASTS = 4;

const boolean IS_METRIC = true;

// Adjust according to your language
const String WDAY_NAMES[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
const String MONTH_NAMES[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

/***************************
   End Settings
 **************************/
// Initialize the oled display for address 0x3c
// sda-pin=14 and sdc-pin=12
SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
OLEDDisplayUi   ui( &display );

OpenWeatherMapCurrentData currentWeather;
OpenWeatherMapCurrent currentWeatherClient;

OpenWeatherMapForecastData forecasts[MAX_FORECASTS];
OpenWeatherMapForecast forecastClient;

#define TZ_MN           ((TZ)*60)
#define TZ_SEC          ((TZ)*3600)
#define DST_SEC         ((DST_MN)*60)
time_t now;

// flag changed in the ticker function every 10 minutes
bool readyForWeatherUpdate = false;

String lastUpdate = "--";

long timeSinceLastWUpdate = 0;

//declaring prototypes
void drawProgress(OLEDDisplay *display, int percentage, String label);
void updateData(OLEDDisplay *display);
void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawForecast(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex);
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);
void setReadyForWeatherUpdate();
////////////////////DHT
void drawCurrentHT (OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawalarm (OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawIP (OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);

// Add frames
// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
FrameCallback frames[] = { drawDateTime, drawCurrentWeather, drawForecast, drawCurrentHT, drawalarm, drawIP };
int numberOfFrames = 6;//5

OverlayCallback overlays[] = { drawHeaderOverlay };
int numberOfOverlays = 1;



void ICACHE_RAM_ATTR ISRoutine ();

/*****************************HTTP server*******************************/
void textCall(Control sender, int type) {
   Serial.println(sender.value);
  String Valueofsender = sender.value;
   Serial.println("123456");
  
   
   strcpy(charoftime,Valueofsender.c_str()); //string转换为char类型
   strncpy(inputofhour, charoftime, 2);
   inputofhour[2] = '\0'; 
   Serial.println(inputofhour);

   strncpy(inputofmin, charoftime+3, 2);
    inputofmin[2] = '\0'; 
   Serial.println(inputofmin);

    strncpy(inputofsecond, charoftime+6, 2);
    inputofsecond[2] = '\0'; 
   Serial.println(inputofsecond);
   
    EEPROM.begin(512);
    EEPROM.write(addr2, atoi(inputofhour));
    EEPROM.write(addr1, atoi(inputofmin));
    EEPROM.write(addr3, atoi(inputofsecond));
  

 
 
  hourofeeprom = EEPROM.read(addr2);
  minofeeprom =  EEPROM.read(addr1);
  secondofeeprom = EEPROM.read(addr3);
  
  
  Serial.println(minofeeprom);
   inputtime =1;
   EEPROM.end();

   
    
}
/*****************************HTTP server*******************************/

void setup() {
  Serial.begin(115200);
  dht.begin();
   EEPROM.begin(512);


  Serial.println("11111111");
  pinMode(pin_interrput, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin_interrput), ISRoutine, RISING);

  ///////////////////////////////
  debouncer.attach(pin_interrput);//
  debouncer.interval(5);//




  Serial.println();
  Serial.println();

  // initialize dispaly
  display.init();
  display.clear();
  display.display();

  //display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(255);

  WiFi.begin(WIFI_SSID, WIFI_PWD);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.clear();
    display.drawString(64, 10, "Connecting to WiFi");
    display.drawXbm(46, 30, 8, 8, counter % 3 == 0 ? activeSymbole : inactiveSymbole);
    display.drawXbm(60, 30, 8, 8, counter % 3 == 1 ? activeSymbole : inactiveSymbole);
    display.drawXbm(74, 30, 8, 8, counter % 3 == 2 ? activeSymbole : inactiveSymbole);
    display.display();
    counter++;
  }

  /*****************************HTTP server*******************************/
  Serial.println("Got IP:");
  Serial.println(WiFi.localIP());
 char bufIP[16];
 sprintf(bufIP, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
 
  Serial.println(WiFi.localIP()[0]);
   Serial.println(WiFi.localIP()[1]);
    Serial.println(WiFi.localIP()[2]);
     Serial.println(WiFi.localIP()[3]);
//  display.print((char*) WiFi.localIP().toString().c_str(),3,0);
//   char buff[14];
//  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);
  
  /*****************************HTTP server*******************************/

  // Get time from network time service
  configTime(TZ_SEC, DST_SEC, "pool.ntp.org");

  ui.setTargetFPS(30);

  ui.setActiveSymbol(activeSymbole);
  ui.setInactiveSymbol(inactiveSymbole);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  ui.setFrames(frames, numberOfFrames);

  ui.setOverlays(overlays, numberOfOverlays);

  // Inital UI takes care of initalising the display too.
  ui.init();

  Serial.println("");

  updateData(&display);

  /*****************************HTTP server*******************************/
  ESPUI.text("Set alarm time:", &textCall, COLOR_ALIZARIN, "00:00:00");

  dnsServer.start(DNS_PORT, "*", apIP);

  ESPUI.begin("ESPUI Control");
  /*****************************HTTP server*******************************/
}

void loop() {

 // Serial.print("makealarmoff=");
//  Serial.println(makealarmoff);


  debouncer.update();//
  val = debouncer.read(); ///

  if ((val == HIGH) && (old_val == LOW)) 
  {
    printState();
  }
  old_val = val; 



  if (millis() - timeSinceLastWUpdate > (1000L * UPDATE_INTERVAL_SECS)) {
    setReadyForWeatherUpdate();
    timeSinceLastWUpdate = millis();
  }

  if (readyForWeatherUpdate && ui.getUiState()->frameState == FIXED) {
    updateData(&display);
  }



 
  if ((hourofeeprom == hourofnow && minofeeprom == minofnow && secondofnow>=secondofeeprom)||(hourofeeprom == hourofnow && minofeeprom+1 == minofnow && secondofnow<=secondofeeprom)) //(secondoflocal<=30)
  {
    
      
     if(makealarmoff ==0)
     {
      Serial.println("alarm is ring");
      for (int thisNote = 0; thisNote < 8; thisNote++) {

        // pin8 output the voice, every scale is 0.5 sencond
        tone(tonesound, melody[thisNote], duration);

        // Output the voice after several minutes

        delay(1000);
      }

      delay(2000);  // restart after two seconds
      
    }
   

  }
  else
  {
   makealarmoff =0; 
  }







  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.
    delay(remainingTimeBudget);
  }

 ///  hourofnow = timeInfo->tm_hour;
 // minofnow = timeInfo->tm_min;
//  secondofnow = timeInfo->tm_sec;
/*Serial.print("hour:");
Serial.println(hourofnow);

Serial.print("min:");
Serial.println(minofnow);

*/
  
}


/*****************************HTTP server*******************************/
void ICACHE_RAM_ATTR ESP8266_UI_timerISR() {
 if (readyForWeatherUpdate == false)
    dnsServer.processNextRequest();  
}
/*****************************HTTP server*******************************/

///////////////////////////////////////////////
void printState()
{
  display.clear();
//  value++;
  ui.nowbutton++;

 /* if (value >= 5) {
    value = 0;
  }
*/
  if (ui.nowbutton >= 6) {
    ui.nowbutton = 0;
  }


}




















//////////////////////////////////////////////////

void ISRoutine () {

   if ((hourofeeprom == hourofnow && minofeeprom == minofnow && secondofnow>=secondofeeprom)||(hourofeeprom == hourofnow && minofeeprom+1 == minofnow && secondofnow<=secondofeeprom))
  {
    makealarmoff =1;
  }
  else
  {
     makealarmoff =0;
  }
  

}












void drawProgress(OLEDDisplay *display, int percentage, String label) {
  display->clear();
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(64, 10, label);
  display->drawProgressBar(2, 28, 124, 10, percentage);
  display->display();
}

void updateData(OLEDDisplay *display) {

  /*****************************HTTP server*******************************/
  //ESPUI
  /*****************************HTTP server*******************************/

  drawProgress(display, 10, "Updating time...");
  drawProgress(display, 30, "Updating weather...");
  currentWeatherClient.setMetric(IS_METRIC);
  currentWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  currentWeatherClient.updateCurrentById(&currentWeather, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);
  drawProgress(display, 50, "Updating forecasts...");
  forecastClient.setMetric(IS_METRIC);
  forecastClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  uint8_t allowedHours[] = {12};
  forecastClient.setAllowedHours(allowedHours, sizeof(allowedHours));
  forecastClient.updateForecastsById(forecasts, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID, MAX_FORECASTS);

  readyForWeatherUpdate = false;
  drawProgress(display, 100, "Done...");
  delay(1000);
}



void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[16];


  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  String date = WDAY_NAMES[timeInfo->tm_wday];

  sprintf_P(buff, PSTR("%s, %02d/%02d/%04d"), WDAY_NAMES[timeInfo->tm_wday].c_str(), timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
  display->drawString(64 + x, 5 + y, String(buff));
  display->setFont(ArialMT_Plain_24);




  sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  display->drawString(64 + x, 15 + y, String(buff));
  display->setTextAlignment(TEXT_ALIGN_LEFT);



}

void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 38 + y, currentWeather.description);

  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = String(currentWeather.temp, 1) + (IS_METRIC ? "°C" : "°F");
  display->drawString(60 + x, 5 + y, temp);

  display->setFont(Meteocons_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(32 + x, 0 + y, currentWeather.iconMeteoCon);
}


void drawForecast(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  drawForecastDetails(display, x, y, 0);
  drawForecastDetails(display, x + 44, y, 1);
  drawForecastDetails(display, x + 88, y, 2);
}

void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex) {
  time_t observationTimestamp = forecasts[dayIndex].observationTime;
  struct tm* timeInfo;
  timeInfo = localtime(&observationTimestamp);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y, WDAY_NAMES[timeInfo->tm_wday]);

  display->setFont(Meteocons_Plain_21);
  display->drawString(x + 20, y + 12, forecasts[dayIndex].iconMeteoCon);
  String temp = String(forecasts[dayIndex].temp, 0) + (IS_METRIC ? "°C" : "°F");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y + 34, temp);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);
  ///////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////

  hourofnow = timeInfo->tm_hour;
  minofnow = timeInfo->tm_min;
  secondofnow = timeInfo->tm_sec;



  display->setColor(WHITE);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0, 54, String(buff));
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  String temp = String(currentWeather.temp, 1) + (IS_METRIC ? "°C" : "°F");
  display->drawString(128, 54, temp);
  display->drawHorizontalLine(0, 52, 128);
}

void setReadyForWeatherUpdate() {
  Serial.println("Setting readyForUpdate to true");
  readyForWeatherUpdate = true;
}








//////////////////////////////////////////////////////////////////////////

void drawCurrentHT (OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y)
{

  float old_h = 0, old_t = 0;
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (old_h == h)
  {
    return;
  }
  else
  {
    float h = dht.readHumidity();
    old_h = h;
  }

  if (old_t == t)
  {
    return;
  }
  else
  {
    float t = dht.readTemperature();
    old_t = t;
  }




  if ((h != 0) || (t != 0))
  {
    Serial.println(h);
    Serial.println(t);
  }

  display->setFont(ArialMT_Plain_16);  //ArialMT_Plain_16
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(10 + x, 0 + y, "Humt :");
  // display->drawString(60 + x, 0 + y,String(h));
  display->drawString(60 + x, 0 + y, String(old_h));
  display->drawString(100 + x, 0 + y, "%");

  display->drawString(10 + x, 25 + y, "Temp :");
  //    display->drawString(60 + x, 25 + y,String(t));
  display->drawString(60 + x, 25 + y, String(old_t));
  display->drawString(100 + x, 25 + y, "°C");

  Serial.println("888");


 
}

void drawalarm (OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y)
{
 int showofhour,showofmin,showofsecond;
 

  display->setFont(ArialMT_Plain_24); //ArialMT_Plain_16
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(35 + x, 0 + y, "Alarm");



if(inputtime ==0)
{
showofhour =EEPROM.read(addr2);
showofmin =EEPROM.read(addr1);
showofsecond = EEPROM.read(addr3);
  display->setFont(ArialMT_Plain_24); //ArialMT_Plain_16
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(43 + x, 25 + y, String(showofhour));
  display->drawString(49 + x, 25 + y, ":");
  display->drawString(73 + x, 25 + y, String(showofmin));
  display->drawString(79 + x, 25 + y, ":");
  display->drawString(103 + x, 25 + y, String(showofsecond));
  if(showofhour<10)
  {
   display->drawString(30 + x, 25 + y, "0"); 
  }
   if(showofmin<10)
  {
   display->drawString(60 + x, 25 + y, "0"); 
  }
  if(showofsecond<10)
  {
   display->drawString(90 + x, 25 + y, "0"); 
  }


}

else
{

  display->setFont(ArialMT_Plain_24); //ArialMT_Plain_16
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(20 + x, 25 + y, String(inputofhour));
  display->drawString(45 + x, 25 + y, ":");
  display->drawString(50 + x, 25 + y, String(inputofmin));
  display->drawString(75 + x, 25 + y, ":");
  display->drawString(80 + x, 25 + y, String(inputofsecond));

  
}

}

void drawIP (OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y)
{
  display->setFont(ArialMT_Plain_24); //ArialMT_Plain_16
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(50 + x, 0 + y, "IP");

 Serial.println("Got IP:");
 Serial.println(WiFi.localIP());
// char bufIP[16];
// sprintf(bufIP, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
 
  display->setFont(ArialMT_Plain_16); //ArialMT_Plain_16
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(5 + x, 25 + y, String(WiFi.localIP()[0]));
  display->drawString(32 + x, 25 + y, ".");
  display->drawString(34 + x, 25 + y, String(WiFi.localIP()[1]));
  display->drawString(60 + x, 25 + y, "."); 
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(90 + x, 25 + y, String(WiFi.localIP()[2]));
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(90 + x, 25 + y, "."); 
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(94 + x, 25 + y, String(WiFi.localIP()[3]));
 


  
}
