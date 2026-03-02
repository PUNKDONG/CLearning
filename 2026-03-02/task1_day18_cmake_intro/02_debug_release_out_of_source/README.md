# 02_debug_release_out_of_source

## 你需要完成

1. 补齐 `CMakeLists.txt` 的 TODO
2. 实现 `src/math_utils.c`
3. 修改 `src/main.c` 完成调用

## Debug 构建

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
./build-debug/app
```

## Release 构建

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
./build-release/app
```

## 自检

1. 两种构建都能通过
2. 输出结果正确
3. 能说清 out-of-source build 的好处

