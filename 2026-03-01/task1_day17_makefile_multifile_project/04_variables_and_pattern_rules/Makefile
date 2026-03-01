# 声明伪目标：clean 不是文件名，而是一个命令入口
.PHONY: clean

# C 编译器
CC = gcc
# 通用编译选项：警告 + C11 标准
CFLAGS = -Wall -Wextra -std=c11
# 自动生成头文件依赖（.d 文件）
DEPFLAGS = -MMD -MP
# 最终可执行文件名
TARGET = out
# 所有源文件列表（新增 .c 时改这里）
SRCS = main.c math_utils.c stats_utils.c
# 把 SRCS 中每个 .c 自动替换成 .o
OBJS = $(SRCS:.c=.o)
# 把每个 .o 对应成 .d（自动依赖文件）
DEPS = $(OBJS:.o=.d)

# 链接规则：目标 out 依赖全部目标文件
$(TARGET): $(OBJS)
	# 把所有 .o 链接成最终可执行文件 out
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# 模式规则：任意 xxx.o 由同名 xxx.c 生成（头文件依赖由 .d 自动维护）
# 自动变量说明：$< 是第一个依赖（这里是 .c），$@ 是当前目标（这里是 .o）
run: $(TARGET)
	./$(TARGET)
%.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# 读取自动生成的依赖；首次构建时 .d 不存在也不报错
-include $(DEPS)

# 清理构建产物
clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)
