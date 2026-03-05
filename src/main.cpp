#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

// Biến bàn phím chuẩn TinyUSB
USBHIDKeyboard Keyboard;

// Nút bấm trên S2 Mini là chân số 0
const int buttonPin = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  // Khởi động Bàn phím
  Keyboard.begin();
  
  // Khởi động USB
  USB.begin();
  
  // Cổng Serial ảo qua USB để bạn xem thông báo
  Serial.begin(115200);
}

void loop() {
  // Logic: Chờ nhấn nút số 0 thì gõ lệnh
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Phat hien nhan nut. Bat dau go...");
    
    // Đợi 1 chút để chống rung nút
    delay(200);

    // Bắt đầu kịch bản BadUSB
    Keyboard.press(KEY_LEFT_GUI); // Phím Windows
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();
    
    delay(500); // Đợi Run mở
    
    Keyboard.print("cmd");
    Keyboard.write(KEY_RETURN);
    
    delay(1000); // Đợi Notepad mở
    
    Keyboard.println("Chao ban, S2 Mini dang thuc thi lenh!");
    
    Serial.println("Da go xong.");

    // Chờ bạn thả nút ra để tránh việc nó gõ liên tục
    while(digitalRead(buttonPin) == LOW) {
      delay(10);
    }
  }
}