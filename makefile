# Variables
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build

LEXER = $(SRC_DIR)/lexer.l
PARSER = $(SRC_DIR)/parser.y
LEXER_CPP = $(BUILD_DIR)/lexer.cpp
PARSER_CPP = $(BUILD_DIR)/parser.cpp
PARSER_HPP = $(BUILD_DIR)/parser.hpp
AST_HPP = $(BUILD_DIR)/AST.hpp
AST_CPP = $(SRC_DIR)/AST.cpp
DRIVER = $(SRC_DIR)/driver.cpp

OUTPUT = $(BIN_DIR)/parser
OBJS = $(BUILD_DIR)/lexer.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/AST.o $(BUILD_DIR)/driver.o

# Compiler and tools
FLEX = flex
BISON = bison
CXX = g++

# Default target
all: $(OUTPUT)

# Ensure build directory exists before running Flex
$(LEXER_CPP): $(LEXER) | $(BUILD_DIR)
	$(FLEX) -o $(LEXER_CPP) $(LEXER)

# Ensure build directory exists before running Bison
$(PARSER_CPP) $(PARSER_HPP): $(PARSER) | $(BUILD_DIR)
	$(BISON) -d -o $(PARSER_CPP) $(PARSER)

# Ensure AST.hpp is in the build directory
$(AST_HPP): $(SRC_DIR)/AST.hpp | $(BUILD_DIR)
	cp $(SRC_DIR)/AST.hpp $(AST_HPP)

# Compile the lexer object file
$(BUILD_DIR)/lexer.o: $(LEXER_CPP) $(PARSER_HPP) $(AST_HPP)
	$(CXX) -c -g -o $@ $(LEXER_CPP)

# Compile the parser object file
$(BUILD_DIR)/parser.o: $(PARSER_CPP) $(PARSER_HPP) $(AST_HPP)
	$(CXX) -c -g -o $@ $(PARSER_CPP)

# Compile the AST object file
$(BUILD_DIR)/AST.o: $(AST_CPP) $(AST_HPP)
	$(CXX) -c -g -o $@ $(AST_CPP)

# Compile the driver object file
$(BUILD_DIR)/driver.o: $(DRIVER)
	$(CXX) -c -g -o $@ $(DRIVER)

# Link the final parser binary
$(OUTPUT): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -g -o $@ $^

# Create the build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean up generated files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean
