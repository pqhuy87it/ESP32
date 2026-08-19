Hoàn toàn được — đây là kiến trúc IoT rất phổ biến. Vấn đề chính chỉ là chọn **transport** giữa phone và ESP32. Có 4 hướng, mỗi hướng hợp một use case khác nhau:

**1. BLE (GATT server trên ESP32)**
Phone kết nối trực tiếp, không cần WiFi/router/internet. Tốt nhất cho thiết bị chạy pin (ESP32 có thể deep sleep giữa các lần advertise). Nhược điểm: chỉ 1 phone kết nối tại 1 thời điểm, tầm ~10m, payload nhỏ, và Android cần runtime permission `BLUETOOTH_SCAN` / `BLUETOOTH_CONNECT` + `ACCESS_FINE_LOCATION` (API < 31).

**2. WiFi + HTTP REST server trên ESP32 (cùng LAN)**
ESP32 chạy `WebServer` / `ESPAsyncWebServer`, expose `GET /sensor` trả JSON. App gọi HTTP polling. Đơn giản nhất để code, nhiều phone xem cùng lúc, nhưng phải cùng mạng và tốn điện (WiFi always-on).

**3. WiFi + WebSocket**
Giống trên nhưng ESP32 push data real-time, app không cần polling. Hợp khi cần update liên tục (biểu đồ live).

**4. MQTT qua broker (HiveMQ Cloud, EMQX, hoặc Mosquitto self-host)**
ESP32 publish topic `home/livingroom/temp`, app subscribe. Đây là cách duy nhất xem được **từ xa qua internet**, và dễ mở rộng nhiều thiết bị. Cần broker + có internet.

## Gợi ý cho anh

Vì anh đã có sẵn ESP32-C6/S3 và làm Flutter, tôi nghĩ đi thẳng **MQTT** hoặc **BLE** tuỳ mục tiêu:

| Nhu cầu | Chọn |
|---|---|
| Chạy pin, xem tại chỗ, không cần server | BLE |
| Cắm điện, xem từ xa, nhiều node | MQTT |
| Prototype nhanh trong 1 buổi tối | HTTP REST |

**Cảm biến**: đừng dùng DHT11/DHT22 (chậm, sai số ±2°C, one-wire timing hay lỗi). Dùng **SHT31** hoặc **BME280** — I2C, ±0.3°C, BME280 có thêm áp suất. Cả hai đều 3.3V nên nối thẳng ESP32 không cần level shifter.

## Phía ESP32 (MQTT + BME280, rút gọn)

```cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;
WiFiClient net;
PubSubClient mqtt(net);

void loop() {
  if (!mqtt.connected()) reconnect();
  mqtt.loop();

  char payload[96];
  snprintf(payload, sizeof(payload),
    "{\"temp\":%.2f,\"hum\":%.2f,\"pres\":%.1f}",
    bme.readTemperature(), bme.readHumidity(),
    bme.readPressure() / 100.0F);

  mqtt.publish("home/sensor/bme280", payload, true); // retained
  delay(5000);
}
```

`retained: true` quan trọng — app vừa mở là nhận được giá trị cuối ngay, không phải chờ chu kỳ publish tiếp theo.

## Phía Flutter

- MQTT → package `mqtt_client`
- BLE → `flutter_blue_plus`
- HTTP → `dio` + `Timer.periodic`

Với MQTT, stream data map thẳng vào `StreamProvider` của Riverpod là ra được reactive UI luôn:

```dart
final sensorStreamProvider = StreamProvider<SensorData>((ref) {
  final client = ref.watch(mqttClientProvider);
  return client.updates!.map((events) {
    final msg = events.first.payload as MqttPublishMessage;
    final json = MqttPublishPayload.bytesToStringAsString(msg.payload.message);
    return SensorData.fromJson(jsonDecode(json));
  });
});
```

Một lưu ý về Android: nếu muốn app nhận data khi ở background, MQTT/WiFi sẽ bị Doze mode cắt kết nối. Lúc đó phải dùng `foreground service` hoặc chuyển sang push notification từ broker.

Anh định làm bản chạy pin hay cắm điện, và có cần xem từ xa ngoài mạng nhà không? Tôi có thể viết luôn code hoàn chỉnh hai đầu (ESP32 + Flutter) cho hướng anh chọn.

---

À, USB OTG — cũng làm được, và thực ra đơn giản hơn nhiều so với BLE/MQTT vì không cần pairing, không cần network. Nhưng có vài điều kiện cứng cần biết trước.

## Ràng buộc phần cứng

Phone phải hỗ trợ **USB OTG / USB Host mode** (hầu hết Android đời mới đều có, trừ vài máy giá rẻ bị cắt). Cáp thì tuỳ:

- Phone USB-C → ESP32 USB-C: cáp C-to-C thường ăn luôn
- Phone USB-C → ESP32 micro-USB: cần OTG adapter (C male → A female) rồi cắm cáp micro vào

