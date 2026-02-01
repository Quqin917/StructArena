CC      := gcc
BLD_DIR := ./build

INC_DIRS   := include
INC_FLAGS  := $(addprefix -I, $(INC_DIRS))

FLAGS   := $(INC_FLAGS) -MMD -MP
CFLAGS  := -Wall -pedantic
LDFLAGS :=

SRC_LIB := $(shell find source -type f -name '*.c')

SRC_DEMO  := apps/main.c
SRC_DEBUG := debug/debug.c

OBJ_LIB = $(addprefix $(BLD_DIR)/, $(SRC_LIB:.c=.o))
OBJ_DEMO_MAIN  = $(addprefix $(BLD_DIR)/, $(SRC_DEMO:.c=.o))
OBJ_DEBUG_MAIN = $(addprefix $(BLD_DIR)/, $(SRC_DEBUG:.c=.o))

OBJ_DEMO  = $(OBJ_LIB) $(OBJ_DEMO_MAIN)
OBJ_DEBUG = $(OBJ_LIB) $(OBJ_DEBUG_MAIN)

DEPS = $(OBJ_LIB:.o=.d) $(OBJ_DEMO_MAIN:.o=.d) $(OBJ_DEBUG_MAIN:.o=.d)

.PHONY: all demo debug clean
all: demo

demo:
	@$(MAKE) BLD_DIR=./build/demo CFLAGS="$(CFLAGS) -O2" linked_demo

debug:
	@$(MAKE) BLD_DIR=./build/debug CFLAGS="$(CFLAGS) -g -O0 -DDEBUG" linked_debug

linked_demo: $(OBJ_DEMO)
	$(CC) $(OBJ_DEMO) -o $@ $(LDFLAGS)

linked_debug: $(OBJ_DEBUG)
	$(CC) $(OBJ_DEBUG) -o $@ $(LDFLAGS)

$(BLD_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf ./build linked_demo linked_debug vgcore.*

-include $(DEPS)
