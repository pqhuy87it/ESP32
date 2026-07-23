# ESP32-S3 Touch GIF Player 🎬

A high-performance, interactive GIF player built using an ESP32-S3 development board and an ILI9341 touch display. This project reads `.gif` files directly from an SD card and plays them back at a buttery-smooth 30 FPS. It features a fully touch-controlled user interface for navigation and file selection.

## 📺 Video Tutorial

Build your own following the step-by-step guide!

[![Watch the tutorial](https://img.youtube.com/vi/YqsAhp6FQOE/0.jpg)](https://www.youtube.com/watch?v=YqsAhp6FQOE)

> **Click the image above to watch the full tutorial on YouTube.**

## ✨ Features
* **Direct SD Card Playback:** Reads and plays GIFs directly from a FAT32-formatted SD card.
* **Smooth Performance:** Achieves up to 30 FPS video-like playback utilizing the ESP32-S3's PSRAM.
* **Touch UI Controls:** Intuitive touch zones for Next, Previous, and Menu navigation.
* **GIF Selector Menu:** Browse and select specific GIFs from your SD card.
* **Playback Modes:** Switch between `Autoplay` (continuous looping through files) and `Single` play modes.

## 🛠️ Hardware Requirements
* **ESP32-S3 Development Board** (Must have **16MB Flash** and **8MB PSRAM** – *PSRAM is critical for frame buffering!*)
* **ILI9341 Touch Display** (with embedded SD card cartridge/reader)
* **Micro SD Card** * **Breadboard & Jumper Wires**

## ⚡ Wiring Guidelines
*⚠️ **Important Note:** The TFT display driver and the Touch panel driver share the same SPI pins (except for the CS pin). However, the SD Card reader uses a **different set of SPI pins** and a different SPI port. Please wire carefully according to your specific board's schematic.*

### 1. ILI9341 Display (SPI)
| Display Pin | ESP32-S3 GPIO | Function |
| :--- | :--- | :--- |
| VCC | 3.3V | Power |
| GND | GND | Ground |
| SCL (SCLK) | GPIO **[12]** | SPI Clock |
| SDA (MOSI) | GPIO **[11]** | SPI MOSI |
| MISO | GPIO **[13]** | SPI MISO |
| RES (RST) | GPIO **[9]** | Reset |
| DC | GPIO **[15]** | Data/Command |
| CS | GPIO **[4]** | Chip Select |
| Touch_CS | GPIO **[46]** | Touch Chip Select |
| Touch_SCL (SCLK) | GPIO **[12]** | Touch SPI Clock |
| DIN (MOSI) | GPIO **[11]** | Touch SPI DIN |
| DOUT | GPIO **[13]** | Touch SPI DOUT |

### 2. SD Card Module (SPI)
| SD Module Pin | ESP32-S3 GPIO | Function |
| :--- | :--- | :--- |
| SD_CS | GPIO **[16]** | Chip Select |
| SD_MOSI | GPIO **[17]** | SPI MOSI |
| SD_CLK | GPIO **[8]** | SPI SCLK |
| SD_MISO | GPIO **[18]** | SPI MISO |

## 💻 Software Setup

### 1. Arduino IDE & Board Configuration
1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software) (Version 2.3.8 was used for this project).
2. Go to **File > Preferences** and paste the following URL into the *Additional Boards Manager URLs* field:
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. Go to **Tools > Board > Boards Manager**, search for `esp32`, and install it (Version 3.3.7 used).
4. Select your exact ESP32-S3 board from the Tools menu.
5. **Crucial:** In the Tools menu, ensure **PSRAM** is enabled (OPI PSRAM / QSPI PSRAM depending on your board). This gives the ESP32 the 8MB of memory needed to buffer the GIF frames.

### 2. Install Required Libraries
Open the Arduino Library Manager and install the following:
* `TFT_eSPI` (For display and touch drivers)
* `AnimatedGIF` (For GIF decoding)

### 3. Configure TFT_eSPI
You must configure the `TFT_eSPI` library to work with the ILI9341 and your specific wiring:
1. Navigate to your Arduino libraries folder (usually `Documents/Arduino/libraries/TFT_eSPI`).
2. Open `User_Setup_Select.h` with a text editor (like VS Code or Notepad).
3. Comment out the default setup and **uncomment** the line corresponding to your custom setup or the ILI9341. 
4. Open `User_Setup.h` and redefine the SPI pins to match your exact wiring schematic. Save and close both files.

### 4. Touch Screen Calibration
Before running the main player, you need to calibrate the touch coordinates:
1. Open the `touch_calibrate` example sketch from the `TFT_eSPI` library.
2. Upload it to your board.
3. Open the Serial Monitor and set the baud rate to `115200`.
4. Follow the on-screen prompts and click the 4 corners of the display.
5. The Serial Monitor will output an array of **calibration values**. Copy these values and paste them into the main GIF player script.

## 📁 SD Card Preparation
1. Format your Micro SD card to **FAT32**.
2. Resize your GIFs to a maximum resolution of **320x240** for optimal, uncut playing.
3. Place your `.gif` files directly into the **root folder** of the SD card.
4. Insert the SD card into the display's reader and press the reset button on the ESP32.

## 🕹️ How to Use (Touch Controls)
Once the device boots up, use the touch screen to control playback:
* **Tap Left Side:** Play Previous GIF.
* **Tap Right Side:** Play Next GIF.
* **Tap Center:** Open the Main Menu / GIF Selector.
* **In Main Menu:** Tap to select a specific file, or use the toggle button to switch between **Autoplay** and **Single GIF** modes.

## 🐛 Troubleshooting
* **White Screen on Boot:** This usually indicates a communication failure with the display. Double-check your wiring connections, ensure your `User_Setup.h` pin definitions are strictly correct, and verify that your selected PsRam enabled.

---
## 🤝 Support

If you found this project helpful, please consider:
* **Subscribing** to the YouTube Channel.
* Giving the video a **Like**.
* Starring this GitHub Repository!

* **YouTube:** https://www.youtube.com/@DsnIndustries/videos
* **Patreon:** https://www.patreon.com/c/dsnIndustries

Happy Making!

## Games
* **Maze Escape:** https://play.google.com/store/apps/details?id=com.DsnMechanics.MazeEscape
* **Air Hockey:** https://play.google.com/store/apps/details?id=com.DsnMechanics.AirHockey
* **Click Challenge:** https://play.google.com/store/apps/details?id=com.DsNMechanics.ClickChallenge
* **Flying Triangels:** https://play.google.com/store/apps/details?id=com.DsnMechanics.Triangle
* **SkyScrapper:** https://play.google.com/store/apps/details?id=com.DsnMechanics.SkyScraper
