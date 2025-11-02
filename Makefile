COMPILER_DIR := compiler
BIN_DIR := bin

COMPILER_SOURCES := $(wildcard $(COMPILER_DIR)/*.c) \
			$(wildcard $(COMPILER_DIR)/**/*.c)

COMPILER_OBJECTS := $(patsubst %.c,$(BIN_DIR)/%.o,$(COMPILER_SOURCES))

COMPILER_TARGET := $(BIN_DIR)/tuffc

CC=clang
CC_FLAGS=-std=c23 -Icompiler -Iinclude -ggdb

$(COMPILER_TARGET): $(COMPILER_OBJECTS)
	$(CC)  $(COMPILER_OBJECTS) -o $(COMPILER_TARGET)

$(BIN_DIR)/%.o: %.c
	@if not exist $(subst /,\, $(dir $@)) mkdir $(subst /,\, $(dir $@))
	$(CC) $(CC_FLAGS) -c $< -o $@

.PHONY: all clean

all: $(COMPILER_TARGET)

clean:
	rm -f $(COMPILER_OBJECTS) $(COMPILER_TARGET)

