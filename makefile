# Variables
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build

LEXER = $(SRC_DIR)/lexer.l
PARSER = $(SRC_DIR)/parser.y
LEXER_CPP = $(BUILD_DIR)/lexer.cpp
PARSER_CPP = $(BUILD_DIR)/parser.cpp
PARSER_HPP = $(BUILD_DIR)/parser.hpp
AST_HPP = $(SRC_DIR)/AST.hpp
AST_CPP = $(SRC_DIR)/AST.cpp
DS_HPP = $(SRC_DIR)/data_structures.hpp
DS_CPP = $(SRC_DIR)/data_structures.cpp
TAC_HPP = $(SRC_DIR)/tac.hpp
TAC_GEN_CPP = $(SRC_DIR)/tac_gen.cpp
TAC_OPT_CPP = $(SRC_DIR)/tac_opt.cpp
DRIVER = $(SRC_DIR)/driver.cpp

OUTPUT = $(BIN_DIR)/ir_gen
OBJS = $(BUILD_DIR)/lexer.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/AST.o $(BUILD_DIR)/data_structures.o \
       $(BUILD_DIR)/tac_gen.o $(BUILD_DIR)/tac_opt.o $(BUILD_DIR)/driver.o

# Compiler and tools
FLEX = flex
BISON = bison
CXX = g++ 
CXXFLAGS = -g -I$(SRC_DIR)

# Default target
all: $(OUTPUT)

# Ensure build directory exists before running Flex
$(LEXER_CPP): $(LEXER) | $(BUILD_DIR)
	$(FLEX) -o $(LEXER_CPP) $(LEXER)

# Ensure build directory exists before running Bison
$(PARSER_CPP) $(PARSER_HPP): $(PARSER) | $(BUILD_DIR)
	$(BISON) -d -o $(PARSER_CPP) $(PARSER) -Wno-conflicts-rr -Wno-conflicts-sr

# Compile the lexer object file
$(BUILD_DIR)/lexer.o: $(LEXER_CPP) $(PARSER_HPP) $(AST_HPP) $(DS_HPP)
	$(CXX) $(CXXFLAGS) -c -o $@ $(LEXER_CPP)

# Compile the parser object file
$(BUILD_DIR)/parser.o: $(PARSER_CPP) $(PARSER_HPP) $(AST_HPP) $(DS_HPP)
	$(CXX) $(CXXFLAGS) -c -o $@ $(PARSER_CPP)

# Compile the AST object file
$(BUILD_DIR)/AST.o: $(AST_CPP) $(AST_HPP)
	$(CXX) $(CXXFLAGS) -c -o $@ $(AST_CPP)

# Compile the data_structures object file
$(BUILD_DIR)/data_structures.o: $(DS_CPP) $(DS_HPP)
	$(CXX) $(CXXFLAGS) -c -o $@ $(DS_CPP)

# Compile the TAC generator object file
$(BUILD_DIR)/tac_gen.o: $(TAC_GEN_CPP) $(TAC_HPP) $(DS_HPP)
	$(CXX) $(CXXFLAGS) -c -o $@ $(TAC_GEN_CPP)

# Compile the TAC optimizer object file
$(BUILD_DIR)/tac_opt.o: $(TAC_OPT_CPP) $(TAC_HPP) $(DS_HPP)
	$(CXX) $(CXXFLAGS) -c -o $@ $(TAC_OPT_CPP)

# Compile the driver object file
$(BUILD_DIR)/driver.o: $(DRIVER) $(TAC_HPP) $(DS_HPP) 
	$(CXX) $(CXXFLAGS) -c -o $@ $(DRIVER)

# Link the final binary
$(OUTPUT): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Create the build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean up generated files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean
