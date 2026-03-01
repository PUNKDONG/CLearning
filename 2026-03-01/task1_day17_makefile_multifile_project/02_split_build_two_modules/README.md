# 练习2：多文件编译与链接

## 目标
理解“编译 .o”和“链接可执行文件”是两步。

## 你要完成
1. 拆分为至少 3 个文件：`main.c`、`math_utils.c`、`math_utils.h`。
2. 在 `main.c` 调用工具函数（如 add/max/abs_diff）。
3. Makefile 分两步：先 `.c -> .o`，再 `.o -> app`。
4. 写出头文件依赖关系。

## 建议文件结构（你自己创建）
- `main.c`
- `math_utils.c`
- `math_utils.h`
- `Makefile`

## 验收
1. `make` 得到可执行文件。
2. 删除可执行文件后，`make` 可重建。
3. 改动 `math_utils.h` 时，相关 `.o` 会重编译。
