// =============================================================================
//  TFT_eSPI User Setup  —  ESP32-S3 + ST7796S 3.5" (320 x 480)
// =============================================================================
//
//  Project : SmartFace
//  Board   : ESP32-S3 DevKit (SPI3 / HSPI port)
//  Panel   : ST7796S 3.5" TFT, 320x480, SPI 4-wire, khong co MISO
//
//  VI TRI FILE:
//    Arduino/libraries/Setup443_S3_ST7796s_320x480.h
//    (ngang hang voi thu muc TFT_eSPI, KHONG nam trong TFT_eSPI/User_Setups/)
//
//  CACH KICH HOAT:
//    Mo TFT_eSPI/User_Setup_Select.h, chi de DUNG MOT dong include duoc bat:
//        #include <../Setup443_S3_ST7796s_320x480.h>
//    Bat 2 file setup cung luc se lam macro chong nhau va driver bi chon sai
//    trong im lang (chuoi #if/#elif uu tien ILI9341 truoc ST7796).
//
//  SO DO NOI DAY:
//    ESP32-S3            ST7796S
//    --------            -------
//    3V3         ------> VCC
//    GND         ------> GND
//    GPIO 10     ------> CS
//    GPIO 8      ------> SDA  (MOSI)
//    GPIO 9      ------> SCL  (SCLK)
//    GPIO 17     ------> DC   (RS)
//    GPIO 16     ------> RES  (RESET)
//    GPIO 18     ------> BLK  (LED backlight, qua MOSFET neu module khong co)
//    (khong noi)         SDO  (MISO - panel nay khong doc nguoc)
//
// =============================================================================


// -----------------------------------------------------------------------------
//  1. NHAN DANG SETUP
// -----------------------------------------------------------------------------
// ID nay chi dung cho muc dich bao cao trong example "Read_User_Setup".
// Khong anh huong gi den hoat dong cua thu vien.
#define USER_SETUP_ID 443


// -----------------------------------------------------------------------------
//  2. DRIVER IC VA KICH THUOC PANEL
// -----------------------------------------------------------------------------
// Chi duoc bat DUNG MOT driver. ST7796_DRIVER nap chuoi init dung cho panel nay
// (power control, frame rate, gamma). Dung ILI9341_DRIVER se lam hinh lem nhem
// vi 2 IC chi giong nhau o cac lenh co ban (0x2A / 0x2B / 0x2C).
#define ST7796_DRIVER

// Kich thuoc theo huong PORTRAIT. Doi huong bang tft.setRotation() luc runtime,
// khong sua o day.
#define TFT_WIDTH  320
#define TFT_HEIGHT 480

// Panel co chan SDA hai chieu (doc du lieu nguoc qua duong MOSI).
// ST7796S module thong dung khong ho tro -> de tat.
//#define TFT_SDA_READ

// Bu offset vung nho CGRAM. Chi can cho ST7789 / ST7735 khi vung hien thi
// nho hon vung nho cua IC. ST7796S khong can.
//#define CGRAM_OFFSET


// -----------------------------------------------------------------------------
//  3. MAU SAC VA HIEN THI
// -----------------------------------------------------------------------------
// Bat MOT trong hai dong duoi neu mau do va xanh duong bi doi cho nhau.
// Thu lan luot, khong bat ca hai.
//#define TFT_RGB_ORDER TFT_RGB    // Red-Green-Blue
//#define TFT_RGB_ORDER TFT_BGR    // Blue-Green-Red

// Bat neu mau bi dao nguoc (mau trang hien ra thanh mau den).
// Mot so lo ST7796S can INVERSION_ON, mot so khong. Thu tung cai.
//#define TFT_INVERSION_ON
//#define TFT_INVERSION_OFF

// Muc logic de BAT den nen. Phai khai bao chung voi TFT_BL o duoi.
// HIGH = 1 (da so module), LOW = 0 (mot so module co transistor dao).
#define TFT_BACKLIGHT_ON HIGH


// -----------------------------------------------------------------------------
//  4. GAN CHAN GPIO  (ESP32-S3)
// -----------------------------------------------------------------------------
// CANH BAO cho ESP32-S3:
//   - GPIO 19, 20  = USB_D- / USB_D+ (USB native). Dung se mat cong USB.
//   - GPIO 22..25  = KHONG TON TAI tren S3.
//   - GPIO 26..32  = danh cho SPI flash / PSRAM. Khong duoc dung.
//   - GPIO 0,3,45,46 = strapping pin. Tranh neu duoc.
//   - Chan FSPI native la 11/12/13 (hoac 35/36/37). Bo chan duoi day KHONG
//     phai native nen tin hieu di qua GPIO matrix -> gioi han toc do SPI.

#define TFT_CS     10    // Chip Select
#define TFT_MOSI    8    // Ghi tren silk cua module la "SDA"
#define TFT_SCLK    9    // Ghi tren silk cua module la "SCL"
#define TFT_DC     17    // Data / Command, co module ghi la "RS"
#define TFT_RST    16    // Reset. Dat -1 neu chan RES noi vao chan EN cua board

// Panel khong co chan SDO/MISO -> de tat. Neu bat va gan sai chan (vi du 20)
// se dung vao USB_D+ tren S3.
//#define TFT_MISO   20

