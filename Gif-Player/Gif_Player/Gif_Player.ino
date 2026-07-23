#include <TFT_eSPI.h>
#include <SPI.h>
#include <SD.h>
#include <AnimatedGIF.h>

#define SD_CS 16
#define SD_MOSI 17
#define SD_MISO 18
#define SD_SCK 8

TFT_eSPI tft = TFT_eSPI();
SPIClass spiSD(FSPI);
AnimatedGIF gif;

uint16_t *canvas;
#define DW 320
#define DH 240

String gifFiles[100];
int fileCount = 0;
int currentIndex = 0;
int menuOffset = 0;
bool autoMode = true;
bool forceNext = false;

int frame_min_x = DW, frame_min_y = DH;
int frame_max_x = 0, frame_max_y = 0;
bool frame_drawn = false;

// Theo doi file handle dang mo boi callback (tranh ro file descriptor)
File *gifFileHandle = NULL;

enum State { MENU,
             PLAYING };
State currentState = PLAYING;

void GIFDraw(GIFDRAW *pDraw) {
  uint8_t *s;
  uint16_t *d, *usPalette;
  int x, y, iWidth;

  iWidth = pDraw->iWidth;
  if (iWidth + pDraw->iX > DW) iWidth = DW - pDraw->iX;
  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y;

  if (y >= DH || pDraw->iX >= DW || iWidth < 1) return;

  if (pDraw->iX < frame_min_x) frame_min_x = pDraw->iX;
  if (pDraw->iX + iWidth > frame_max_x) frame_max_x = pDraw->iX + iWidth;
  if (y < frame_min_y) frame_min_y = y;
  if (y > frame_max_y) frame_max_y = y;
  frame_drawn = true;

  s = pDraw->pPixels;
  d = &canvas[y * DW + pDraw->iX];

  if (pDraw->ucDisposalMethod == 2) {
    for (x = 0; x < iWidth; x++) {
      if (s[x] == pDraw->ucTransparent) s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }

  if (pDraw->ucHasTransparency) {
    uint8_t ucTransparent = pDraw->ucTransparent;
    for (x = 0; x < iWidth; x++) {
      if (s[x] != ucTransparent) {
        uint16_t col = usPalette[s[x]];
        d[x] = (col >> 8) | (col << 8);
      }
    }
  } else {
    for (x = 0; x < iWidth; x++) {
      uint16_t col = usPalette[s[x]];
      d[x] = (col >> 8) | (col << 8);
    }
  }
}

void *GIFOpenFile(const char *fname, int32_t *pSize) {
  // Neu con file cu chua dong (lan open truoc that bai) -> dong truoc
  if (gifFileHandle) {
    gifFileHandle->close();
    delete gifFileHandle;
    gifFileHandle = NULL;
  }
  File f = SD.open(fname);
  if (f) {
    *pSize = f.size();
    gifFileHandle = new File(std::move(f));
    return (void *)gifFileHandle;
  }
  return NULL;
}

void GIFCloseFile(void *pHandle) {
  File *f = static_cast<File *>(pHandle);
  if (f) {
    f->close();
    delete f;
    if (f == gifFileHandle) gifFileHandle = NULL;
  }
}

int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {
  int32_t iBytesRead = iLen;
  File *f = static_cast<File *>(pFile->fHandle);
  if ((pFile->iSize - pFile->iPos) < iLen) iBytesRead = pFile->iSize - pFile->iPos;
  if (iBytesRead <= 0) return 0;
  iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
  pFile->iPos = f->position();
  return iBytesRead;
}

int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition) {
  File *f = static_cast<File *>(pFile->fHandle);
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  return pFile->iPos;
}

// Dong file GIF con dang mo (dung khi open that bai de tranh ro descriptor)
void closeStrayHandle() {
  if (gifFileHandle) {
    gifFileHandle->close();
    delete gifFileHandle;
    gifFileHandle = NULL;
  }
}

