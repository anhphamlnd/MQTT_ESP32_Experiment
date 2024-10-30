## Bố trí thí nghiệm
Dùng thư viện PubSubClient trên ESP32 kết nối với HiveMQ
Sử dụng thư viện Ticker, một thư viện chuẩn trong Arduino để gọi hàm publish một cách đều đặn và bất đồng bộ, mỗi giây (1s) một lần:
Mã: mqttPulishTicker.attach(1, mqttPublish)
Subscribe tới topic esp32/echo_test ngay sau khi MQTT connect thành công
Gọi hàm mqttClient.loop() trong main loop để handle các thông điệp nhận được từ broker (bất đồng bộ, event driven) bất kỳ lúc nào.
Phát hiện mất kết nối MQTT if (!mqttClient.connected()) trong main loop để kết nối lại mqttReconnect() ngay khi phát hiện mất kết nối.
## Kịch bản thí nghiệm
- Sau khi ESP32 khởi động, sẽ kết nối WiFi vào một điểm phát AP đã định (ssid, và pass trong secrets/wifi.h) --> thành công
- Sẽ thấy MQTT Client kết nối đến broker thành công và bắt đầu gửi (publish) và nhận (subscribe) số đếm tăng dần trong echo_topic đều đặn
- Khi đó sẽ tiến hành ngắt điểm phát WiFi, tiện nhất là phát wifi từ điện thoại để bật ngắt nó nhanh chóng trong tầm tay
- Quan sát phản ứng của MQTT Client trong mã khi mất kết nối WiFi giữa chừng,
sau đó bật lại điểm phát WiFi và quan sát khả năng khôi phục kết nối, và quan sát việc mất gói tin trong quá trình kết nối.
## Mục đích 
- Xem việc ngắt kết nối từ bên dưới chồng Internet Protocol có ảnh hưởng tới lớp trên không. Ở đây là lớp WiFi (link layer) bị ngắt --> lớp TCP/IP bị ngắt --> có ảnh hưởng tới lớp ứng dụng MQTT trên cùng hay không? ESP core lib sẽ in ra thông điệp lỗi như nào (có báo lỗi từ lớp dưới lên lớp bên trên hay không?)
- Quan sát sự bỏ mặc việc mất thông điệp trong QoS = 0. 
- Hiểu rõ hơn về cơ chế hoạt động của MQTT client bên trên tầng TCP/IP, nhất là cơ chế phát hiện mất kết nối và khôi phục kết nối ở lớp vật lý, rất hay xảy ra trong thực tế.
## Kết quả thu trên serial monitor
Quan sát thông báo được đưa ra ta có một số nhận xét như sau:

1.Quan sát ban đầu

![hinh1](https://github.com/user-attachments/assets/8dcecdc0-bd58-46b1-a17f-ea2c9b8d14c0) **hình 1**
- Hình 1 cho ta thấy thiết bị ESP32 đã kết nối thành công với mạng Wi-Fi và giao tiếp với MQTT broker. Thiết bị kết nối với SSID có tên là 'aaa' và nhận địa chỉ IP 192.168.220.113. Sau đó, ESP32 cố gắng kết nối với MQTT broker và đã thành công, được xác nhận qua thông báo "esp32-client-10:06:1C:86:15:A4 connected", cho thấy thiết bị (với địa chỉ MAC 10:06:1C:86:15:A4) đã kết nối tới broker
- Đồng thời 1 điều thú vị là có 2 thông điệp được publish là 0 và 1 trước cả khi MQTT kết nối thành công
- Ta cũng thấy được cơ chế Echo hoạt động bình thường ngay cả khi QoS = 0

2.Khi ngắt kết nối Wifi
![hinh2](https://github.com/user-attachments/assets/895db454-07cd-4582-99f9-e6f75756f85c) **hình 2**

Hình 2 minh họa trường hợp ngắt Wifi kết nối với ESP32 giữa chừng
Ngay lập tức ssl_client ở lớp dưới trên con ESP32 báo lỗi (ssl_client.cpp:37 ...)
Sau đó thì thư viện PubSubClient vẫn tiếp tục publishing thông điệp 1s mỗi lần.
Việc kết nối lại không thành công cho đến khi ta bật điểm phát WiFi lại.

3.Khi Wifi được kết nối lại
![hinh3](https://github.com/user-attachments/assets/49473649-ce44-452c-846a-7f2265de69c4) **hình 3**

Hình 3 minh họa việc esp32 được kết nối lại với Wifi,việc kết nối với MQTT cũng mất khoảng 3 giây để có thể kết nối lại
## Kết quả thu trên HiveMQ
![hinh4](https://github.com/user-attachments/assets/a482a1d9-5d91-477f-b539-3e381218bc8f) **hình 4**

Hình 4 minh họa việc HiveMQ nhận thông tin từ esp32 gửi lên

![hinh5](https://github.com/user-attachments/assets/3b15a4d2-a1e9-47b4-afbe-f0f62e689b03)**hình 5**

![hinh6](https://github.com/user-attachments/assets/29b9eb24-8cf6-4080-a4af-fd7941b08ba4)**hình 6**

Hình 5 và hình 6 minh họa việc publish thông tin từ HiveMQ khi ta đăng kí 1 topic với chủ đề là esp32/echo_test và thông tin nhận được là "My name is Anh" được hiển thị trên serial monitor có thể thấy được trong hình 6.

## Các thay đổi
- So với file đã được hướng dẫn sử dụng MQTTX thì với bài tập sử dụng HiveMQ này, em đã có sự thay đổi trong việc sử dụng ca.cert vì nội dung cert của 2 broker này là khác nhau.
- Tạo lập thư mục secrets trong include bao gồm 2 file là mqtt.h và wifi.h để thiết lập thông tin bảo mật cho chương trình
  + Trong file mqtt.h bao gồm thông tin về broker,port,username và password sử dụng cho việc kết nối với HiveMQ
  + Trong file wifi.h bao gồm thông tin về ssid và password sử dụng cho việc kết nối với wifi  
