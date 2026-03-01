# 练习5：Debug / Release 双构建

## 目标
学会按场景切换编译参数。

## 你要完成
1. 增加 `debug` 与 `release` 目标。
2. 两种构建使用不同产物名（互不覆盖）。
3. 增加 `print` 目标，输出关键变量（`CC/CFLAGS/TARGET`）。

## 建议文件结构（你自己创建）
- `main.c`
- `math_utils.c`
- `math_utils.h`
- `Makefile`

## 验收
1. `make debug` 和 `make release` 都成功。
2. debug 产物可用于调试（符号信息完整）。
3. debug/release 产物可同时存在。
