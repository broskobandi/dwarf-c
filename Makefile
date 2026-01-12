# Project details
PROJECT := game
OS = linux
BUILD_TYPE = debug
CC = gcc
CFLAGS = -Wall -Wextra -Wunused-result -Wconversion
CPPFLAGS = -Isrc
LDFLAGS = -lSDL2

# Project dirs
BUILD_DIR := build/$(OS)/$(BUILD_TYPE)
OBJ_DIR := $(BUILD_DIR)/obj
SRC_DIR := src

# Files
MAIN := $(SRC_DIR)/main.c
SRC := $(filter-out $(MAIN), $(wildcard $(SRC_DIR)/*.c))
BIN := $(BUILD_DIR)/$(PROJECT)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all run clean

all: $(BIN)

run: $(BIN)
	./$<

clean:
	rm -rf build

$(BIN): $(MAIN) $(OBJ) | $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h | $(OBJ_DIR)
	@$(CC) -c -fPIC $(CFLAGS) $(CPPFLAGS) $< -o $@

$(BUILD_DIR):
	@mkdir -p $@

$(OBJ_DIR):
	@mkdir -p $@
