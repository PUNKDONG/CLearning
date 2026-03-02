# Day18 CMake 入门（练习题版）

日期：`2026-03-03`  
状态：只提供练习骨架，不提供完整答案。

## 练习目标

1. 会从 0 写最小 `CMakeLists.txt`
2. 会做 out-of-source build
3. 会区分 `Debug` 和 `Release`

## 练习 1：最小多文件工程

目录：`01_min_cmake_multifile/`

你需要完成：

1. 在 `CMakeLists.txt` 中补齐 `project`、`add_executable`、`target_include_directories`、`target_compile_options`
2. 在 `src/utils.c` 中实现函数
3. 在 `src/main.c` 中打印结果并自测

建议命令：

```bash
cmake -S . -B build
cmake --build build
./build/app
```

## 练习 2：Debug/Release + out-of-source

目录：`02_debug_release_out_of_source/`

你需要完成：

1. 补齐 `CMakeLists.txt`
2. 完成 `math_utils.c` 的两个函数
3. 分别跑 Debug 和 Release

建议命令：

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
./build-debug/app

cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
./build-release/app
```

## 验收标准

1. 两个练习都能独立构建和运行
2. 你能解释 `target_include_directories` 的作用
3. 你能解释 out-of-source build 的意义

