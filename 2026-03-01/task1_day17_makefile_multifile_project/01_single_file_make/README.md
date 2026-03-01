# 练习1：单文件 Makefile 最小闭环

## 目标
跑通最小 make 工作流。

## 你要完成
1. 写一个最简单的 C 程序（例如输出一行文本）。
2. 写 Makefile，至少包含：`all`、`run`、`clean`。
3. 编译选项包含：`-Wall -Wextra -std=c11`。

## 建议文件结构（你自己创建）
- `main.c`
- `Makefile`

## 验收
1. `make` 能生成程序。
2. `make run` 能运行。
3. `make clean` 后构建产物被清理。
