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
