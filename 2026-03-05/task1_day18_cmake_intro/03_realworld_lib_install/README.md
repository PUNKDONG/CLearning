# 03_realworld_lib_install

你接手了一个小型命令行工具项目。当前只有源码，构建系统缺失。

## 目标

让项目可以在 Linux 上完成以下流程：

1. 正常编译并运行可执行文件 `net_tool`
2. 产出一个可复用库 `netfmt`（静态库与动态库都需要）
3. 支持安装到自定义前缀目录
4. 安装后可被其他项目通过 `find_package` 使用

## 运行行为要求

程序运行后输出必须是：

```
https://api.example.com:8443/v1/ping
```

## 验收标准

1. Debug、Release 构建都通过
2. `build-debug/net_tool` 与 `build-release/net_tool` 均可运行并输出正确
3. `cmake --install` 后，目标前缀目录包含：
   - 可执行文件
   - 库文件（静态 + 动态）
   - 头文件
   - CMake package 配置文件
4. 使用一个独立的最小消费端工程调用 `find_package(netfmt CONFIG REQUIRED)` 可以成功链接并运行
