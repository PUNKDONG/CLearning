# 04_realworld_modules_ctest

你需要把一个多模块 C 项目整理成可维护的 CMake 工程。

## 目标

1. 生成两个库目标：
   - `mathx`
   - `textx`
2. 生成一个可执行目标：
   - `app`
3. 启用测试并纳入 CTest
4. 支持 Debug/Release 构建
5. 在开启严格警告时仍可通过编译

## 运行行为要求

`app` 输出必须是：

```
sum=42
upper=CMAKE
```

## 测试要求

执行测试命令后应全部通过（至少 2 个测试用例）。

## 约束

1. 不允许修改 `src/` 和 `tests/` 里的业务逻辑代码
2. 只通过 CMake 组织目标、依赖、include 路径和测试
3. 构建目录必须是 out-of-source
