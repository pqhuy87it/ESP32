#define USER_SETUP_INFO "User_Setup"

// ##################################################################################
// Section 1. Driver
// ##################################################################################

#define ILI9341_DRIVER          // Màn ILI9341 320x240

// Nếu màu bị đảo đỏ<->xanh thì thử 1 trong 2:
// #define TFT_RGB_ORDER TFT_RGB
#define TFT_RGB_ORDER TFT_BGR

// ##################################################################################
// Section 2. Pins (ESP32-S3) — theo wiring README
// ##################################################################################
// TFT + Touch dùng CHUNG bus SPI (chỉ khác CS).
// SD Card dùng bus FSPI RIÊNG: SCK=8, MISO=18, MOSI=17, CS=16 (khai báo trong code .ino).

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#define TFT_MISO 13
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS    4    // Chip select TFT
#define TFT_DC   15    // Data/Command
#define TFT_RST   9    // Reset
#define TOUCH_CS 46

// ##################################################################################
// Section 3. Fonts (code dùng font 2 & 4)
// ##################################################################################

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

// ##################################################################################
// Section 4. Other
// ##################################################################################

#define SPI_FREQUENCY        40000000   // ILI9341 chạy tốt 40MHz; nhiễu thì hạ 27MHz
#define SPI_READ_FREQUENCY   20000000
#define SPI_TOUCH_FREQUENCY   2500000

// SD dùng FSPI (SPIClass spiSD(FSPI) trong code) => TFT/Touch phải chạy HSPI để tránh xung đột.
#define USE_HSPI_PORT
#define SUPPORT_TRANSACTIONS