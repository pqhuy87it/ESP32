#include<GxEPD2_BW.h>
#include<WiFi.h>
#include<WebServer.h>
#include<LittleFS.h>
#include<Fonts/FreeMonoBold9pt7b.h>
#include<Fonts/FreeSans9pt7b.h>
#include<Fonts/FreeSerif9pt7b.h>
#include<Fonts/FreeMonoBold12pt7b.h>
#include<Fonts/FreeSans12pt7b.h>
#include<Fonts/FreeSerif12pt7b.h>

// E-Paper Display Pins
#define CS_PIN D1
#define DC_PIN D3
#define RST_PIN D0
#define BUSY_PIN D5

// Initialize E-Paper Display
GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(CS_PIN, DC_PIN, RST_PIN, BUSY_PIN));

// Wi-Fi Credentials
constchar* ssid = "*********";
constchar* password = "*********";

// Web Server
WebServer server(80);

// Default Font
const GFXfont* currentFont = &FreeMonoBold9pt7b;
uint16_t textColor = GxEPD_BLACK;
uint16_t textAlign = 0; // 0 = Left, 1 = Center, 2 = Right

// Function Prototypes
voidhandleRoot();
voidhandleUpdate();
voidhandleImageUpload();
voidhandleImageUploadFile();
voidupdateDisplay(String text);
voiddisplayImage(constchar* filename);
voiddisplayIPAddress(String ip);

// Embedded Web App HTML with Modern UI
constchar* html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>E-Paper Sticky Note</title>
<script src="https://cdn.tailwindcss.com"></script>
</head>
<body class="bg-gray-100">
<div class="max-w-md mx-auto mt-10 p-6 bg-white rounded-lg shadow-md">
<h1 class="text-2xl font-bold mb-6">E-Paper Sticky Note</h1>

<form id="textForm" class="mb-6">
<label class="block text-sm font-medium mb-2">Text:</label>
<textarea id="text" name="text" class="w-full p-2 border rounded-md mb-4"></textarea>

<label class="block text-sm font-medium mb-2">Font:</label>
<select id="font" name="font" class="w-full p-2 border rounded-md mb-4">
<option value="mono9">Monospace (9pt)</option>
<option value="sans9">Sans-Serif (9pt)</option>
<option value="serif9">Serif (9pt)</option>
<option value="mono12">Monospace (12pt)</option>
<option value="sans12">Sans-Serif (12pt)</option>
<option value="serif12">Serif (12pt)</option>
</select>

<label class="block text-sm font-medium mb-2">Text Alignment:</label>
<select id="align" name="align" class="w-full p-2 border rounded-md mb-4">
<option value="0">Left</option>
<option value="1">Center</option>
<option value="2">Right</option>
</select>

<button type="submit" class="w-full bg-blue-500 text-white p-2 rounded-md hover:bg-blue-600">Update Display</button>
</form>

<h2 class="text-xl font-bold mb-4">Upload Image</h2>
<form id="imageForm">
<input type="file" id="image" name="image" accept="image/bmp" class="mb-4">
<button type="submit" class="w-full bg-green-500 text-white p-2 rounded-md hover:bg-green-600">Upload Image</button>
</form>
</div>

<script>
const textForm = document.getElementById('textForm');
const imageForm = document.getElementById('imageForm');

textForm.addEventListener('submit', async (e) => {
e.preventDefault();
const formData = new FormData(textForm);
const response = await fetch('/update', {
method: 'POST',
body: new URLSearchParams(formData)
});
if (response.ok) {
alert("Display updated successfully!");
}
});

imageForm.addEventListener('submit', async (e) => {
e.preventDefault();
const formData = new FormData(imageForm);
const response = await fetch('/image', {
method: 'POST',
body: formData
});
if (response.ok) {
alert("Image uploaded successfully!");
}
});
</script>
</body>
</html>
)rawliteral";

voidsetup(){
Serial.begin(115200);

// Initialize E-Paper Display
display.init(115200, true, 50, false);
display.setRotation(1);

// Connect to Wi-Fi
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.println("Connecting to Wi-Fi...");
}
Serial.println("Connected to Wi-Fi");
Serial.println(WiFi.localIP());