// Chan dieu khien den nen. PHAI khai bao neu muon thu vien tu bat den khi
// goi tft.begin(). Neu comment dong nay ma chan BLK khong duoc noi cung 3V3
// thi man hinh se den thui du code chay dung.
// Luu y dong dien: den nen man 3.5" an 100-150mA, vuot kha nang cua 1 GPIO.
// Dung MOSFET (AO3400) hoac NPN + tro 1k neu module chua co san transistor.
// Muon dieu chinh do sang thi dung ledcAttachPin() tren chan nay.
// #define TFT_BL     18

// Chan CS cua IC cam ung (XPT2046). Man hinh khong co touch -> de tat.
// Neu bat, thu vien se bien dich them module touch va chiem cho tren SPI bus.
//#define TOUCH_CS   13


// -----------------------------------------------------------------------------
//  5. CHON SPI PORT   *** BAT BUOC TREN ESP32-S3 ***
// -----------------------------------------------------------------------------
// Day la nguyen nhan gay Guru Meditation Error (StoreProhibited) trong
// tft.begin() tren ESP32-S3.
//
// Co che loi:
//   Khong bat USE_HSPI_PORT  ->  SPI_PORT = FSPI
//   Tren S3, Arduino core dinh nghia FSPI = 0
//   TFT_eSPI dung SPI_PORT de TINH DIA CHI thanh ghi: REG_SPI_BASE(0) = NULL
//   -> con tro NULL -> ghi vao NULL -> StoreProhibited -> panic + reboot
//
// Ban chat: co hai he danh so khac nhau cho cung ngoai vi. Macro FSPI/HSPI cua
// Arduino la INDEX BUS (0, 1), con TFT_eSPI can SO HIEU PERIPHERAL (2, 3) theo
// TRM. Bat USE_HSPI_PORT se gan SPI_PORT = 3 (peripheral SPI3), la gia tri dung.
//
// Tren ESP32-S3: SPI0/SPI1 danh cho flash va PSRAM, chi SPI2/SPI3 dung duoc.
#define USE_HSPI_PORT


// -----------------------------------------------------------------------------
//  6. TOC DO SPI
// -----------------------------------------------------------------------------
// Chi bat MOT dong. Bo chan 8/9/10 khong phai FSPI native nen tin hieu qua
// GPIO matrix, kem theo do dai day jumper -> khong nen de 80MHz ngay tu dau.
//
// Quy trinh nen theo: chay on dinh o 40MHz truoc, roi nang dan. Trieu chung khi
// qua nhanh la pixel rac, vach ngang, hoac man hinh khong init duoc.
//#define SPI_FREQUENCY  20000000    // 20 MHz - an toan nhat, dung khi debug
//#define SPI_FREQUENCY  27000000    // 27 MHz
#define SPI_FREQUENCY  40000000      // 40 MHz - dang dung, on dinh
//#define SPI_FREQUENCY  80000000     // 80 MHz - chi khi day rat ngan (<10cm)

// Toc do khi DOC tu TFT. Neu khong khai bao, thu vien lay luon gia tri cua
// SPI_FREQUENCY (40MHz) - qua nhanh cho thao tac doc. Nen giu 20MHz.
#define SPI_READ_FREQUENCY  20000000

// Toc do SPI cho IC cam ung XPT2046 (yeu cau toi da 2.5MHz).
// Chi can khi da bat TOUCH_CS o Section 4.
//#define SPI_TOUCH_FREQUENCY  2500000


// -----------------------------------------------------------------------------
//  7. FONT
// -----------------------------------------------------------------------------
// Comment lai nhung font khong dung de tiet kiem FLASH. Bat het khoang 17KB.
#define LOAD_GLCD    // Font 1. Adafruit 8 pixel goc, ~1820 bytes
#define LOAD_FONT2   // Font 2. Cao 16 pixel, ~3534 bytes, 96 ky tu
#define LOAD_FONT4   // Font 4. Cao 26 pixel, ~5848 bytes, 96 ky tu
#define LOAD_FONT6   // Font 6. Cao 48 pixel, ~2666 bytes, chi 1234567890:-.apm
#define LOAD_FONT7   // Font 7. 7-segment 48 pixel, ~2438 bytes, chi 1234567890:.
#define LOAD_FONT8   // Font 8. Cao 75 pixel, ~3256 bytes, chi 1234567890:-.
//#define LOAD_FONT8N  // Font 8 ban hep hon, de 3 chu so vua man 160 pixel
#define LOAD_GFXFF   // FreeFonts. Mo duong dung 48 font Adafruit_GFX + font tu tao

// Anti-aliased font nap tu SPIFFS/LittleFS/FLASH. Tat se tiet kiem ~20KB
// nhung mat luon ca he thong filing cho font.
#define SMOOTH_FONT


// -----------------------------------------------------------------------------
//  8. TUY CHON KHAC
// -----------------------------------------------------------------------------
// SPI Transactions can thiet khi co thiet bi SPI khac dung chung bus (the SD,
// module RF...). Tren ESP32 thu vien TU DONG bat de dung HAL mutex, nen khai
// bao o day khong co tac dung.
//#define SUPPORT_TRANSACTIONS
