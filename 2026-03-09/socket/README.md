# 2026-03-09 Socket 练习

目标：通过 2 个 C 语言编程题，掌握 `UDP` 编程和 `TCP` 套接字编程。

要求：
- 先自己实现，不直接看答案。
- 每题至少完成服务端和客户端两个程序。
- 编译时开启严格警告。

目录建议：
- `task1_udp_ping_time.md`：UDP 题目
- `task2_tcp_line_stats.md`：TCP 题目
- `udp_server.c` / `udp_client.c`
- `tcp_server.c` / `tcp_client.c`

推荐编译命令：
```bash
gcc -Wall -Wextra -Werror -o udp_server udp_server.c
gcc -Wall -Wextra -Werror -o udp_client udp_client.c
gcc -Wall -Wextra -Werror -o tcp_server tcp_server.c
gcc -Wall -Wextra -Werror -o tcp_client tcp_client.c
```

建议练习顺序：
1. 先做 UDP
2. 再做 TCP
3. 写完后再自己补 README，记录遇到的坑和测试结果
