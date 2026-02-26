# Day14 文件读写与错误处理教学

## 1. 今天目标

你今天要掌握 4 件事：

1. 会正确使用 `fopen` / `fprintf` / `fgets` / `fputs` / `fclose`。
2. 会用 `perror()` + `errno` 诊断文件 I/O 失败原因。
3. 能写出“判空 + 判返回值 + 统一关闭”的稳定代码。
4. 形成固定的文件 I/O 模板，减少低级错误。

---

## 2. `fopen` 模式详解（必须背）

函数原型：

```c
FILE *fopen(const char *path, const char *mode);
```

返回值：
- 成功：返回 `FILE *`
- 失败：返回 `NULL`（并设置 `errno`）

常用模式：

1. `"r"`
- 只读打开
- 文件必须存在
- 不会创建文件

2. `"w"`
- 只写打开
- 文件不存在则创建
- 文件存在会清空原内容

3. `"a"`
- 追加写
- 文件不存在则创建
- 每次写入都在末尾

4. `"r+"`
- 读写
- 文件必须存在
- 不清空文件

5. `"w+"`
- 读写
- 不存在则创建
- 存在则清空

6. `"a+"`
- 读写 + 追加
- 不存在则创建
- 写入位置总在末尾

工程建议：
- 读取配置：`r`
- 覆盖式导出：`w`
- 日志追加：`a`
- 读写且保留内容：`r+`

---

## 3. `fprintf` / `fgets` / `fclose`

## 3.1 `fprintf`：格式化写入

```c
int fprintf(FILE *stream, const char *format, ...);
```

返回值：
- 成功：返回写入字符数（不含 `\0`）
- 失败：返回负值

示例：

```c
if (fprintf(fp, "%d,%s,%.2f\n", id, name, score) < 0) {
    perror("fprintf failed");
}
```

---

## 3.2 `fgets`：安全按行读取

```c
char *fgets(char *s, int n, FILE *stream);
```

行为：
- 最多读 `n-1` 个字符，末尾自动补 `\0`
- 读到换行会保留 `\n`（如果读到了）
- 失败或 EOF 返回 `NULL`

为什么比 `gets` 安全：
- `gets` 不知道缓冲区大小，无法防止溢出。
- `fgets` 有长度上限，能控制边界。

典型读取循环：

```c
char buf[256];
while (fgets(buf, sizeof(buf), fp) != NULL) {
    fputs(buf, stdout);
}
if (ferror(fp)) {
    perror("fgets/fputs failed");
}
```

不要写：

```c
while (!feof(fp)) { ... } // 经典错误
```

---

## 3.3 `fputs`：写字符串到文件流

```c
int fputs(const char *s, FILE *stream);
```

行为：
- 把字符串 `s` 原样写入 `stream`
- 不会自动加 `\n`
- 成功返回非负值，失败返回 `EOF`

示例：

```c
if (fputs("hello\n", fp) == EOF) {
    perror("fputs failed");
}
```

和 `fprintf` 的区别：
- `fputs`：只写字符串，不做格式化
- `fprintf`：支持 `%d/%s/%.2f` 等格式化

---

## 3.4 `fgetc` / `fputc`：按字符读写（补充）

```c
int fgetc(FILE *stream);
int fputc(int c, FILE *stream);
```

用途：
- 逐字符处理（例如统计字符、复制二进制/文本的小实验）

返回值要点：
- `fgetc`：返回读到的字符（转为 `unsigned char` 再转 `int`），到 EOF/错误返回 `EOF`
- `fputc`：成功返回写入字符，失败返回 `EOF`

示例：

```c
int ch;
while ((ch = fgetc(in)) != EOF) {
    if (fputc(ch, out) == EOF) {
        perror("fputc failed");
        break;
    }
}
if (ferror(in)) {
    perror("fgetc failed");
}
```

---

## 3.5 `fclose`：关闭文件句柄

```c
int fclose(FILE *stream);
```

返回值：
- 成功：`0`
- 失败：`EOF`

注意：
- `fclose` 失败也要处理（例如磁盘写满导致缓冲区刷盘失败）。
- 关闭后文件指针不要再用。

示例：

```c
if (fclose(fp) == EOF) {
    perror("fclose failed");
}
fp = NULL;
```

---

## 4. 错误处理：`perror()` + `errno`

## 4.1 `errno` 是什么

- `errno` 是线程局部错误码变量。
- 标准库/系统调用出错时会设置它。
- 需要头文件：

```c
#include <errno.h>
```

## 4.2 `perror` 是什么

```c
void perror(const char *s);
```

输出格式类似：

```text
fopen input.txt failed: No such file or directory
```

它会把你传入的前缀 + 当前 `errno` 对应文本打印到 `stderr`。

## 4.3 标准错误处理模板

```c
#include <stdio.h>
#include <errno.h>

FILE *fp = fopen(path, "r");
if (fp == NULL) {
    perror("fopen failed");
    return 1;
}

// ... 读写 ...

if (fclose(fp) == EOF) {
    perror("fclose failed");
    return 1;
}
```

你也可以配合 `strerror(errno)` 自定义日志，但 `perror` 足够覆盖大多数场景。

---

## 5. 文件 I/O 最佳实践（你后面每天都按这个）

1. `fopen` 后第一时间判空。
2. 每次 I/O 调用都检查返回值：
- `fprintf < 0` 失败
- `fgets == NULL` 需要区分 EOF 还是错误（看 `ferror`）
3. 写操作完成后，必须检查 `fclose` 返回值。
4. 出错路径也要关闭已打开文件（避免句柄泄漏）。
5. 不要使用 `gets`。
6. 不要用 `while(!feof(fp))` 作为读循环条件。
7. 固定使用“单一退出 + 清理区”风格，减少漏关文件。

---

## 6. 一个稳定模板（可直接套）

```c
#include <stdio.h>
#include <errno.h>

int process_file(const char *in_path, const char *out_path) {
    FILE *in = NULL;
    FILE *out = NULL;
    char line[256];
    int rc = -1;

    in = fopen(in_path, "r");
    if (in == NULL) {
        perror("open input failed");
        goto cleanup;
    }

    out = fopen(out_path, "w");
    if (out == NULL) {
        perror("open output failed");
        goto cleanup;
    }

    while (fgets(line, sizeof(line), in) != NULL) {
        if (fprintf(out, "%s", line) < 0) {
            perror("write failed");
            goto cleanup;
        }
    }

    if (ferror(in)) {
        perror("read failed");
        goto cleanup;
    }

    rc = 0;

cleanup:
    if (in != NULL && fclose(in) == EOF) {
        perror("close input failed");
        rc = -1;
    }
    if (out != NULL && fclose(out) == EOF) {
        perror("close output failed");
        rc = -1;
    }
    return rc;
}
```

---

## 7. 今天的最小验收

你写完练习后至少满足：

1. 不存在未关闭文件句柄。
2. 文件不存在时有明确错误输出（`perror`）。
3. 写入失败时能报错而不是静默成功。
4. 读循环结束后会检查 `ferror`。

做到这 4 条，你今天就达标。
