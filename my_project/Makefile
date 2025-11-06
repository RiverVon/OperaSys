# 编译器设置
CC = gcc
CFLAGS = -Wall -g

# 项目目录结构
SRC_DIR = src
BUILD_DIR = build

# 源文件和目标文件
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# 可执行文件名
TARGET = my_program

# 默认目标
all: $(BUILD_DIR) $(TARGET)

# 创建构建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 链接目标
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 编译规则
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
# 清理
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean

