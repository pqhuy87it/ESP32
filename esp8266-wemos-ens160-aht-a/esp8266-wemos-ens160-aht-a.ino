
/*
  TEST ENS160+AHT21
  a 20231201 start testing.

*/

/***************************************************************************
  ENS160 - Digital Air Quality Sensor
  
  This is an example for ENS160 basic reading in standard mode
    
  Updated by Sciosense / 25-Nov-2021
 ***************************************************************************/

#include <Wire.h>
int ArduinoLED = 2;

///// AHT20 start
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

//Variables  Tim, reused same names so I did not have to change much.
int tempC;        //To store the temperature in C
int tempF;        //temp in F
int humidity;     //To store the humidity
///// AHT20 end

#include "ScioSense_ENS160.h"  // ENS160 library
//ScioSense_ENS160      ens160(ENS160_I2CADDR_0); //0x52
ScioSense_ENS160      ens160(ENS160_I2CADDR_1); //0x53..ENS160+AHT21

/*--------------------------------------------------------------------------
  SETUP function
  initiate sensor
 --------------------------------------------------------------------------*/
void setup() {

  Serial.begin(9600);

  while (!Serial) {}

  //Switch on LED for init
  pinMode(ArduinoLED, OUTPUT);
  digitalWrite(ArduinoLED, LOW);

  Serial.println("------------------------------------------------------------");
  Serial.println("ENS160 - Digital air quality sensor");
  Serial.println();
  Serial.println("Sensor readout in standard mode");
  Serial.println();
  Serial.println("------------------------------------------------------------");
  delay(1000);

  Serial.print("ENS160...");
  ens160.begin();
  Serial.println(ens160.available() ? "done." : "failed!");
  if (ens160.available()) {
    // Print ENS160 versions
    Serial.print("\tRev: "); Serial.print(ens160.getMajorRev());
    Serial.print("."); Serial.print(ens160.getMinorRev());
    Serial.print("."); Serial.println(ens160.getBuild());
  
    Serial.print("\tStandard mode ");
    Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
  }

  // AHT20 start
  Serial.println("Adafruit AHT10/AHT20 demo!");

  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
  //AHT20 end

} // end void setup

/*--------------------------------------------------------------------------
  MAIN LOOP FUNCTION
  Cylce every 1000ms and perform measurement
 --------------------------------------------------------------------------*/

void loop() {
  
    ///// AHT20 start
  sensors_event_t humidity1, temp; //Tim had to change to  humidity1
  aht.getEvent(&humidity1, &temp);// populate temp and humidity objects with fresh data
  tempC = (temp.temperature);
  tempF = (temp.temperature)*1.8+32;
  humidity = (humidity1.relative_humidity);
  Serial.print("Temperature: "); 
  Serial.print(tempC); 
  Serial.println(" degrees C");
  Serial.print("Temperature: "); 
  Serial.print(tempF); 
  Serial.println(" degrees F");
  Serial.print("Humidity: "); 
  Serial.print(humidity); 
  Serial.println("% rH");

  delay(1000);

    ///// AHT20 end

  if (ens160.available()) {

    // Give values to Air Quality Sensor.
    ens160.set_envdata(tempC, humidity);

    ens160.measure(true);
    ens160.measureRaw(true);
  
    Serial.print("AQI: ");Serial.print(ens160.getAQI());Serial.print("\t");
    Serial.print("TVOC: ");Serial.print(ens160.getTVOC());Serial.print("ppb\t");
    Serial.print("eCO2: ");Serial.print(ens160.geteCO2());Serial.println("ppm\t");
    //Serial.print("R HP0: ");Serial.print(ens160.getHP0());Serial.print("Ohm\t");
    //Serial.print("R HP1: ");Serial.print(ens160.getHP1());Serial.print("Ohm\t");
    //Serial.print("R HP2: ");Serial.print(ens160.getHP2());Serial.print("Ohm\t");
    //Serial.print("R HP3: ");Serial.print(ens160.getHP3());Serial.println("Ohm");
  }
  delay(1000);

} //end void loop