// Display IP Address on E-Paper
displayIPAddress(WiFi.localIP().toString());

// Initialize LittleFS
if (!LittleFS.begin(true)) {
Serial.println("Failed to mount LittleFS");
return;
}

// Serve the web app
server.on("/", HTTP_GET, handleRoot);
server.on("/update", HTTP_POST, handleUpdate);
server.on("/image", HTTP_POST, handleImageUpload, handleImageUploadFile);
server.begin();
Serial.println("HTTP server started");
}

voidloop(){
server.handleClient();
}

// Serve the web app
voidhandleRoot(){
server.send(200, "text/html", html);
}

// Handle text and font updates
voidhandleUpdate(){
String text = server.arg("text");
String font = server.arg("font");
textAlign = server.arg("align").toInt();

// Set the selected font
if (font == "mono9") {
currentFont = &FreeMonoBold9pt7b;
} elseif (font == "sans9") {
currentFont = &FreeSans9pt7b;
} elseif (font == "serif9") {
currentFont = &FreeSerif9pt7b;
} elseif (font == "mono12") {
currentFont = &FreeMonoBold12pt7b;
} elseif (font == "sans12") {
currentFont = &FreeSans12pt7b;
} elseif (font == "serif12") {
currentFont = &FreeSerif12pt7b;
}

// Update the display
updateDisplay(text);
server.send(200, "text/plain", "Display updated");
}

// Handle image upload
voidhandleImageUpload(){
server.send(200, "text/plain", "Image uploaded");
}

voidhandleImageUploadFile(){
HTTPUpload& upload = server.upload();
if (upload.status == UPLOAD_FILE_START) {
// Open the file for writing
File file = LittleFS.open("/image.bmp", "w");
if (!file) {
Serial.println("Failed to open file for writing");
return;
}
} elseif (upload.status == UPLOAD_FILE_WRITE) {
// Write the received data to the file
File file = LittleFS.open("/image.bmp", "a");
if (file) {
file.write(upload.buf, upload.currentSize);
file.close();
}
} elseif (upload.status == UPLOAD_FILE_END) {
// Display the uploaded image
displayImage("/image.bmp");
}
}

// Update the display with text
voidupdateDisplay(String text){
display.setFullWindow();
display.firstPage();
do {
display.fillScreen(GxEPD_WHITE);
display.setFont(currentFont);
display.setTextColor(textColor);

// Calculate text position based on alignment
int16_t x = 0; // Default left alignment
int16_t y = 11;

if (textAlign == 1) { // Center alignment
x = (display.width() / 2) - (text.length() * 6); // Approximate center alignment
} elseif (textAlign == 2) { // Right alignment
x = display.width() - (text.length() * 12) - 10; // Approximate right alignment
}

display.setCursor(x, y);
display.println(text);
} while (display.nextPage());
}

// Display an image on the E-Paper
voiddisplayImage(constchar* filename){
File file = LittleFS.open(filename, "r");
if (!file) {
Serial.println("Failed to open image file");
return;
}

// Read the BMP file into a buffer
size_t fileSize = file.size();
uint8_t* buffer = (uint8_t*)malloc(fileSize);
if (!buffer) {
Serial.println("Failed to allocate memory for image");
file.close();
return;
}
file.read(buffer, fileSize);
file.close();

// Display the image
display.setFullWindow();
display.firstPage();
do {
display.fillScreen(GxEPD_WHITE);
display.drawImage(buffer, 0, 0, display.width(), display.height(), false, false, false);
} while (display.nextPage());

// Free the buffer
free(buffer);
}

// Display IP Address on E-Paper
voiddisplayIPAddress(String ip){
display.setFullWindow();
display.firstPage();
do {
display.fillScreen(GxEPD_WHITE);
display.setFont(&FreeMonoBold9pt7b);
display.setTextColor(GxEPD_BLACK);
display.setCursor(10, 30);
display.println("IP Address:");
display.setCursor(10, 60);
display.println(ip);
} while (display.nextPage());
}