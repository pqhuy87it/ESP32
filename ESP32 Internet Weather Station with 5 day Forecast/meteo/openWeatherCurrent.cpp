/**The MIT License (MIT)

  Copyright (c) 2018 by ThingPulse Ltd., https://thingpulse.com

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
*/
#ifdef ESP8266
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif

#ifdef ESP32
#include <WebServer.h>
#include <WiFi.h>
#include <HTTPClient.h>
#endif
#include "openWeatherCurrent.h"


OpenWeatherMapCurrent::OpenWeatherMapCurrent() {

}

bool OpenWeatherMapCurrent::updateCurrent(OpenWeatherMapCurrentData *data, String url) {
  return doUpdate(data, url);
}

bool OpenWeatherMapCurrent::doUpdate(OpenWeatherMapCurrentData *data, String url) {
  unsigned long lostTest = 10000UL;
  unsigned long lost_do = millis();
  bool ret = true;
  this->weatherItemCounter = 0;
  this->data = data;
  JsonStreamingParser parser;
  parser.setListener(this);
  //Serial.printf("Getting url: %s\n", url.c_str());
  //HTTPClient http;
  //http.begin(url);
  WiFiClient client;
if (client.connect("api.openweathermap.org", 80))
  {
  bool isBody = false;
  char c;
  int size;
  client.print("GET " + url + " HTTP/1.1\r\n"
               "Host:api.openweathermap.org\r\n"
               "Connection: close\r\n\r\n");
  

    while (client.connected() || client.available()) {
      if (client.available()) {
        if ((millis() - lost_do) > lostTest) {
          Serial.println ("lost in client with a timeout");
          client.stop();
          //	ESP.restart();
          ret = false;;
          break;
        }
        c = client.read();
        if (c == '{' || c == '[') {
          isBody = true;
        }
        if (isBody) {
          parser.parse(c);
        }
      }
      // give WiFi and TCP/IP libraries a chance to handle pending events
      yield();
    }
  }

else ret = false;

this->data = nullptr;
return ret;
}

void OpenWeatherMapCurrent::whitespace(char c) {
//  Serial.println("whitespace");
}

void OpenWeatherMapCurrent::startDocument() {
//  Serial.println("start document");
}

void OpenWeatherMapCurrent::key(String key) {
  currentKey = String(key);
}

void OpenWeatherMapCurrent::value(String value) {
  // "lon": 8.54, float lon;
  if (currentKey == "lon") {
    this->data->lon = value.toFloat();
    this->data->cod = 0;
  }
  // "lat": 47.37 float lat;
  if (currentKey == "lat") {
    this->data->lat = value.toFloat();
  }
  // weatherItemCounter: only get the first item if more than one is available
  if (currentParent == "weather" && weatherItemCounter == 0) {
    // "id": 521, weatherId weatherId;
    if (currentKey == "id") {
      this->data->weatherId = value.toInt();
    }

    // "description": "shower rain", String description;
    if (currentKey == "description") {
      this->data->description = value;
    }
    // "icon": "09d" String icon;
    //String iconMeteoCon;
    if (currentKey == "icon") {
      this->data->icon = value;

    }

  }

  // "temp": 290.56, float temp;
  if (currentKey == "temp") {
    this->data->temp = value.toFloat();
    this->data->rain = 0;
    this->data->snow = 0 ;

  }
  // "pressure": 1013, uint16_t pressure;
  if (currentKey == "pressure") {
    this->data->pressure = value.toInt();
  }
  // "humidity": 87, uint8_t humidity;
  if (currentKey == "humidity") {
    this->data->humidity = value.toInt();
  }


  // "wind": {"speed": 1.5}, float windSpeed;
  if (currentKey == "speed") {
    this->data->windSpeed = value.toFloat();
  }
  // "wind": {deg: 226.505}, float windDeg;
  if (currentKey == "deg") {
    this->data->windDeg = value.toFloat();
  }

  if (currentParent == "rain" && currentKey == "3h") {
    this->data->rain = value.toFloat(); //обрезаю мусор
  }
  if (currentParent == "snow" && currentKey == "3h") {
    this->data->snow = value.toFloat();
  }


  // "dt": 1527015000, uint64_t observationTime;
  if (currentKey == "dt") {
    this->data->observationTime = value.toInt();
  }
  // "country": "CH", String country;
  if (currentKey == "country") {
    this->data->country = value;
  }
  // "sunrise": 1526960448, uint32_t sunrise;
  if (currentKey == "sunrise") {
    this->data->sunrise = value.toInt();
  }
  // "sunset": 1527015901 uint32_t sunset;
  if (currentKey == "sunset") {
    this->data->sunset = value.toInt();
  }
  // "name": "Zurich", String cityName;
  if (currentKey == "name") {
    this->data->cityName = value;
  }
  if (currentKey == "cod") {
    this->data->cod = value.toInt();
  }

}

void OpenWeatherMapCurrent::endArray() {

}


void OpenWeatherMapCurrent::startObject() {
  currentParent = currentKey;
}

void OpenWeatherMapCurrent::endObject() {
  if (currentParent == "weather") {
    weatherItemCounter++;
  }
  currentParent = "";
}

void OpenWeatherMapCurrent::endDocument() {

}

void OpenWeatherMapCurrent::startArray() {

}
