Hai con đều là RISC-V, chân cắm gần giống nhau, nhưng C6 là thế hệ sau và khác nhiều hơn tưởng.

## Bảng so sánh

| | ESP32-C3 | ESP32-C6 |
|---|---|---|
| **CPU** | 1× RISC-V 32-bit @160 MHz | 1× HP RISC-V @160 MHz **+ 1× LP core @20 MHz** |
| **SRAM** | 400 KB | 512 KB (HP) + 16 KB (LP) |
| **PSRAM** | Không hỗ trợ | Không hỗ trợ |
| **Wi-Fi** | Wi-Fi 4 (b/g/n) | **Wi-Fi 6 (802.11ax)** + TWT |
| **Bluetooth** | BLE 5.0 | BLE 5.3 |
| **802.15.4** | ❌ | ✅ **Zigbee 3.0 / Thread / Matter** |
| **GPIO** | 22 | 30 (QFN40) |
| **ADC** | ADC1 (5ch) + ADC2 (1ch, xung đột Wi-Fi) | **Chỉ ADC1, 7ch** — bỏ hẳn ADC2 |
| **LP peripheral** | Không có ULP | LP UART, LP I2C, LP GPIO, LP timer |
| **Thêm mới** | — | SDIO 2.0 slave, PARLIO, ETM, ECC/ECDSA accelerator |
| **Strapping pins** | 2, 8, 9 | 4, 5, 8, 9, 15 |
| **Giá / size** | Rẻ hơn, QFN32 | Đắt hơn ~1.5–2×, QFN40 |

Giống nhau: USB Serial/JTAG tích hợp (nạp code + Serial qua USB không cần CH340), 1 SPI dùng được cho ngoại vi (SPI2/FSPI), TWAI/CAN, không có DAC, không có USB-OTG.

## Điểm đáng chú ý thực tế

**Cái đáng tiền nhất của C6 là 802.15.4.** Nó biến C6 thành Zigbee end-device hoặc Thread/Matter node thật sự — cắm thẳng vào Zigbee2MQTT của bạn mà không cần bridge Wi-Fi. C3 thì không bao giờ làm được.

**LP core là thứ C3 thiếu hẳn.** C3 không có ULP coprocessor, muốn đọc sensor định kỳ là phải wake full core. C6 để LP core @20 MHz đọc LP I2C rồi ngủ tiếp — với dự án 18650 chạy pin, chênh lệch tuổi thọ rất rõ.

**Bẫy khi port code từ C3 sang C6:**
- Bảng chân hoàn toàn khác, strapping pin khác → setup TFT_eSPI phải viết lại, không copy được.
- Mất ADC2, nên GPIO nào đọc analog được cũng đổi hết.
- C6 cần arduino-esp32 core **3.x** trở lên (ESP-IDF 5.1+). Core 2.x không biết C6 là gì.
- TFT_eSPI trên C6 phải dùng bản mới từ GitHub, bản Library Manager cũ chưa có target C6 trong `TFT_eSPI_ESP32.h` → lỗi lúc compile hoặc treo lúc `tft.init()`.

**Khi nào vẫn nên chọn C3:** chỉ cần Wi-Fi + BLE, chạy cắm điện, muốn rẻ và nhỏ. SuperMini C3 hiện quá phổ biến và tài liệu nhiều hơn hẳn.
