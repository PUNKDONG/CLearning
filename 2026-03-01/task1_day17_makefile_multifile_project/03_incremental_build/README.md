# 练习3：增量构建

## 目标
观察 make 只重编译变更相关文件。

## 你要完成
1. 基于练习2继续。
2. 在 Makefile 里明确写依赖关系。
3. 记录三种场景的重编译行为：
   - 只改 `main.c`
   - 只改 `math_utils.c`
   - 只改 `math_utils.h`

## 建议文件结构（你自己创建）
- `main.c`
- `math_utils.c`
- `math_utils.h`
- `Makefile`
- `notes.md`（记录观察结果）

## 验收
1. 只改 `main.c` 时，不重编译 `math_utils.o`。
2. 只改 `math_utils.c` 时，不重编译 `main.o`。
3. 改头文件时，依赖它的 `.o` 都重编译。
