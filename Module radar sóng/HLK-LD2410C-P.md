## HLK-LD2410C-P — 24GHz mmWave human presence radar

Đây là module radar sóng milimet của **Hi-Link (Shenzhen Hi-Link Electronic)**, dùng để phát hiện **sự hiện diện của người** (không chỉ chuyển động như PIR).

**Hậu tố "-P"** = phiên bản đã hàn sẵn **pin header 2.54mm** (bản LD2410C thường chỉ có pad/lỗ trống). Lưu ý LD2410 và LD2410B dùng pitch 1.27mm, còn bản C dùng pitch 2.54mm chuẩn nên cắm breadboard/header dễ hơn — tiện cho anh đấu với ESP32.

### Thông số chínhKích thước 22 × 16 mm, Vin 5–12V DC, dòng ~79mA, nhiệt độ hoạt động −40 đến +85°C

| Hạng mục | Giá trị |
|---|---|
| Nguyên lý | FMCW (Frequency-Modulated Continuous Wave), băng ISM 24GHz |
| Radar SoC | S3KM1110 (chip lớn ở mặt trước, giữa 2 patch antenna 1T1R) |
| Tầm phát hiện | tối đa 5m, resolution 0.75m (chia 8 distance gate) |
| Góc quét | ±60 độ |
| Giao tiếp | UART + 1 chân OUT digital |
| Baud mặc định | 256000, 8N1 |
| Mức logic | UART là 3.3V và KHÔNG chịu được 5V; chân OUT là GPIO 3.3V, lên HIGH khi có người trong vùng cấu hình |
| Cấu hình | qua Bluetooth (app) hoặc serial, có tool cấu hình trực quan: khoảng cách, sensitivity từng gate, thời gian delay khi không có người |

Chip nhỏ có logo JL ở mặt sau + thạch anh 24MHz là **BLE MCU** (dùng cho app `HLKRadarTool` trên điện thoại). 4 pad `GND / DP / DM / VIN` ở mép dưới mặt sau là **USB test pad** để nạp lại firmware con BLE này — bình thường không cần dùng.

### Pinout (silkscreen mặt trước, từ trên xuống)

```
TX  → RX của MCU (3.3V)
RX  → TX của MCU
OUT → GPIO bất kỳ (3.3V, HIGH = có người)
GND → GND
VCC → 5V (module có LDO onboard)
```

### Đấu với ESP32 của anh

Nối trực tiếp được, không cần level shifter vì cả hai đều 3.3V logic — chỉ cấp VCC 5V. Về software:

- **ESPHome**: có component `ld2410` native, khai báo `uart` + `ld2410:` là xong, ra đủ sensor (presence, moving/still distance, energy từng gate).
- **Arduino / ESP-IDF**: dùng lib `ncmreynolds/ld2410` hoặc `MyLD2410` (bản này API sạch hơn, hỗ trợ đọc/ghi config đầy đủ).
- Nếu chỉ cần bật/tắt đèn thì dùng chân `OUT` là đủ, khỏi cần UART.

### Vài lưu ý thực tế

- 24GHz **xuyên được nhựa, thạch cao, kính** → dễ bị trigger bởi người đi ngoài cửa hoặc phòng bên. Cần giảm sensitivity ở các gate xa.
- Rất nhạy với **quạt, rèm bay, luồng gió điều hòa, máy giặt rung** → false positive kinh điển.
- **Không để kim loại trước antenna.** Liên quan tới K1C của anh: in vỏ bằng PLA/PETG/ABS bình thường thì OK, nhưng **tránh filament carbon fiber hoặc có phụ gia dẫn điện**, và đừng sơn nhũ kim loại. Hi-Link có phát hành tài liệu hướng dẫn thiết kế radome (`毫米波传感器天线罩设计指南`) trên trang sản phẩm, nên đọc trước khi thiết kế vỏ trên Fusion.
- Datasheet + protocol UART (`LD2410C 串口通信协议 V1.09`) tải ở hlktech.com hoặc lấy PDF từ LCSC (part `C19723500`).

### So sánh nhanh trong họ LD24xx

| Model | Điểm khác |
|---|---|
| LD2410C | Presence cơ bản, rẻ nhất, pitch 2.54mm |
| LD2410S | Bản ultra-low-power, chạy pin |
| LD2412 | Góc rộng hơn (~75°), tầm ~9m |
| LD2450 | **Tracking tọa độ X/Y**, tối đa 3 target — nếu anh cần biết người ở *đâu* chứ không chỉ có/không |

Anh cần em viết sẵn file code ESP32 (Arduino hoặc ESPHome yaml) đọc UART và parse distance/energy từng gate không? Hoặc nếu tính tích hợp vào SmartFace để tự bật màn hình khi có người lại gần thì em gợi ý luôn cách wiring chung UART.
