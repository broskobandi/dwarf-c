PROJECT := dwarf
COMPILE_COMMANDS := build/compile_commands.json
OS = linux
BUILD_TYPE = debug
CC = bear --output $(COMPILE_COMMANDS) -- clang
CFLAGS = -Wall -Wextra -Wunused-result -Wconversion
CPPFLAGS = -Isrc
LDFLAGS = -lSDL2

BUILD_DIR := build/$(OS)/$(BUILD_TYPE)
SRC_DIR := src
OBJ_DIR := $(BUILD_DIR)/obj
ASSETS_DIR := $(BUILD_DIR)/assets

MAIN := $(SRC_DIR)/main.c
SRC := $(filter-out $(MAIN), $(wildcard $(SRC_DIR)/*.c))

BIN := $(BUILD_DIR)/$(PROJECT)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all run clean distclean

all: $(BIN)

run: $(BIN)
	./$<

clean:
	rm -rf $(BIN) $(OBJ) $(COMPILE_COMMANDS)

distclean:
	rm -rf build

$(BIN): $(MAIN) $(OBJ) | $(BUILD_DIR) $(ASSETS_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h | $(OBJ_DIR)
	$(CC) -c -fPIC $(CFLAGS) $(CPPFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

$(ASSETS_DIR):
	cp -r assets $(BUILD_DIR)/
