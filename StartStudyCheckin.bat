@echo off
chcp 65001 >nul
setlocal

REM 1) 先在 WSL 内清理 5000 端口监听进程，防止端口占用
wsl.exe -d Ubuntu bash -lc "lsof -t -iTCP:5000 -sTCP:LISTEN 2>/dev/null | xargs -r kill -9"

REM 2) 启动打卡服务（新窗口）
start "StudyCheckin-WSL" wsl.exe -d Ubuntu bash -lc "cd /home/zy/CLearning/study_checkin_app && ./start.sh"

REM 3) 稍等后打开页面
timeout /t 2 /nobreak >nul
start "" http://127.0.0.1:5000/

endlocal
