# Bài 2 — Đọc và kiểm tra cảm biến
Họ tên: Nguyễn Phúc Minh Đức · MSSV: 24110213 · Nhóm: 12

## File trong repo này
- `sensor_delay.ino` — Bản chương trình đọc cảm biến sử dụng hàm chặn `delay(2000)`
- `sensor_millis.ino` — Bản chương trình lập lịch không chặn (non-blocking) sử dụng `millis()`
- `data.csv` — Dữ liệu thu thập thực tế (timestamp, raw, converted, unit, valid, filtered)
- `chart.png` — Biểu đồ so sánh tín hiệu trước và sau khi lọc nhiễu

---

## 4.1 Cấu hình (setup)
- **Phần cứng sử dụng:** 
  - Vi điều khiển: ESP32 (NodeMCU-32S / ESP32 DevKit V1)
  - Cảm biến: Cảm biến nhiệt độ và độ ẩm số DHT11
- **Thư viện sử dụng:** `DHT sensor library` của Adafruit (phiên bản `1.4.6`) kết hợp `Adafruit Unified Sensor`.
- **Sơ đồ kết nối chân (Pinout):**

| Chân DHT11 | Chân ESP32 | Chức năng |
| :--- | :--- | :--- |
| **VCC** | `3V3` | Cấp nguồn hoạt động 3.3V |
| **GND** | `GND` | Nối đất chung (Mass) |
| **DATA** | `GPIO 14` | Truyền nhận dữ liệu số chuẩn 1-Wire |

- **Chu kỳ lấy mẫu và lý do lựa chọn:**
  - **Chu kỳ đã chọn:** `2000 ms` (1 mẫu / 2 giây), thu thập liên tục trong 60 giây (tổng cộng 30 mẫu ở điều kiện môi trường ổn định, không có tác động ngoại cảnh).
  - **Lý do (theo Datasheet và thực nghiệm):** Theo datasheet của DHT11, tốc độ lấy mẫu tối đa (sampling rate) là 1 Hz (tương đương tối thiểu 1 giây giữa hai lần đọc liên tiếp). Việc chọn chu kỳ **2 giây** giúp tạo khoảng đệm an toàn hơn mức tối thiểu của phần cứng, đảm bảo cảm biến kịp ổn định trạng thái điện trở/nhiệt bên trong, tránh hiện tượng đọc lỗi (timeout) và giảm hiệu ứng tự làm nóng (self-heating) trên linh kiện.

---

## 4.2 Cách báo lỗi
Khi thực hiện ngắt nguồn, rút dây tín hiệu `DATA` (GPIO 14) và cấp nguồn chạy lại chương trình, giao tiếp 1-Wire bị gián đoạn. Lúc này, hàm `dht.readTemperature()` và `dht.readHumidity()` của thư viện Adafruit không nhận được xung phản hồi từ cảm biến sau thời gian chờ (timeout) và trả về giá trị **`NaN` (Not a Number)**. 

Chương trình sử dụng hàm `isnan()` để kiểm tra: nếu phát hiện `NaN`, hệ thống sẽ giữ nguyên trạng thái lỗi (`NaN`), đánh dấu cờ `valid = 0` và **tuyệt đối không gán bằng `0`** để tránh nhầm lẫn với mức nhiệt độ $0^\circ\text{C}$ thực tế ngoài môi trường. Ngoài ra, nếu nối sai nguồn hoặc xử lý vòng lặp chờ bất hợp lệ quá lâu trên vi điều khiển, Watchdog Timer (WDT) của ESP32 sẽ phát hiện CPU bị treo và tự động khởi động lại (reset) mạch.

**Dòng log thực tế trích xuất từ Serial Monitor khi rút dây DATA:**
```text
timestamp,raw_temp,converted_temp,unit,valid
12004,NaN,NaN,C,0
---

## 4.3 Bộ lọc đã chọn
Trong phần trước mình đã chọn bộ lọc Trung bình trượt (Moving Average - MA) với cửa sổ 5 mẫu (MA5)
Nhược điểm : độ trễ giả sử khi tăng đột ngột từ 29 lên 32 độ C thì MA5 sẽ tăng lên từ từ
Ưu điểm : dễ dàng tiếp cận, sơ đồ mượt

---

## 4.4 Kết quả thí nghiệm lập lịch
Trong 60 giây đo liên tục, nhiệt độ ghi nhận ổn định ở khoảng 29.5°C, độ ẩm dao động nhỏ từ 63% đến 64%. Sau khi áp dụng bộ lọc trung bình trượt MA5, tín hiệu gần như trùng với dữ liệu gốc do nhiễu rất thấp và môi trường đo ổn định. Độ trễ của bộ lọc không đáng kể trong thí nghiệm này.
