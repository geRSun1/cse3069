# Bài 2 — Đọc và kiểm tra cảm biến
Họ tên: Nguyễn Phúc Minh Đức · MSSV: 24110213 · Nhóm: 12

## File trong repo này
- sensor_delay.ino — bản dùng delay()
- sensor_millis.ino — bản dùng millis()
- data.csv — dữ liệu gốc (timestamp, raw, converted, unit, valid)
- chart.png — biểu đồ trước/sau lọc

## 4.1 Cấu hình (setup)
- Thông tin phần cứng : cảm biến DHT11
			: vi mạch esp32
- Thư viện sử dụng : DHT sensor library (Adafruit)
chân dht | chân esp32 |
vcc            3v3           
gnd            gnd   
data           gpio 14 

## 4.2 Cách báo lỗi
Khi k cắm chân Data hoặc k cắm chân GND cho mạch thì mạch chỉ hiện time không hiện các cột như temparature, humidity, valid. 
Còn đối với khi k cấp nguồn cho mạch, Watchdog Timer (WDT) của ESP32 phát hiện CPU1 bị treo quá lâu nên tự reset.

## 4.3 Bộ lọc đã chọn
Trong phần trước mình đã chọn bộ lọc Trung bình trượt (Moving Average - MA) với cửa sổ 5 mẫu (MA5)
Nhược điểm : độ trễ giả sử khi tăng đột ngột từ 29 lên 32 độ C thì MA5 sẽ tăng lên từ từ
Ưu điểm : dễ dàng tiếp cận, sơ đồ mượt

## 4.4 Kết quả thí nghiệm lập lịch
Trong 60 giây đo liên tục, nhiệt độ ghi nhận ổn định ở khoảng 29.5°C, độ ẩm dao động nhỏ từ 63% đến 64%. Sau khi áp dụng bộ lọc trung bình trượt MA5, tín hiệu gần như trùng với dữ liệu gốc do nhiễu rất thấp và môi trường đo ổn định. Độ trễ của bộ lọc không đáng kể trong thí nghiệm này.
## 4.5 Đóng góp cá nhân
Nhóm 1 người
