# 2026-03-12 进程专题计划

目标：掌握视频 34-38 涉及的 `system`、`fork`、`execve`、`wait`、`waitpid`，并能独立写出最小可运行案例。

## 今日计划

### 1. 先学什么

建议顺序：
1. `system`：先理解“父进程直接调用 shell 帮你执行命令”
2. `fork`：理解“复制出一个子进程”
3. `fork` 复制文件描述符：理解“父子进程会共享同一个打开文件表项”
4. `execve`：理解“当前进程代码被新程序替换”
5. `wait` / `waitpid`：理解“父进程如何回收子进程，避免僵尸进程”

### 2. 今日时间安排

- `09:00-09:40` 看视频 34-35，整理 `system` / `fork` 笔记
- `09:40-10:30` 做 `01_system_basic.c`、`02_fork_basic.c`
- `10:30-11:20` 做 `03_fork_fd_share.c`
- `14:00-15:00` 看视频 36-37，做 `04_execve_basic.c`
- `15:00-16:00` 看视频 38，做 `05_waitpid_basic.c`
- `16:00-16:30` 回顾 5 个 API 的函数原型、返回值、典型错误

### 3. 今日必须掌握

- `system` 会启动 shell，适合快速执行命令，但不适合精细控制
- `fork` 成功后会返回两次：
  - 父进程中返回子进程 PID
  - 子进程中返回 `0`
- `fork` 后父子进程代码从同一位置继续往下执行
- `fork` 后会复制文件描述符，父子进程都能访问同一个文件
- `execve` 成功后不会返回；只有失败才返回 `-1`
- `wait` / `waitpid` 用来回收子进程，避免僵尸进程

### 4. 今日高频坑点

- `fork` 后忘记区分父子分支
- 在子进程里 `execve` 失败后没有 `perror`
- 误以为 `execve` 成功后后面的代码还会执行
- 父进程不 `wait` / `waitpid`
- `waitpid` 的第一个参数写错，不理解 `-1` / 指定 PID / `WNOHANG`
- `fork` 后对同一个文件描述符的读写顺序判断错误

### 5. 今日验收标准

- 你能口述这 5 个函数的作用、返回值和典型使用场景
- 你能写出一个最小 `fork + waitpid` 案例
- 你能解释“为什么 `execve` 成功后不返回”
- 你能解释“为什么 `fork` 后父子进程都能操作同一个文件描述符”

## 练习文件

- `01_system_basic.c`
- `02_fork_basic.c`
- `03_fork_fd_share.c`
- `04_execve_basic.c`
- `05_waitpid_basic.c`

## 推荐编译命令

```bash
gcc -Wall -Wextra -Werror -std=c11 -o 01_system_basic 01_system_basic.c
gcc -Wall -Wextra -Werror -std=c11 -o 02_fork_basic 02_fork_basic.c
gcc -Wall -Wextra -Werror -std=c11 -o 03_fork_fd_share 03_fork_fd_share.c
gcc -Wall -Wextra -Werror -std=c11 -o 04_execve_basic 04_execve_basic.c
gcc -Wall -Wextra -Werror -std=c11 -o 05_waitpid_basic 05_waitpid_basic.c
```

## 建议

- 今天不要追求写很多题，重点是把这 5 个 API 的行为真正看懂
- 每题做完后，自己在注释里写一句“这个函数到底是干嘛的”
