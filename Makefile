ifeq ($(OS), Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

ALLOCATOR ?= malloc

ifeq ($(DETECTED_OS), Linux)
    ALLOCATOR := mmap
    $(info System detected: Linux. Auto-selecting MMAP allocator.)
else ifeq ($(DETECTED_OS), Darwin)
    ALLOCATOR := mmap
    $(info System detected: macOS. Auto-selecting MMAP allocator.)
else
    ALLOCATOR := malloc
    $(info System detected: $(DETECTED_OS). Auto-selecting malloc allocator.)
endif

CC      := gcc
TARGET  := arena_benchmark
INC_DIR := include
OBJ_DIR := build
SRC_DIR := source

# Default to release mode if not specified (e.g., run `make` or `make MODE=debug`)
MODE ?= release
CFLAGS := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c11 -MMD -MP

ifeq ($(MODE), debug)
    CFLAGS += -g -O0 -DDEBUG
		SRC_DIR += debug
    $(info Build Mode: DEBUG)
else
    CFLAGS += -O2 -march=native -flto -DNDEBUG -pg
		SRC_DIR += apps
    $(info Build Mode: RELEASE)
endif

SRCS    := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))

ifeq ($(ALLOCATOR), mmap)
    SRCS := $(filter-out source/arena.c, $(SRCS))
    $(info Implementation: source/arena_Linux.c)
else
    SRCS := $(filter-out source/arena_Linux.c, $(SRCS))
    $(info Implementation: source/arena.c)
endif

OBJS    := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS    := $(OBJS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $@"
	@$(CC) $(CFLAGS) $^ -o $@

# Compile rule for any .c file
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	@echo "Cleaning build files..."
	@rm -rf $(OBJ_DIR) $(TARGET) vgcore.*

-include $(DEPS)
