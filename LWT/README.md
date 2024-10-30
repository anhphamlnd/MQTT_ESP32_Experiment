 # ESP32 MQTT Last Will and Testament (LWT) Demo
1. Giới thiệu
Dự án này thiết lập một kết nối MQTT an toàn từ ESP32 đến một broker MQTT. Chương trình sử dụng WiFiClientSecure để tạo kết nối TLS, bảo vệ giao tiếp MQTT của ESP32. Thông qua PubSubClient, ESP32 sẽ phát dữ liệu vào một topic và thiết lập Last Will and Testament (LWT) để thông báo cho broker khi ESP32 ngắt kết nối đột ngột.

 ## Các tính năng chính
+ Kết nối MQTT bảo mật qua TLS với WiFiClientSecure.
+ Thiết lập Last Will and Testament (LWT) để thông báo khi ESP32 ngắt kết nối bất thường.
+ Phát thông điệp định kỳ qua MQTT vào topic đã chọn.
+ Xử lý callback MQTT để nhận và in ra các thông điệp nhận được từ broker.
Yêu cầu
ESP32 Board đã cài đặt trong Arduino IDE.
Thư viện cần thiết:
WiFiClientSecure
PubSubClient
Ticker
HiveMQ Broker hỗ trợ kết nối an toàn qua TLS.
## Thí nghiệm
Trong bài tập này,em sử dụng chương trình mình đã demo trước đó là Wifi_Disconnect để thực hiện.Nội dung về cơ bản là giống chỉ khác nhau ở phần ở đây ta sẽ tiến hành thí 
nghiệm để kiểm tra LWT.

Đầu tiên em đăng kí 2 topic trên HiveMQ là esp32/lwt_test và esp32/echo_test 
![topic](https://github.com/user-attachments/assets/d3b69819-34e5-4648-a8a3-a256de1ad38d)


Đây là hình ảnh lúc hoạt động ban đầu 
![normal1](https://github.com/user-attachments/assets/07fefea3-754c-49c1-bfcc-31ceb46f8715)
![normal2](https://github.com/user-attachments/assets/7af424db-f467-465f-9a04-b2513849ee10)


Đây là hình ảnh sau khi rút kết nối ESP32 với máy tính

![ngatketnoi](https://github.com/user-attachments/assets/3d44e115-a56c-4624-9402-d0564daa985a)

Có thể thấy esp32 đã gửi 1 thông điện LWT tới broker thông qua topic esp/lwt_test được đăng kí và thông báo rằng không còn kết nối nữa.


