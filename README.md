# ANM-Esp32BadUSB

Dự án triển khai BadUSB sử dụng vi điều khiển ESP32-S2 Mini, cho phép thực hiện các cuộc tấn công tự động bằng cách giả lập bàn phím và chuột.

## Mục lục
- [Yêu cầu](#yêu-cầu)
- [Cài đặt](#cài-đặt)
  - [1. Cài đặt Visual Studio Code và PlatformIO](#1-cài-đặt-visual-studio-code-và-platformio)
  - [2. Mở dự án, Build và Upload](#2-mở-dự-án-build-và-upload)
- [Thông tin phần cứng](#thông-tin-phần-cứng)
- [Cấu hình dự án (platformio.ini)](#cấu-hình-dự-án-platformioini)

## Yêu cầu
Trước khi bắt đầu, bạn cần chuẩn bị:
1.  **Phần cứng**: Mạch WEMOS LOLIN S2 Mini.
2.  **Phần mềm**:
    *   [Visual Studio Code](https://code.visualstudio.com/): Một trình soạn thảo mã nguồn mạnh mẽ.
    *   [Git](https://git-scm.com/downloads): Hệ thống quản lý phiên bản phân tán.

## Cài đặt

### 1. Cài đặt Visual Studio Code và PlatformIO
1.  Tải và cài đặt [Visual Studio Code](https://code.visualstudio.com/).
2.  Mở VS Code, vào tab **Extensions** (Biểu tượng ô vuông bên trái hoặc `Ctrl+Shift+X`).
3.  Tìm kiếm `PlatformIO IDE` và nhấn **Install**.
    ![Install PlatformIO](https://platformio.org/assets/images/platformio-ide-vscode-installer.gif)
4.  Chờ quá trình cài đặt hoàn tất. Có thể bạn sẽ cần khởi động lại VS Code.

### 2. Mở dự án, Build và Upload
1. Mở thư mục dự án trong VS Code (`File > Open Folder...`).
2. Khi đã mở dự án trong VS Code với PlatformIO, bạn sẽ thấy các biểu tượng của PlatformIO ở thanh trạng thái phía dưới:

![PlatformIO Toolbar](https://platformio.org/assets/images/ide-vscode-toolbar.png)

-   **Build**: Nhấn vào biểu tượng dấu tick (✓) để biên dịch mã nguồn.
-   **Upload**: Kết nối mạch LOLIN S2 Mini với máy tính qua cổng USB. Sau đó, nhấn vào biểu tượng mũi tên (→) để nạp chương trình vào mạch.

**Lưu ý quan trọng**: Để có thể nạp code, bạn cần đưa mạch về chế độ **Bootloader**. Hãy làm theo các bước sau:
1.  Nhấn và giữ nút `0`.
2.  Nhấn rồi thả nút `RST`.
3.  Thả nút `0`.
4.  Sau đó tiến hành Upload từ PlatformIO.

## Thông tin phần cứng
Dự án này được phát triển và thử nghiệm trên mạch **WEMOS LOLIN S2 Mini**.

-   **Vi điều khiển**: ESP32-S2
-   **Flash**: 4MB
-   **PSRAM**: 2MB
-   **Kết nối**: USB-C
-   **Sơ đồ chân (Pinout)**:
    ![LOLIN S2 Mini Pinout](https://www.wemos.cc/en/latest/_static/s2_mini_v1.0.0_pinout.png)

Để sử dụng chức năng giả lập HID (Bàn phím, Chuột), chúng ta tận dụng thư viện `TinyUSB` được tích hợp sẵn cho chip ESP32-S2.

## Cấu hình dự án (platformio.ini)
File `platformio.ini` là file cấu hình chính cho PlatformIO, giúp định nghĩa môi trường build cho dự án.

```ini
[env:lolin_s2_mini]
platform = espressif32
board = lolin_s2_mini
framework = arduino

; Kích hoạt chuẩn TinyUSB cho S2 (rất quan trọng)
; Không dùng ARDUINO_USB_MODE=1 ở đây vì lolin_s2_mini mặc định là 0 (Tức là dùng TinyUSB)
build_flags = 
    -D USE_TINYUSB
```

-   `platform = espressif32`: Chỉ định rằng chúng ta đang sử dụng nền tảng ESP32 của Espressif.
-   `board = lolin_s2_mini`: Khai báo loại mạch cụ thể là LOLIN S2 Mini. PlatformIO sẽ tự động tải về các gói hỗ trợ cần thiết cho mạch này.
-   `framework = arduino`: Sử dụng Arduino framework để lập trình.
-   `build_flags = -D USE_TINYUSB`: Đây là một cờ biên dịch quan trọng. Nó định nghĩa macro `USE_TINYUSB` trong toàn bộ dự án, cho phép kích hoạt và sử dụng thư viện TinyUSB để giả lập các thiết bị USB như bàn phím.
