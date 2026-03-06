#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/build/mini_grep"

if [[ ! -x "$BIN" ]]; then
  echo "[ERR] binary not found: $BIN"
  echo "Run: cmake -S . -B build && cmake --build build"
  exit 1
fi

echo "[1] normal match"
"$BIN" hello "$ROOT_DIR/tests/data/normal.txt"

echo "[2] ignore case"
"$BIN" -i hello "$ROOT_DIR/tests/data/normal.txt"

echo "[3] line number"
"$BIN" -n hello "$ROOT_DIR/tests/data/normal.txt"

echo "[4] file not found (expect non-zero)"
if "$BIN" hello "$ROOT_DIR/tests/data/not_exists.txt"; then
  echo "[ERR] should fail"
  exit 1
fi

echo "[5] missing args (expect non-zero)"
if "$BIN" hello; then
  echo "[ERR] should fail"
  exit 1
fi

echo "[6] invalid option (expect non-zero)"
if "$BIN" -x hello "$ROOT_DIR/tests/data/normal.txt"; then
  echo "[ERR] should fail"
  exit 1
fi

echo "[7] empty file"
"$BIN" hello "$ROOT_DIR/tests/data/empty.txt"

echo "[8] long line"
"$BIN" -i very_long "$ROOT_DIR/tests/data/long_line.txt" || true

echo "[9] repeated match"
"$BIN" -i grep "$ROOT_DIR/tests/data/normal.txt"

echo "[10] keyword not found"
"$BIN" xyz_not_found "$ROOT_DIR/tests/data/normal.txt"

echo "all tests executed"
