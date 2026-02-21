# --- Configuration ---
CC      := gcc
TARGET  := arena_benchmark
SRC_DIR := source debug
INC_DIR := include
OBJ_DIR := build

# Find all .c files in the source directories
SRCS    := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
# Map .c files to .o files in the build directory
OBJS    := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))
# Map .o files to .d (dependency) files
DEPS    := $(OBJS:.o=.d)

# --- Build Modes ---
# Default to release mode if not specified (e.g., run `make` or `make MODE=debug`)
MODE ?= release

# Base flags
CFLAGS := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c11 -MMD -MP

ifeq ($(MODE), debug)
    CFLAGS += -g -O0 -DDEBUG
    $(info Build Mode: DEBUG)
else
    # Release mode: Maximize speed, enable link-time optimization
    CFLAGS += -O2 -march=native -flto -DNDEBUG
    $(info Build Mode: RELEASE)
endif

# --- Targets ---
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

# Include generated dependencies
-include $(DEPS)
