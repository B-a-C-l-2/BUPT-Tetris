# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -Wextra -g -Iinclude

# 目标可执行文件
TARGET = tetris

# 源文件目录和头文件目录
SRC_DIR = src
INC_DIR = include

# 所有源文件
SRCS = $(wildcard $(SRC_DIR)/*.c)

# 所有目标文件（将 .c 替换为 .o）
OBJS = $(SRCS:.c=.o)

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# 编译每个 .o 文件
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理编译结果
clean:
	rm -f $(OBJS)

# 清理所有（包括可执行文件和目标文件）
distclean: clean
	rm -f $(TARGET)

# 运行程序
run: $(TARGET)
	./$(TARGET)

# 检查项目状态
check:
	@echo "Source files: $(SRCS)"
	@echo "Object files: $(OBJS)"
	@echo "Target: $(TARGET)"
