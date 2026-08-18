## Bảng thông số

| | ESP32-S3 | Raspberry Pi Pico 2 W (RP2350) |
|---|---|---|
| CPU | 2× Xtensa LX7 @ 240 MHz | 2× Cortex-M33 @ 150 MHz (hoặc 2× RISC-V Hazard3, chọn lúc boot) |
| FPU | Single-precision | Single-precision + DSP/SIMD, có TrustZone |
| SRAM | 512 KB | 520 KB |
| PSRAM | 2–8 MB (Octal/Quad, tích hợp trong module) | RP2350 hỗ trợ qua QSPI CS thứ 2, nhưng **board Pico 2 W không hàn sẵn** |
| Flash | 4–32 MB tùy module | 4 MB onboard |
| WiFi | 802.11 b/g/n tích hợp trong SoC | CYW43439 qua SPI (chậm hơn đáng kể) |
| BT | BLE 5.0 + Mesh | BLE 5.2 |
| USB | OTG full-speed (device + host) + USB-Serial/JTAG | USB 1.1 device + host |
| Màn hình | **LCD_CAM**: parallel 8/16-bit Intel-8080 + DMA, camera DVP | **HSTX** (DVI/DPI ra GPIO 12–19), hoặc PIO |
| Đặc sản | Vector instruction cho AI/DSP, ULP RISC-V coprocessor, SDMMC host, TWAI (CAN), touch sensor | **12× PIO state machine** (3 block), 12 DMA channel |
| ADC | 2× 12-bit, 20 ch (nổi tiếng phi tuyến) | 1× 12-bit, 4 ch, 500 ksps (sạch hơn) |
| Crypto HW | AES/SHA/RSA/HMAC, secure boot, flash encryption | SHA-256, signed boot, OTP, glitch detector (không có AES engine) |
| Deep sleep | ~7 µA (ULP vẫn chạy được) | RP2350 dormant thấp, nhưng module WiFi + regulator trên board kéo lên vài trăm µA–mA |
| Nguồn | 3.3 V | 1.8–5.5 V (có buck onboard) |
| GPIO | ~45 | 26 (trên board) |

## Nên dùng ESP32-S3 khi

- **Nhiều TFT / màn hình lớn**: PSRAM 8 MB cho double-buffer + LCD_CAM parallel + DMA. Đây là lý do cái macro-pad 3 màn NV3023 và các project GIF player chạy được — Pico 2 W với 520 KB SRAM sẽ hết RAM ngay khi muốn buffer 2 khung 320×240.
- **Mạng là trọng tâm**: WiFi throughput cao hơn nhiều lần (MAC nằm trong SoC), OTA, HTTPS, MQTT, web server. Pico 2 W đi qua SPI nên nghẽn cổ chai.
- **Camera** (DVP), **audio/wake-word** (I2S + ESP-SR), **SD card** qua SDMMC 4-bit.
- **Chạy pin dài hạn** với ULP làm sensor polling khi CPU ngủ.
- **Cần crypto/secure boot đầy đủ** cho firmware thương mại.
- Cần nhiều GPIO, hoặc cần CAN bus.

## Nên dùng Pico 2 W khi

- **Timing chính xác / giao thức lạ**: PIO là thứ ESP32 không có tương đương. Bit-bang DVI, VGA, WS2812 số lượng lớn, quadrature encoder, protocol reverse-engineer, logic analyzer, emulate bus của thiết bị cũ — PIO làm bằng hardware, không tốn CPU cycle.
- **Real-time deterministic**: không có WiFi stack tranh core, không có `esp_timer` hay Bluetooth ISR chen ngang. Với motor control hoặc sampling đều đặn, Pico dự đoán được hơn.
- **USB host**: TinyUSB stack trên Pico chín hơn — cắm bàn phím/chuột USB vào MCU thì Pico dễ hơn S3 nhiều.
- **Điện áp vào linh hoạt**: cấp trực tiếp 5 V hoặc 2S Li-ion qua buck… thực ra 2S (8.4 V) vượt ngưỡng 5.5 V, cần buck ngoài — nhưng 1S Li-ion cấp trực tiếp vào VSYS thì Pico ăn được, ESP32 thì phải qua LDO.
- **Debug tử tế**: SWD 2 dây + `picoprobe`, breakpoint/watchpoint thật. JTAG trên S3 dùng được nhưng lằng nhằng hơn.
- **Toolchain đơn giản**: C SDK gọn, MicroPython chất lượng cao hơn hẳn MicroPython trên ESP32.

## Không nên dùng

**ESP32-S3 không phù hợp khi:**
- Cần timing cứng ở mức µs mà vẫn bật WiFi/BLE — WiFi task sẽ làm jitter.
- Cần ADC chính xác — ADC của ESP32 phi tuyến, nhiễu, nhiều kênh còn xung đột với WiFi (ADC2).
- Cần Bluetooth Classic (A2DP, SPP) — S3 **không có**, chỉ BLE. Phải dùng ESP32 cổ điển.
- Cần DAC analog — S3 đã bỏ DAC (ESP32 gốc có 2 kênh).
- Deep sleep cực thấp mà vẫn phải giữ nhiều state — RTC RAM chỉ 16 KB.

**Pico 2 W không phù hợp khi:**
- Cần framebuffer lớn hoặc nhiều màn hình — thiếu PSRAM trên board là rào chắn thật.
- App network-heavy, streaming, hoặc nhiều kết nối đồng thời.
- Cần ultra-low-power battery node — hệ sinh thái sleep của Pico 2 W kém hoàn thiện hơn ESP32 nhiều, và module CYW43 tiêu thụ đáng kể.
- Cần WiFi + nhiều thứ khác cùng lúc: CYW43 chiếm SPI riêng và một số GPIO nội bộ.
- Cần OTA update qua WiFi ra production — ESP-IDF có sẵn, Pico phải tự dựng.

Một lưu ý: RP2350 stepping đầu (A2, chính là lô Pico 2/Pico 2 W đầu tiên) có errata **E9** — GPIO ở chế độ input với pull-down có thể bị latch lên ~2.2 V khi thả nổi, ảnh hưởng đọc nút bấm và mạch touch/ADC. Workaround là dùng pull-down ngoài ~8.2 kΩ hoặc thao tác pad register. Nếu định làm nút bấm nhiều thì nên check silicon revision của board đang có.

Cho hướng đang làm — port kiến trúc 3 màn 240×240 sang ESP32 N16R8 — S3 là lựa chọn đúng, không có lý do gì đổi sang Pico ở đây. Pico 2 W đáng để cầm khi nào cần reverse-engineer protocol hoặc bit-bang cái gì lạ.
