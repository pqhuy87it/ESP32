Mạch trong ảnh là **BIOSEC TM1026M** (silkscreen `TZM1026_V1.0`, chip `TA0982`/`TA0702` là chip thuật toán tự phát triển của hãng 图正/BIOSEC) — module vân tay **điện dung bán dẫn all-in-one**, giao tiếp **UART TTL**. Cảm biến nằm ở mặt bên kia của board, connector trắng 6 chân (P1) là cổng giao tiếp ra ngoài.

**Trả lời ngắn: dùng được với ESP32**, vì nó là UART 3.3V — khớp trực tiếp mức logic ESP32, không cần level shifter.

## Đấu nối

Theo tài liệu cộng đồng cho TM1026M: chân 1 và 3 lên 3.3V, chân 6 xuống GND, TX/RX nối chéo với UART của MCU; chân 2 có thể để trống mà vẫn giao tiếp lệnh bình thường (chân 2 khả năng là nguồn cho mạch touch-detect).

| Module | ESP32 |
|---|---|
| VCC | **3.3V** (tuyệt đối không 5V) |
| GND | GND |
| TX | RX (ví dụ GPIO16) |
| RX | TX (ví dụ GPIO17) |

Dòng tiêu thụ rất nhỏ — khoảng 40mA khi đăng ký/nhận dạng vân tay, LDO trên board ESP32 dư sức. Nhưng bạn cần **cáp đúng connector** (loại 1.0mm hoặc 1.25mm 6 pin), nhớ hỏi shop có kèm cáp không.

## Vấn đề thật sự: tài liệu

Đây mới là chỗ đau, không phải phần cứng:

- **Không có thư viện Arduino chính thức.** Repo `Weixiang/TM1026-Fingerprint-Sensor-Library` trên GitHub thực chất là bản fork đổi tên của thư viện Adafruit (mô tả copy nguyên văn), protocol khác nên đừng tin. Bạn sẽ phải tự implement protocol theo tài liệu.
- Tài liệu đều là **do cộng đồng chia sẻ lại**, không phải từ hãng. Hai nguồn đáng xem:
  - `github.com/yeziruo233/BIOSEC-TM1026M` — spec + protocol + `TA_ComTool` (phần mềm test trên PC)
  - `github.com/atx25/TM-1206` — bộ lệnh giao tiếp, cùng họ protocol
- Tập lệnh **khá hạn chế**: enroll, delete, query, match là dùng được; còn trích xuất ảnh/đặc trưng vân tay thì trả về vài trăm byte không có thông tin kích thước nên không parse được. Nếu bạn chỉ cần "quẹt tay → mở khoá" thì đủ; muốn xử lý ảnh vân tay thì không.
- Lưu ý thêm: nhiều module này là hàng tồn/loại ra từ dây chuyền, version 1.0, chưa từng được hãng bán rộng rãi — nên chất lượng lô hàng khá xổ số, và có thể firmware đã bị customize cho một sản phẩm cụ thể.

## Quy trình tôi đề xuất

1. Mua thêm **USB-TTL (CH340/CP2102)**, test module bằng `TA_ComTool` trên PC trước. Xác nhận module còn sống và biết chắc baudrate/pinout đúng.
2. Bắt frame lệnh từ tool đó (hoặc logic analyzer), rồi mới port sang ESP32 với `HardwareSerial` — không dùng `SoftwareSerial`.
3. Dùng UART1/UART2, tránh UART0 để không đụng Serial monitor.

## Nếu muốn đường dễ hơn

Cùng tầm giá nhưng tài liệu đầy đủ và có thư viện sẵn:

- **R503 / R502** — điện dung, vòng LED, dùng protocol ZhianTec nên `Adafruit_Fingerprint` chạy luôn. Đây là lựa chọn hợp lý nhất nếu bạn muốn xong nhanh.
- **AS608 / R307** — quang học, rẻ nhất, tài liệu tiếng Việt đầy rẫy, nhưng cần 5V cho R307.
- **HLK-ZW101** từ Hi-Link — module bán dẫn all-in-one, chip thuật toán RISC-V low-power, cảm biến chủ động RF hỗ trợ phát hiện ngón tay ở chế độ tiết kiệm điện, không cần chip touch-detect ngoài. Hãng có datasheet đầy đủ, và cái low-power finger detection đó rất phù hợp nếu bạn định chạy pin.

Với TM1026M thì coi như một project reverse-engineering nhỏ — thú vị nếu bạn thích mò protocol, nhưng đừng chọn nó nếu đang cần thứ hoạt động trong buổi tối nay.
