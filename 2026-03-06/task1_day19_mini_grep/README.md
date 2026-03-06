# mini_grep

A training-oriented grep-lite project for C language review.

## Build

```bash
cd /home/zy/CLearning/2026-03-06/task1_day19_mini_grep
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Run

```bash
./build/mini_grep hello tests/data/normal.txt
./build/mini_grep -i -n hello tests/data/normal.txt
```

## Test Scenarios

```bash
./tests/run_tests.sh
```

## Debug with gdb

```bash
gdb ./build/mini_grep
(gdb) break parse_args
(gdb) break run_grep
(gdb) run -i -n hello tests/data/normal.txt
```

## Notes

- Log file: `logs/mini_grep.log`
- Return code uses unified `ErrorCode` in `include/error.h`
