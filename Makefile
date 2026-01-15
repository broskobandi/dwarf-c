PROJECT := game
CC = clang
CFLAGS = -Wall -Wextra -Wunused-result -Wconversion
CPPFLAGS = -Isrc
LDFLAGS = -lSDL2
OS = linux
BUILD_TYPE = debug

BUILD_DIR := build/$(OS)/$(BUILD_TYPE)
SRC_DIR := src
OBJ_DIR := $(BUILD_DIR)/obj

MAIN := $(SRC_DIR)/main.c
SRC := $(filter-out $(MAIN), $(wildcard $(SRC_DIR)/*.c))
INC := $(wildcard $(SRC_DIR)/*.h)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BIN := $(BUILD_DIR)/$(PROJECT)

.PHONY: all run clean

all: $(BIN)

run: $(BIN)
	./$<

clean:
	rm -rf build

$(BIN): $(MAIN) $(OBJ) $(INC) | $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(MAIN) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h | $(OBJ_DIR)
	@$(CC) -c -fPIC $(CFLAGS) $(CPPFLAGS) $< -o $@

$(BUILD_DIR):
	@mkdir -p $@

$(OBJ_DIR):
	@mkdir -p $@
