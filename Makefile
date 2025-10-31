COMPILER_DIR := compiler
BIN_DIR := bin

COMPILER_SOURCES := $(wildcard $(COMPILER_DIR)/*.cc) \
			$(wildcard $(COMPILER_DIR)/**/*.cc)

COMPILER_OBJECTS := $(patsubst %.cc,$(BIN_DIR)/%.o,$(COMPILER_SOURCES))

COMPILER_TARGET := $(BIN_DIR)/tuffc

CXX=clang++
CXX_FLAGS=-std=c++23 -Icompiler

$(COMPILER_TARGET): $(COMPILER_OBJECTS)
	$(CXX)  $(COMPILER_OBJECTS) -o $(COMPILER_TARGET)

$(BIN_DIR)/%.o: %.cc
	if not exist $(subst /,\, $(dir $@)) mkdir $(subst /,\, $(dir $@))
	$(CXX) $(CXX_FLAGS) -c $< -o $@

.PHONY: all clean

all: $(COMPILER_TARGET)

clean:
	rm -f $(COMPILER_OBJECTS) $(COMPILER_TARGET)