**Phone sẽ cấp nguồn cho ESP32** qua VBUS. ESP32-S3 tắt WiFi ăn ~50–80mA, hoàn toàn ổn. Nhưng nếu bật WiFi (peak ~300–500mA) thì có máy sẽ ngắt OTG vì quá dòng. Anh không cần WiFi trong kịch bản này nên tắt hẳn đi, vừa đỡ nóng vừa đỡ hụt nguồn.

## Điểm mấu chốt: chip nào enumerate ra sao

Đây là chỗ hay nhầm. Board ESP32-S3 thường có **2 cổng USB**:

| Cổng | Đường đi | Android nhìn thấy | Baud rate |
|---|---|---|---|
| "UART" | CH340 / CP2102 → UART0 | Cần driver theo chip (VID/PID riêng) | Phải set đúng (115200) |
| "USB" | Native USB-Serial-JTAG của S3 | **CDC-ACM chuẩn** | Bị bỏ qua, set gì cũng chạy |

Anh có ESP32-S3 và C6 — cả hai đều có native USB, nên **dùng cổng "USB" (native)**. Nó enumerate thành CDC-ACM chuẩn, Android nhận không cần driver riêng, và throughput cao hơn nhiều (không bị nghẽn ở UART bridge).

## Phía ESP32

Trong Arduino IDE, chọn `USB CDC On Boot: Enabled`, `USB Mode: USB-OTG (TinyUSB)` hoặc `Hardware CDC and JTAG`. Rồi:

```cpp
#include <Wire.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);          // native USB: số này bị ignore
  Serial.setDebugOutput(false);  // tắt log hệ thống cho stream sạch
  WiFi.mode(WIFI_OFF);           // tiết kiệm dòng từ phone
  btStop();
  Wire.begin(8, 9);              // SDA, SCL — chỉnh theo board
  bme.begin(0x76);
}

void loop() {
  // NDJSON: mỗi dòng 1 object, kết thúc bằng '\n'
  Serial.printf("{\"t\":%.2f,\"h\":%.2f,\"p\":%.1f}\n",
                bme.readTemperature(),
                bme.readHumidity(),
                bme.readPressure() / 100.0F);
  delay(1000);
}
```

**Newline-delimited JSON** là framing đơn giản và đủ tốt. Serial là byte stream, không có message boundary — nếu không có delimiter thì app sẽ nhận được nửa packet. Đừng bỏ qua bước này.

## Phía Flutter

Package: `usb_serial` (wrap thư viện `usb-serial-for-android`, hỗ trợ CDC-ACM, CP210x, CH34x, FTDI, PL2303).

```dart
final devices = await UsbSerial.listDevices();
final port = await devices.first.create();
await port.open();

await port.setDTR(true);
await port.setRTS(true);
await port.setPortParameters(
  115200, UsbPort.DATABITS_8, UsbPort.STOPBITS_1, UsbPort.PARITY_NONE,
);

// Ghép byte chunk lại rồi tách theo '\n'
final stream = port.inputStream!
    .transform(utf8.decoder)
    .transform(const LineSplitter());

await for (final line in stream) {
  try {
    final data = jsonDecode(line);
    // -> đẩy vào StreamController / StreamProvider
  } catch (_) {
    // bỏ qua dòng rác (boot log, packet cụt lúc mới mở port)
  }
}
```

`AndroidManifest.xml`:

```xml
<uses-feature android:name="android.hardware.usb.host" />

<intent-filter>
  <action android:name="android.hardware.usb.action.USB_DEVICE_ATTACHED" />
</intent-filter>
<meta-data
  android:name="android.hardware.usb.action.USB_DEVICE_ATTACHED"
  android:resource="@xml/device_filter" />
```

Phần `device_filter.xml` cho phép app tự bật khi cắm cáp — không có nó thì user phải mở app thủ công rồi mới cấp quyền.

## Ba cái bẫy hay dính

**DTR/RTS làm reset board.** Trên board có mạch auto-reset (dùng DTR + RTS điều khiển EN/BOOT), lúc app mở port mà toggle DTR là ESP32 khởi động lại. Nếu gặp, thử `setDTR(false)` + `setRTS(false)`. Cổng native USB không dính lỗi này vì không qua mạch auto-reset.

**Boot log lẫn vào stream.** ESP32 in ra một đống `rst:0x1 (POWERON)...` ở lần boot. Cứ try/catch từng dòng như trên là xong.

**Rút cáp không báo trước.** Đăng ký `UsbSerial.usbEventStream` để bắt `USB_DEVICE_DETACHED`, không thì app treo ở `await for` mãi mãi.

Một điều nữa vì anh là iOS dev: **hướng này Android-only.** iOS không cho truy cập USB serial ngoài chương trình MFi, nên nếu sau này định port sang iPhone thì phải viết lại transport layer — lúc đó BLE là lựa chọn duy nhất chạy được cả hai nền. Nếu có ý định đó, tôi khuyên abstract cái transport ra interface ngay từ đầu (`SensorTransport` với impl `UsbTransport` / `BleTransport`), đổi sau sẽ nhàn.

Anh muốn tôi viết bản hoàn chỉnh không — ESP32 sketch + Flutter app có UI biểu đồ real-time, cấu trúc Clean Architecture với Riverpod như anh vẫn làm?