void drawMenu() {
  tft.fillScreen(TFT_BLACK);
  tft.drawRoundRect(5, 5, 310, 230, 8, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("GIF SELECTOR", 160, 25, 4);

  for (int i = 0; i < 5; i++) {
    int fileIdx = i + menuOffset;
    if (fileIdx >= fileCount) break;
    int yPos = 60 + (i * 30);

    if (fileIdx == currentIndex) {
      tft.fillRoundRect(15, yPos - 12, 210, 25, 4, TFT_NAVY);
      tft.setTextColor(TFT_CYAN);
    } else {
      tft.setTextColor(TFT_WHITE);
    }
    tft.setTextDatum(ML_DATUM);
    tft.drawString(gifFiles[fileIdx].substring(1, 20), 25, yPos, 2);
  }

  tft.fillRoundRect(240, 60, 65, 40, 5, TFT_DARKGREY);
  tft.fillRoundRect(240, 110, 65, 40, 5, TFT_DARKGREY);
  tft.fillRoundRect(240, 170, 65, 45, 5, autoMode ? TFT_GREEN : TFT_MAROON);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("UP", 272, 80, 2);
  tft.drawString("DOWN", 272, 130, 2);
  tft.drawString(autoMode ? "AUTO" : "SINGL", 272, 192, 2);
}

void handleTouch() {
  uint16_t x, y;
  if (tft.getTouch(&x, &y)) {
    Serial.print("TOUCH x=");
    Serial.print(x);
    Serial.print(" y=");
    Serial.println(y);

    if (currentState == PLAYING) {
      if (x < 80) {
        currentIndex = (currentIndex - 1 + fileCount) % fileCount;
        forceNext = true;
      } else if (x > 240) {
        currentIndex = (currentIndex + 1) % fileCount;
        forceNext = true;
      } else {
        currentState = MENU;
        drawMenu();
      }
      delay(200);
    } else {
      if (x > 230) {
        if (y < 100) {
          if (menuOffset > 0) menuOffset--;
        } else if (y < 160) {
          if (menuOffset < fileCount - 5) menuOffset++;
        } else {
          autoMode = !autoMode;
        }
        drawMenu();
      } else if (x < 220 && y > 50) {
        int selection = ((y - 50) / 30) + menuOffset;
        if (selection < fileCount) {
          currentIndex = selection;
          currentState = PLAYING;
          forceNext = true;
          tft.fillScreen(TFT_BLACK);
          memset(canvas, 0, DW * DH * 2);
        }
      }
      delay(150);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n===== BOOT =====");

  if (!psramInit()) {
    Serial.println("PSRAM Fail");
    while (1)
      ;
  }
  canvas = (uint16_t *)ps_malloc(DW * DH * 2);
  memset(canvas, 0, DW * DH * 2);

  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  delay(100);
  uint16_t calData[5] = { 419, 3453, 344, 3399, 1 };
  tft.setTouch(calData);
  delay(100);

  spiSD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spiSD)) {
    tft.println("SD Error");
    Serial.println("SD Error");
    while (1)
      ;
  }

  File root = SD.open("/");
  while (File entry = root.openNextFile()) {
    String n = entry.name();
    if (n.endsWith(".gif") || n.endsWith(".GIF")) {
      if (fileCount < 100) gifFiles[fileCount++] = "/" + n;
    }
    entry.close();
  }
  root.close();

  Serial.print("So file GIF tim thay: ");
  Serial.println(fileCount);

  gif.begin(LITTLE_ENDIAN_PIXELS);
}

void loop() {
  if (currentState == PLAYING) {
    if (fileCount > 0) {
      forceNext = false;

      if (gif.open(gifFiles[currentIndex].c_str(), GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {

        tft.fillScreen(TFT_BLACK);
        frame_min_x = DW;
        frame_max_x = 0;
        frame_min_y = DH;
        frame_max_y = 0;
        frame_drawn = false;

        while (gif.playFrame(true, NULL)) {

          if (frame_drawn) {
            int w = frame_max_x - frame_min_x;

            tft.startWrite();
            tft.setWindow(frame_min_x, frame_min_y, frame_max_x - 1, frame_max_y);
            for (int y = frame_min_y; y <= frame_max_y; y++) {
              tft.pushPixels(&canvas[y * DW + frame_min_x], w);
            }
            tft.endWrite();

            frame_min_x = DW;
            frame_max_x = 0;
            frame_min_y = DH;
            frame_max_y = 0;
            frame_drawn = false;
          }

          handleTouch();
          if (forceNext || currentState != PLAYING) break;
        }
        gif.close();

        // Sau khi phat xong 1 GIF: giu lai ~2s va VAN quet cham
        // De cham luon phan hoi ke ca khi GIF ngan/het frame
        unsigned long holdStart = millis();
        while (millis() - holdStart < 2000) {
          handleTouch();
          if (forceNext || currentState != PLAYING) break;
          delay(10);
        }

        if (autoMode && !forceNext && currentState == PLAYING) {
          currentIndex = (currentIndex + 1) % fileCount;
          memset(canvas, 0, DW * DH * 2);
        }
      } else {
        // Open that bai: dong file bi ro, van quet cham roi sang file ke tiep
        Serial.print("Loi mo GIF: ");
        Serial.println(gifFiles[currentIndex]);
        closeStrayHandle();
        handleTouch();
        currentIndex = (currentIndex + 1) % fileCount;
        delay(500);
      }
    } else {
      // Khong co file nao -> van quet cham de khong bi treo
      handleTouch();
      delay(50);
    }
  } else {
    handleTouch();
  }
}
