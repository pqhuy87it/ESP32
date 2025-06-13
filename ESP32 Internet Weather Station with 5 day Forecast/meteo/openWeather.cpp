
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
#include "openWeather.h"

//------------------------------------------ Universal weather request parcer for opewWeatherMap site ---------
void OWMrequest::doUpdate(String url, byte maxForecasts) {
  unsigned long lostTest = 30000UL;
  unsigned long lost_do = millis();
  bool isBody = false;
  char c;
  int count = 0;
  JsonStreamingParser parser;
  parser.setListener(this);


  HTTPClient http;
  WiFiClient * client;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    client = http.getStreamPtr();
    while (client->connected()) {
      while (true) {
        if (client->available() > 0) {
          c = client->read();
          if (c == '{' || c == '[') {
            isBody = true;

          }
          if (isBody) {
            parser.parse(c);
            if (c == '{') count++;
            else if (c == '}') count--;
            //           Serial.print (F("count"));  Serial.println (count);
            if (count == 0)
            {
              parser.reset();
              return;
              //break; //end
            }
          }

        }
        else {
          uint32_t now_ms = millis();
          while (millis() - now_ms < 200) {
            yield();
            if (client->available() > 0)
              break;
          }
          if (client->available() <= 0)
            break;
        }
      }
    }
  }

  else
    Serial.println (F("NOT connrct api.openweathermap.org:80"));
  parser.reset();
  //  client->stop();




}

void OWMrequest::init(void) {
  for (byte i = 0; i < OWM_max_layers; ++i)
    p_key[i] = "";
  currentParent = currentKey = "";
}

void OWMrequest::startObject() {
  for (byte i = OWM_max_layers - 1; i > 0; --i) {
    p_key[i] = p_key[i - 1];
  }
  p_key[0] = currentParent;
  currentParent = currentKey;
}

void OWMrequest::endObject() {
  currentParent = p_key[0];
  for (byte i = 0; i < OWM_max_layers - 1; ++i) {
    p_key[i] = p_key[i + 1];
  }
  currentKey = "";
}

//------------------------------------------ Five day forecast from openweatrhermap.org -----------------------
byte OWMfiveForecast::updateForecast(OWM_fiveForecast *forecasts, byte maxForecasts, String url_my) {
  this->forecasts = forecasts;
  this->max_forecasts = maxForecasts;
  OWMrequest::init();
  index = 0;
  timestamp = 0;

  String url = url_my;
  doUpdate(url, maxForecasts);

  this->forecasts = nullptr;
  return index;
}

void OWMfiveForecast::value(String value) {
  //#ifdef debug
  //Serial.print(F("String value = "));
  //Serial.println(value);
  //Serial.print(F("index = "));
  //Serial.println(index);
  //#endif debug



  if (index >= max_forecasts) {
    return;
  }



  if (currentKey == "dt") {
    if (timestamp == 0) {
      index = 0;
      forecasts[index].dt = value;
      timestamp = value.toInt();
    } else {
      uint32_t t = value.toInt();
      if (t > timestamp) {                          // new forecast time
        if (index < max_forecasts - 1) {
          ++index;
          timestamp = t;
          forecasts[index].dt = value;
        }
      }
    }
  } else if (currentKey == "temp") {
    forecasts[index].temp = int(round(value.toFloat())); // value.toFloat();
    forecasts[index].snow = 0;
    forecasts[index].rain = 0 ;
  } else

    if (currentKey == "pressure") {
      forecasts[index].pressure = value.toInt();
    } else

      if (currentParent == "weather" && currentKey == "description") {
        forecasts[index].description = value;
      } else if (currentParent == "weather" && currentKey == "icon") {
        forecasts[index].icon = value;
      } else

        if (currentParent == "wind" && currentKey == "speed") {
          forecasts[index].w_speed = value.toInt();
        } else if (currentParent == "wind" && currentKey == "deg") {
          forecasts[index].w_deg = value.toInt();
        } else

          if (currentParent == "rain" && currentKey == "3h") {
            forecasts[index].rain = value.toFloat(); //обрезаю мусор
          }  else if (currentParent == "snow" && currentKey == "3h") {
            forecasts[index].snow = value.toFloat();
          }
          else

            if (currentKey == "humidity") {
              forecasts[index].h = value.toInt();
            }

}
