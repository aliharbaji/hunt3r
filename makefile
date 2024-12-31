# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
LDFLAGS = -lcrypto

# Directories
SRC_DIR = src
BUILD_DIR = build
HASH_DIR = $(SRC_DIR)/hash
QUEUE_DIR = $(SRC_DIR)/queue
READ_DIR = $(SRC_DIR)/read
UTILITY_DIR = $(SRC_DIR)/utility
ACCOUNT_DIR = $(SRC_DIR)/account

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.c $(HASH_DIR)/*.c $(ACCOUNT_DIR)/*.c $(QUEUE_DIR)/*.c $(READ_DIR)/*.c $(UTILITY_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = hibp

# Create necessary directories
$(shell mkdir -p $(BUILD_DIR))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --verbose ./$(TARGET)

.PHONY: all clean valgrind
