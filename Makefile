# Project
COMPILE_COMMANDS := build/compile_commands.json
PROJECT := dwarf
CC ?= bear --output $(COMPILE_COMMANDS) -- clang
CFLAGS ?= -Wall -Wextra -Wunused-result -Wconversion
CPPFALGS ?= -Isrc
LDFLAGS ?= -lSDL2 -lvec 

# Dirs
BUILD_DIR ?= build/linux/debug
OBJ_DIR := $(BUILD_DIR)/obj
SRC_DIR := src
BIN_DIR := $(BUILD_DIR)/bin
INSTALL_DIR := /usr/local/bin
INC_DIR := $(SRC_DIR)

# Files
MAIN := $(SRC_DIR)/main.c
SRC := $(filter-out $(MAIN), $(wildcard $(SRC_DIR)/*.c))
INC := $(wildcard $(INC_DIR)/*.h)

# Targets
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BIN ?= $(BIN_DIR)/$(PROJECT)

# Rules
.PHONY: all run clean distclean install

all: $(BIN)

run: $(BIN)
	@./$(BIN)

clean:
	rm -rf build/linux build/win

distclean:
	rm -rf build

install: $(BIN)
	cp $(BIN) $(INSTALL_DIR)

$(BIN): $(MAIN) $(OBJ) | $(BIN_DIR)
	@echo Building $@...
	@$(CC) $(CFLAGS) $(CPPFALGS) $^ -o $@ $(LDFLAGS)
	@echo Done. $@..."\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h | $(OBJ_DIR)
	@echo Building $@...
	@$(CC) -c -fPIC $(CFLAGS) $(CPPFALGS) $< -o $@
	@echo Done. $@..."\n"

$(BUILD_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@mkdir -p $@

$(OBJ_DIR):
	@mkdir -p $@
