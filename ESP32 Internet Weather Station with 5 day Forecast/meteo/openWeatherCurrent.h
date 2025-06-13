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

#pragma once
#include <JsonListener.h>
#include <JsonStreamingParser.h>

typedef struct OpenWeatherCurrentData {
  // "lon": 8.54,
  float lon;
  // "lat": 47.37
  float lat;
  // "id": 521,
  uint16_t weatherId;
  
  // "description": "shower rain",
  String description;
  // "icon": "09d"
  String icon;
 
  // "temp": 290.56,
  float temp;
  // "pressure": 1013,
  uint16_t pressure;
  // "humidity": 87,
  uint8_t humidity;
  
  // "wind": {"speed": 1.5},
  float windSpeed;
  // "wind": {deg: 226.505},
  float windDeg;

  
  uint32_t observationTime;
  // "country": "CH",
  String country;
  // "sunrise": 1526960448,
  uint32_t sunrise;
  // "sunset": 1527015901
  uint32_t sunset;
  // "name": "Zurich",
  String cityName;

float rain;                                     
float snow;
int cod;
} OpenWeatherMapCurrentData;

class OpenWeatherMapCurrent: public JsonListener {
  private:
    String currentKey;
    String currentParent;
    OpenWeatherMapCurrentData *data;
    uint8_t weatherItemCounter = 0;
    boolean metric = true;
    String language;

    bool doUpdate(OpenWeatherMapCurrentData *data, String url);
  
  public:
    OpenWeatherMapCurrent();
    bool updateCurrent(OpenWeatherMapCurrentData *data, String url);
       virtual void whitespace(char c);

    virtual void startDocument();

    virtual void key(String key);

    virtual void value(String value);

    virtual void endArray();

    virtual void endObject();

    virtual void endDocument();

    virtual void startArray();

    virtual void startObject();
};
