#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>


// WiFi credentials
const char* ssid = "SSID";
const char* password = "Password";


// NeoPixel configuration
#define LED_PIN 2
#define NUM_LEDS 16
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


uint8_t brightness = 255; // Default brightness (max)


// Current mode state
String currentMode = "Free";


// Create web server instance
AsyncWebServer server(80);


// Webpage HTML
const char index_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Mood Light Control</title>
<metaname="viewport"content="width=device-width, initial-scale=1">
<style>
body {
font-family: Arial, sans-serif;
text-align: center;
margin: 0;
padding: 0;
}
.container {
margin: 20px;
}
button {
padding: 20px;
font-size: 18px;
margin: 10px;
cursor: pointer;
border: none;
border-radius: 5px;
transition: 0.3s;
}
.red { background-color: red; color: white; }
.yellow { background-color: yellow; color: black; }
.green { background-color: green; color: white; }
input[type="color"] {
margin: 10px;
height: 50px;
width: 50px;
border: none;
cursor: pointer;
}
input[type="range"] {
width: 80%;
margin: 20px10%;
}
.current-mode {
font-size: 20px;
margin: 20px;
font-weight: bold;
}
@media (max-width: 600px) {
button {
padding: 15px;
font-size: 16px;
}
}
</style>
</head>
<body>
<h1>Mood Signal Control</h1>
<divclass="container">
<divclass="current-mode">Current Mood: <spanid="currentMode">Free</span></div>
<buttonclass="red"onclick="setMode('red', 'Busy')">Busy</button>
<buttonclass="yellow"onclick="setMode('yellow', 'Working')">Working</button>
<buttonclass="green"onclick="setMode('green', 'Free')">Free</button>
<br>
<inputtype="color"id="colorPicker"onchange="setCustomColor(this.value)" />
<br>
<inputtype="range"min="10"max="255"value="255"id="brightnessSlider"oninput="setBrightness(this.value)" />
</div>
<script>
functionsetMode(color, mode) {
fetch(`/set?color=${color}&mode=${mode}`);
document.getElementById('currentMode').innerText = mode;
}


functionsetCustomColor(color) {
fetch(`/setCustom?color=${color.substring(1)}`);
}


functionsetBrightness(brightness) {
fetch(`/brightness?level=${brightness}`);
}
</script>
</body>
</html>
)rawliteral";


void setup() {
// Initialize serial monitor
Serial.begin(115200);


// Initialize NeoPixel strip
strip.begin();
strip.setBrightness(brightness);
strip.show();


// Connect to Wi-Fi
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.println("Connecting to WiFi...");
}
Serial.println("Connected to WiFi");
Serial.print("IP Address: ");
Serial.println(WiFi.localIP());


// Serve the web page
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
request->send(200, "text/html", index_html);
});


// Handle pre-defined mode changes
server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request) {
if (request->hasParam("color") && request->hasParam("mode")) {
String color = request->getParam("color")->value();
String mode = request->getParam("mode")->value();
currentMode = mode;


if (color == "red") setLEDColor(255, 0, 0);
else if (color == "yellow") setLEDColor(255, 255, 0);
else if (color == "green") setLEDColor(0, 255, 0);
}
request->send(200, "text/plain", "OK");
});


// Handle custom color
server.on("/setCustom", HTTP_GET, [](AsyncWebServerRequest *request) {
if (request->hasParam("color")) {
String hexColor = request->getParam("color")->value();
long rgb = strtol(hexColor.c_str(), NULL, 16);
setLEDColor((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
}
request->send(200, "text/plain", "OK");
});


// Handle brightness control
server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request) {
if (request->hasParam("level")) {
brightness = request->getParam("level")->value().toInt();
strip.setBrightness(brightness);
strip.show();
}
request->send(200, "text/plain", "OK");
});


// Start the server
server.begin();
}


// Function to set LED color
void setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
for (int i = 0; i < NUM_LEDS; i++) {
strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off all LEDs
}
strip.show();
delay(500); // Delay for 0.5 seconds
for (int i = 0; i < NUM_LEDS; i++) {
strip.setPixelColor(i, strip.Color(r, g, b));
}
strip.show();
}


void loop() {
// Nothing to do here, handled by the server
}