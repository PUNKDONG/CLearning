#!/usr/bin/env bash
set -euo pipefail

cd /home/zy/CLearning/study_checkin_app

if [ ! -d ".venv" ]; then
  python3 -m venv .venv
fi

source .venv/bin/activate

if ! python -c "import flask" >/dev/null 2>&1; then
  pip install -r requirements.txt >/tmp/study_checkin_pip.log 2>&1
fi

pids=$(lsof -t -iTCP:5000 -sTCP:LISTEN 2>/dev/null || true)
if [ -n "$pids" ]; then
  kill -9 $pids || true
fi

nohup env HOST=127.0.0.1 PORT=5000 DEBUG=0 python app.py >/tmp/study_checkin_server.log 2>&1 &
echo $! >/tmp/study_checkin_server.pid
