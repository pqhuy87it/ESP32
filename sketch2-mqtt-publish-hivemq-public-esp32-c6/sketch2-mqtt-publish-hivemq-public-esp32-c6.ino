#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// Replace with your Wi-Fi credentials
const char* ssid = "YOUR-WIFI-SSID";
const char* password = "YOUR-WIFI-PSWD";

// MQTT broker settings
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic_publish = "botfreaks/mqttfxtest/data";
const char* mqtt_topic_subscribe = "botfreaks/mqttfxtest/commands";

WiFiClient espClient;
PubSubClient client(espClient);

// Define RGB LED parameters
#define noOfPixels 1
#define rgbPin 8  // Adjust if needed for your setup
Adafruit_NeoPixel rgbLed(noOfPixels, rgbPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  rgbLed.begin();
  Serial.println("Begin");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    rgbLed.setPixelColor(0, rgbLed.Color(255, 0, 0));  // Red while connecting
    rgbLed.show();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  rgbLed.setPixelColor(0, rgbLed.Color(0, 255, 0));  // Green when connected
  rgbLed.show();

  // Connect to MQTT broker
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32_C6")) {
      Serial.println("Connected to MQTT broker");
      rgbLed.setPixelColor(0, rgbLed.Color(0, 0, 255));  // Blue when MQTT connected
      rgbLed.show();
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  client.loop();

  // Publish sample data every 5 seconds
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();
    String msg = "Hello from ESP32-C6!";
    client.publish(mqtt_topic_publish, msg.c_str());
    Serial.println("Message published");
  }
}
