# mini_grep 综合训练（按训练顺序）

## 第一部分：训练目标与覆盖知识点

这个项目用于系统复习 C 语言与工程化能力，覆盖：

1. C 语法基础：流程控制、函数、数组、`const`。
2. 指针：`char *`、`const char *`、函数参数传递。
3. 字符数组与字符串：`fgets`、`strstr`、大小写处理。
4. 文件操作：`fopen/fgets/ferror/fclose`。
5. 命令行参数：`argc/argv` 与选项解析。
6. 结构体：`CliOptions`、`GrepStats`。
7. 枚举：统一错误码 `ErrorCode`。
8. 宏：日志宏、常量宏、辅助宏。
9. 函数拆分：解析、匹配、检索、日志职责分离。
10. 多文件工程：`src + include` 模块组织。
11. 错误码设计：参数错误、文件打开失败、IO 错误分层。
12. 日志模块：`info/error` 两级输出。
13. Make 与 CMake：手写命令式 vs 目标声明式。
14. gdb 调试：断点、单步、变量观察、栈追踪。
15. 边界处理：空文件、超长行、关键字不存在。
16. Linux CLI 习惯：返回码、stderr、可复现实验命令。

## 第二部分：正式需求说明

1. 项目背景：实现 grep 简化版，用于文本关键字检索。
2. 功能要求：
   - 关键字检索。
   - `-i` 忽略大小写。
   - `-n` 输出行号。
3. 输入输出：
   - 输入：`mini_grep [-i] [-n] <keyword> <file>`。
   - 输出：匹配到的行（可带行号）。
4. 参数规则：
   - 选项可组合。
   - 必须提供 `<keyword>` 和 `<file>`。
5. 非法输入行为：
   - 输出错误信息 + usage。
   - 返回统一错误码（非 0）。
6. 日志要求：
   - 至少 `info/error` 两级。
   - 输出到 stderr，尽量落盘到 `logs/mini_grep.log`。
7. 错误处理要求：
   - 参数、文件打开、IO 各自有错误码。
8. 建议目录结构：
   - `src/ include/ tests/ logs/ CMakeLists.txt README.md`。
9. 评分标准（100）：
   - 功能正确 35
   - 参数与错误处理 20
   - 模块化与可读性 20
   - 构建与调试友好性 15
   - 测试覆盖 10
10. 可扩展方向：多文件检索、递归目录、正则、高亮、单元测试。

## 第三部分：四层训练模式

### 第1层：只给题目，不给答案

完成最小功能：
1. 实现 `mini_grep [-i] [-n] <keyword> <file>`。
2. 正确读取文件并输出匹配行。
3. 对错误输入给出非 0 返回码。

### 第2层：模块划分提示

建议模块：
1. `args.*`：参数解析。
2. `matcher.*`：匹配策略（区分大小写/忽略大小写）。
3. `grep.*`：文件读取与行输出。
4. `logger.*`：统一日志。
5. `error.*`：错误码文本映射。
6. `main.c`：流程编排，不堆业务细节。

### 第3层：关键函数设计建议

建议原型：

```c
int parse_args(int argc, char **argv, CliOptions *opts, ErrorCode *err, const char **err_msg);
int line_matches(const char *line, const char *keyword, int ignore_case);
int run_grep(const CliOptions *opts, GrepStats *stats, ErrorCode *err);
int logger_init(const char *log_path);
void logger_log(LogLevel level, const char *fmt, ...);
```

职责建议：
1. `parse_args` 只处理语法，不做文件 IO。
2. `run_grep` 只做检索，不做参数解释。
3. `line_matches` 可单独测试。

### 第4层：完整参考实现

完整参考实现已经放在本项目目录：
1. `src/main.c`
2. `src/args.c`
3. `src/matcher.c`
4. `src/grep.c`
5. `src/logger.c`
6. `src/error.c`

## 第四部分：目录结构与职责

1. `src/`：业务实现代码。
2. `include/`：头文件与对外接口。
3. `tests/`：测试数据与测试脚本。
4. `logs/`：运行日志输出目录。
5. `CMakeLists.txt`：构建规则。
6. `README.md`：构建、运行、调试说明。

## 第五部分：CMake 方案

要求已落实：
1. C11 标准。
2. 开启常见警告。
3. Debug/Release 区分。
4. Debug 默认 `-O0 -g3`，便于 gdb。
5. 命令示例：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/mini_grep -i -n hello tests/data/normal.txt
gdb ./build/mini_grep
```

## 第六部分：测试数据与场景

覆盖场景：
1. 正常匹配。
2. 忽略大小写。
3. 行号输出。
4. 文件不存在。
5. 参数缺失。
6. 参数非法。
7. 空文件。
8. 超长行。
9. 多次重复匹配。
10. 关键字不存在。

对应数据与脚本：
1. `tests/data/normal.txt`
2. `tests/data/empty.txt`
3. `tests/data/long_line.txt`
4. `tests/run_tests.sh`

## 第七部分：gdb 练习任务

建议任务：
1. 在 `parse_args` 下断点，检查 `argv` 解析路径。
2. 在 `run_grep` 下断点，观察 `line_no`、`matched_lines`。
3. `step` 跟踪 `line_matches` 的大小写分支。
4. 观察 `ferror(fp)` 触发路径。
5. 检查 `-n` 输出是否与 `line_no` 一致。
6. 刻意输入错误参数，验证错误码。
7. 在超长行场景观察 `truncated_lines` 变化。

## 第八部分：工程要求落实清单

1. 代码风格清晰。
2. 注释只在关键逻辑位置。
3. 参数合法性检查已做。
4. 错误码统一。
5. `info/error` 日志模块已做。
6. main 未堆积全部逻辑。
7. 模块职责单一。
8. 适合作为复习工程。
9. 覆盖常见基础能力点。
10. 只用标准库，无不必要复杂依赖。

## 第九部分：能力总结与升级方向

本项目锻炼能力：
1. C 语言能力：字符串、文件 IO、参数解析、结构体、枚举。
2. 工程能力：模块化、错误码、日志、CMake 构建。
3. 调试能力：gdb 定位参数/匹配/IO 问题。

可升级方向：
1. 支持多个文件。
2. 支持正则（POSIX regex）。
3. 支持递归目录检索。
4. 支持终端高亮。
5. 日志改成可配置级别与滚动。
6. 引入单元测试框架（如 CTest + 自定义断言）。
