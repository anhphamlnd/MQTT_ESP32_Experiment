## Thí nghiệm kết nối Mosquitto với MQTTX
Mục đích :Dự án này là một thí nghiệm sử dụng Mosquitto làm MQTT broker và MQTTX làm MQTT client để kiểm tra các tính năng cơ bản của giao thức MQTT như kết nối, đăng ký, xuất bản và nhận thông báo. Thí nghiệm này giúp tìm hiểu cách thức hoạt động của MQTT và kiểm tra khả năng tương tác của các client với broker.
## Tiến hành
Cần lưu ý : Đảm bảo rằng firewall cho phép các kết nối qua các cổng đã định.

Thí nghiệm với cổng 1883
 - Sau khi đã cài đặt mosquitto và MQTTX theo hướng dẫn, trong file mosquitto.conf ta thêm
   
   listener 1883
   
   allow_anonymous true

   Tiếp theo cấu hình cho MQTTX
   ![hinh1](https://github.com/user-attachments/assets/fe5237fa-a82d-4af3-8706-60a07cabb89b) **hình 1**

   Cấu hình tên ,host,port như hình 1
   
   Tiếp theo khởi tạo kết nối
   
![mosquitto_sub_1883](https://github.com/user-attachments/assets/34b83fe3-67fc-49f6-8f4c-00df055870e5)

Ta làm thí nghiệm với mosquitto đóng vai trò subcribe với topic là test, ta thấy rằng khi ta publish thông tin từ MQTTX thì mosquitto nhận được tương ứng

![mosquitto_pub_1883](https://github.com/user-attachments/assets/75cce35a-f527-4449-90f0-49780358d5f3)

Ta làm thí nghiệm với mosquitto đóng vai trò publish với topic là test, ta thấy rằng khi ta publish thông tin từ mosquitto thì MQTTX nhận được thông tin tương ứng

## Đối với thí nghiệm với cổng 8883 TLS
1.Trước tiên ta cần tải OpenSSL từ trên máy để thiết lập chuẩn bảo mật 
Tiếp theo làm theo các lệnh dưới đây để tạo chứng chỉ 

openssl genrsa -out ca.key 2048

openssl req -x509 -new -nodes -key ca.key -sha256 -days 1024 -out ca.crt

openssl req -x509 -new -nodes -key ca.key -sha256 -days 1024 -out ca.crt

openssl genrsa -out server.key 2048

openssl req -new -key server.key -out server.csr

openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 500 -sha256

openssl genrsa -out client.key 2048

openssl req -new -key client.key -out client.csr

openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 500 -sha256

2.Cần cấu hình cho mosquitto.conf
listener 8883

cafile C:\cert\ca.crt

certfile C:\cert\server.crt

keyfile C:\cert\server.key

require_certificate true

3.Trong MTQQX cần thiết lập các thông số cần thiết và tải lên các chứng chỉ 

![mqttx](https://github.com/user-attachments/assets/a125983b-75d2-44b0-a13f-38ef9fa4e182)

Mặc dù em đã thực hiện theo các bước này,tuy nhân trong khi thiết mosquitto_sub và mosquitto_pub vẫn bị "protocol error" và chưa thể giải quyết được
