#include <Arduino.h>
#include <payloads.h>
#include "USBHIDKeyboard.h"

extern USBHIDKeyboard Keyboard;

void runSystemAudit()
{
    Serial.println("Dang chay System Fingerprinting...");

    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();
    delay(500);

    // chay powershell voi lenh thu thap thong tin
    String cmd = "powershell -WindowStyle Normal \"$d = [Environment]::GetFolderPath('Desktop'); systeminfo > \\\"$d\\SystemFingerprint.txt\\\"; whoami >> \\\"$d\\SystemFingerprint.txt\\\"; notepad \\\"$d\\SystemFingerprint.txt\\\"; Read-Host 'Da xuat file. Nhan Enter de ket thuc'\"";

    Keyboard.print(cmd);
    Keyboard.write(KEY_RETURN);

    Serial.println("Payload da gui. Dang cho phan hoi tu he thong...");
}