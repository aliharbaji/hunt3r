# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
LDFLAGS = -lcrypto

# Directories
SRC_DIR = .
HASH_DIR = hash
QUEUE_DIR = queue
READ_DIR = read
UTILITY_DIR = utility

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.c $(HASH_DIR)/*.c $(QUEUE_DIR)/*.c $(READ_DIR)/*.c $(UTILITY_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Output executable
TARGET = hibp

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
