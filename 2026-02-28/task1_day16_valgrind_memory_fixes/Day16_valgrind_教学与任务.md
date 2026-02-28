# Day16：`valgrind` 查泄漏与内存问题修复

## 1. 今天目标

今天只做一件事：用 `valgrind` 把“看不见的内存错误”抓出来并修掉。

你需要达成：

1. 会用 `valgrind` 跑程序并读懂核心输出。
2. 至少修复 2 个内存问题（建议：1 个泄漏 + 1 个非法访问）。
3. 修复后再次运行，确认 `ERROR SUMMARY: 0 errors`（或明显下降）。

---

## 2. 编译方式（必须）

```bash
gcc -g -O0 -Wall -Wextra -std=c11 xxx.c -o xxx
```

说明：

- `-g`：让 valgrind 报错能指到你的源码行。
- `-O0`：减少优化干扰，定位更稳定。

---

## 3. valgrind 常用命令

最常用：

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./app
```

参数作用：

- `--leak-check=full`：给出详细泄漏信息和调用栈。
- `--show-leak-kinds=all`：显示 definitely/indirectly/possibly lost 等类型。
- `--track-origins=yes`：未初始化值来源追踪（慢一点但很有用）。

把结果保存日志：

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
  --log-file=vg_01.log ./app
```

---

## 4. 你今天要重点看哪几行输出

1. `Invalid read of size ...` / `Invalid write of size ...`  
   说明有越界或悬空指针访问。

2. `Use of uninitialised value`  
   使用了未初始化内存。

3. `definitely lost: N bytes in M blocks`  
   明确泄漏，必须修。

4. `ERROR SUMMARY: X errors`  
   总错误数，修复前后对比的关键指标。

---

## 5. 修复优先级（按这个顺序）

1. 先修崩溃/非法访问（`Invalid read/write`）。  
2. 再修确定泄漏（`definitely lost`）。  
3. 最后看未初始化值来源（`track-origins`）。  

原因：访问越界会导致后续报错连锁，先修它能减少噪音。

---

## 6. 今日任务（3 题）

1. `01_memory_leak_fix.c`  
   目标：修复泄漏，让 `definitely lost` 变 0。

2. `02_invalid_access_fix.c`  
   目标：修复非法访问（越界/悬空），让 `Invalid read/write` 消失。

3. `03_two_bugs_in_one.c`  
   目标：同一程序里至少修 2 类问题（泄漏 + 非法访问）。

每题流程固定：

1. 编译（`-g -O0`）  
2. 跑 valgrind 保存日志  
3. 按日志定位代码行  
4. 修复  
5. 再跑 valgrind 对比前后

---

## 7. 验收标准

每题你都要能给出：

1. 修复前关键日志（哪类错误、哪一行）。  
2. 修复后关键日志（错误消失或显著减少）。  
3. 你改了什么、为什么这样改。  

今天最低通过线：

- 至少 2 个不同类型内存问题被修复。  

