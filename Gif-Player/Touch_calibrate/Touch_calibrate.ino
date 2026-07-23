#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  delay(2000);   // Đợi mở Serial Monitor kịp
  Serial.println("\n\n===== BOOT START =====");

  Serial.print("SETUP_INFO: ");
  Serial.println(USER_SETUP_INFO);   // In ra file setup đang dùng -> kiểm tra đúng setup chưa
  Serial.print("TFT_WIDTH x HEIGHT: ");
  Serial.print(TFT_WIDTH); Serial.print(" x "); Serial.println(TFT_HEIGHT);

  Serial.println(">> Truoc tft.init()");
  tft.init();
  Serial.println(">> Sau tft.init() OK");

  tft.setRotation(3);
  Serial.print(">> Sau setRotation, width="); Serial.print(tft.width());
  Serial.print(" height="); Serial.println(tft.height());

  // Test hien thi TRUOC khi dung den touch, de tach loi man hinh vs loi touch
  Serial.println(">> Test fill mau...");
  tft.fillScreen(TFT_RED);
  delay(500);
  tft.fillScreen(TFT_GREEN);
  delay(500);
  tft.fillScreen(TFT_BLUE);
  delay(500);
  Serial.println(">> Neu ban thay do/xanh la/xanh duong nhap nhay => MAN HINH OK");

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawCentreString("Screen OK", tft.width()/2, tft.height()/2, 2);
  delay(1500);

  Serial.println(">> Truoc touch_calibrate()");
  touch_calibrate();
  Serial.println(">> Sau touch_calibrate() OK");

  tft.fillScreen(TFT_BLACK);
  tft.drawCentreString("Touch screen to test!", tft.width()/2, tft.height()/2, 2);
  Serial.println("===== SETUP DONE =====");
}

void loop(void) {
  uint16_t x = 0, y = 0;
  bool pressed = tft.getTouch(&x, &y);

  if (pressed) {
    Serial.print("Touch x="); Serial.print(x);
    Serial.print(" y="); Serial.println(y);
    tft.fillCircle(x, y, 2, TFT_WHITE);
  }
}

void touch_calibrate() {
  uint16_t calData[5];

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Touch corners as indicated");
  tft.setTextFont(1);
  tft.println();

  Serial.println(">>>> Bat dau calibrateTouch - cham 4 goc theo mui ten");
  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
  Serial.println(">>>> calibrateTouch xong");

  Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = { ");
  for (uint8_t i = 0; i < 5; i++) {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }
  Serial.println(" };");
  Serial.println("  tft.setTouch(calData);");
  Serial.println();

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  delay(2000);
}