# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -Wextra -g -I.

# 目标可执行文件
TARGET = tetris

# 所有源文件
SRCS = $(wildcard *.c)

# 所有目标文件（.o）
OBJS = $(SRCS:.c=.o)

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# 编译每个 .o 文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理编译结果
clean:
	rm -f $(OBJS) $(TARGET)

# 清理所有（包括可执行文件和目标文件）
distclean: clean
	rm -f $(TARGET)

# 运行程序
run: $(TARGET)
	./$(TARGET)

# 检查所有源文件
check:
	@echo "Source files: $(SRCS)"
	@echo "Object files: $(OBJS)"
	@echo "Target: $(TARGET)"
