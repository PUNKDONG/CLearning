# 练习4：变量与模式规则

## 目标
把重复命令改成可维护模板。

## 你要完成
1. 在 Makefile 使用变量：`CC`、`CFLAGS`、`TARGET`、`SRCS`、`OBJS`。
2. 使用模式规则完成 `.c -> .o`。
3. 保留 `clean` 并声明 `.PHONY`。
4. 用 `make -n` 检查命令展开是否符合预期。

## 建议文件结构（你自己创建）
- `main.c`
- `math_utils.c`
- `math_utils.h`
- `Makefile`

## 验收
1. 不再重复粘贴相同编译命令。
2. 新增 `.c` 文件后，改变量即可参与构建。
3. `make clean` 可稳定清理。
