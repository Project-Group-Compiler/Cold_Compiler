# Variables
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build

LEXER = $(SRC_DIR)/lexer.l
PARSER = $(SRC_DIR)/parser.y
LEXER_CPP = $(BUILD_DIR)/lexer.cpp
PARSER_CPP = $(BUILD_DIR)/parser.cpp
PARSER_HPP = $(BUILD_DIR)/parser.hpp

OUTPUT = $(BIN_DIR)/compiler
OBJS = $(BUILD_DIR)/lexer.o

# Compiler and tools
FLEX = flex
BISON = bison
CXX = g++

# Default target
all: $(OUTPUT)

# Ensure build directory exists before running Bison
$(PARSER_CPP) $(PARSER_HPP): $(PARSER) | $(BUILD_DIR)
	$(BISON) -d -o $(PARSER_CPP) $(PARSER)

# Ensure build directory exists before running Flex
$(LEXER_CPP): $(LEXER) $(PARSER_HPP) | $(BUILD_DIR)
	$(FLEX) -o $(LEXER_CPP) $(LEXER)

# Compile the lexer object file
$(BUILD_DIR)/lexer.o: $(LEXER_CPP) $(PARSER_HPP)
	$(CXX) -c -o $@ $(LEXER_CPP)

# Link the final compiler binary
$(OUTPUT): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^

# Create the build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean up generated files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the lexer/parser on input files
run: $(OUTPUT)
	$(OUTPUT) $(ARGS)

# Phony targets
.PHONY: all clean run
