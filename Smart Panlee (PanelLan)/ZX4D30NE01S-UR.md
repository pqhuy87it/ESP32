Mạch trong ảnh là **Smart Panlee (PanelLan) ZX4D30NE01S-UR** — dòng silkscreen `ZX4D30NS-E01-B11-UR` là mã PCB revision, tên thương mại đầy đủ là **ZX4D30NE01S-UR-4827**, codename nội bộ là **SC02**. Nhà sản xuất: Shenzhen QM Smart Panlee Technology (thương hiệu liên quan Wireless-Tag / WT32).

## Thông số chính

| Hạng mục | Chi tiết |
|---|---|
| MCU module | WT32-S3-WROVER-N16R8 → ESP32-S3, 16MB Flash + 8MB Octal PSRAM |
| Màn hình | 4.3", 480×272, giao tiếp RGB parallel, driver IC ST7701 |
| Touch | Resistive TP qua I2C (TP_SCL = GPIO39, TP_SDA = GPIO38) — nhưng thư viện PanelLan liệt kê SC02 là không có touch, chỉ có RS485 |
| Nguồn | 5V |
| Kết nối | USB Type-C, UART, RS485, GPIO mở rộng |

## Các chân trên board (khớp với ảnh)

**Cụm `V O E T R G`** (test point debug, góc trái trên): +5V, BOOT (IO0), ESP_EN, TXD, RXD, GND

**CN3 – `+5V / RXD / TXD / IO1 / IO2 / GND`** (Extended GPIO):
RXD1 = GPIO41 (chỉ input), TXD1 = GPIO4 (chỉ output), IO1 = GPIO1 (input only), IO2 = GPIO2 (output only). Cùng bộ tín hiệu này cũng ra ở connector FPC 10 pin.

**CN1 – `+5V / GND / B / A`** (RS485): TXD = GPIO0, RXD = GPIO39, DE flow control = GPIO38 — chú ý chân RS485 trùng với chân touch I2C, nên thực tế chỉ dùng được một trong hai.

**LCD RGB**: CLK = GPIO14, HSYNC = 47, VSYNC = 48, DE = 40, DISP = 21, BL_EN = 45 (active high), còn lại là 16 chân data RGB565.

## Lập trình

Có 3 hướng, tùy mức độ bạn muốn kiểm soát:

1. **Arduino** — thư viện chính chủ `PanelLan` (cài từ Library Manager), dựa trên LovyanGFX. Chọn board bằng `PanelLan tft(BOARD_SC02);`. Repo: `github.com/smartpanle/PanelLan_esp32_arduino`
2. **ESP-IDF** — `QMSD-ESP32-BSP`, gồm driver màn hình, touch và UI framework (LVGL)
3. **GUI online kéo–thả** — nền tảng 8ms.xyz, kiểu MIT App Inventor, dùng để dựng UI nhanh không cần code. Đây là lý do board được marketing như "serial screen": nạp firmware sẵn rồi điều khiển UI qua UART từ MCU khác.

Datasheet đầy đủ: `doc.panel-tag.com/_static/espboard/ESP32S3/ZX4D30NE01S-UR-4827.pdf`, doc board: `doc.panel-tag.com/ESP32-S3/board.html`

## Vài lưu ý thực tế

Với kinh nghiệm ESP32-S3 + TFT_eSPI của bạn thì cần biết trước: **TFT_eSPI không hỗ trợ RGB parallel LCD** — nó chỉ làm SPI/8080. Board này bắt buộc phải dùng LovyanGFX (qua PanelLan) hoặc `esp_lcd_rgb_panel` của IDF. Ngoài ra RGB panel trên ESP32-S3 hay bị screen tearing, cần dùng arduino-esp32 SDK có bật bounce buffer — cấu hình PSRAM Octal 80MHz và bounce buffer là điều kiện gần như bắt buộc để hình mượt.

Bạn định dùng board này cho project nào? Nếu là cái dashboard/clock UI đang làm thì 480×272 RGB sẽ cho fill rate tốt hơn hẳn mấy con SPI, nhưng bù lại ăn RAM và chân GPIO gần hết.
